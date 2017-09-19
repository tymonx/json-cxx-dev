/*!
 * @copright
 * Copyright (c) 2017, Tymoteusz Blazejczyk
 * All rights reserved.
 *
 * @copright
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * @copright
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * @copright
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * @copright
 * * Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * @copright
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * @file json/allocator/block.cpp
 *
 * @brief Implementation
 */

#include "json/allocator/block.hpp"
#include "json/allocator/pool.hpp"

#include <new>
#include <cstdlib>
#include <cstring>

using json::allocator::Block;
using json::allocator::Pool;

struct Header {
    Header* prev;
    Pool allocator;
};

Block::~Block() noexcept { }

void* Block::allocate(std::size_t size) noexcept {
    void* ptr = nullptr;

    if (size) {
        auto header = static_cast<Header*>(m_header_last);

        while (header && !ptr) {
            ptr = header->allocator.allocate(size);
            header = header->prev;
        }

        if (!ptr) {
            auto block_size = Pool::MINIMAL_SIZE + size;

            if (block_size < m_block_size) {
                block_size = m_block_size;
            }

            header = static_cast<Header*>(std::malloc(block_size));

            if (header) {
                header->prev = static_cast<Header*>(m_header_last);
                m_header_last = header;
                new (&header->allocator) Pool(
                        std::uintptr_t(header) + sizeof(Header),
                        std::uintptr_t(header) + block_size
                    );
            }
        }
    }

    return ptr;
}

void* Block::reallocate(void* ptr, std::size_t size) noexcept {
    if (ptr) {
        if (size) {
            Header* header = static_cast<Header*>(m_header_last);
            Header* header_next = nullptr;

            while (header && !header->allocator.valid(ptr)) {
                header_next = header;
                header = header->prev;
            }

            if (header) {
                auto reallocated = header->allocator.reallocate(ptr, size);
                if (!reallocated) {
                    reallocated = allocate(size);
                    if (reallocated) {
                        if (size > header->allocator.size(ptr)) {
                            size = header->allocator.size(ptr);
                        }
                        std::memcpy(reallocated, ptr, size);
                        header->allocator.deallocate(ptr);
                        if (header->allocator.empty()) {
                            if (header_next) {
                                header_next->prev = header->prev;
                            }
                            else {
                                m_header_last = header->prev;
                            }
                            header->allocator.~Pool();
                            std::free(header);
                        }
                    }
                }
                ptr = reallocated;
            }
        }
        else {
            deallocate(ptr);
            ptr = nullptr;
        }
    }
    else {
        ptr = allocate(size);
    }

    return ptr;
}

void Block::deallocate(void* ptr) noexcept {
    if (ptr) {
        Header* header = static_cast<Header*>(m_header_last);
        Header* header_next = nullptr;

        while (header && !header->allocator.valid(ptr)) {
            header_next = header;
            header = header->prev;
        }

        if (header) {
            header->allocator.deallocate(ptr);
            if (header->allocator.empty()) {
                if (header_next) {
                    header_next->prev = header->prev;
                }
                else {
                    m_header_last = header->prev;
                }
                header->allocator.~Pool();
                std::free(header);
            }
        }
    }
}
