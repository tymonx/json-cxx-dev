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
#include <algorithm>

using json::allocator::Block;
using json::allocator::Pool;

struct Header {
    Header* prev;
    Pool allocator;
};

static inline void copy(const void* src, json::Size len, void* dst) noexcept {
    std::copy_n(static_cast<const std::uint8_t*>(src), len,
            static_cast<std::uint8_t*>(dst));
}

Block::~Block() noexcept { }

void* Block::allocate(Size size) noexcept {
    void* ptr = nullptr;

    if (size) {
        auto header = static_cast<Header*>(m_header_last);

        while (header && !ptr) {
            ptr = header->allocator.allocate(size);
            header = header->prev;
        }

        if (!ptr) {
            auto block_size = sizeof(Header) + Pool::MINIMAL_SIZE + size;

            if (block_size < m_block_size) {
                block_size = m_block_size;
            }

            auto block = new (std::nothrow) std::uint8_t[block_size];

            if (block) {
                header = reinterpret_cast<Header*>(block);
                header->prev = static_cast<Header*>(m_header_last);
                m_header_last = header;
                new (&header->allocator) Pool(block + sizeof(Header),
                        block + block_size);

                ptr = header->allocator.allocate(size);
            }
        }
    }

    return ptr;
}

void* Block::reallocate(void* ptr, Size size) noexcept {
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
                        auto allocated_size = header->allocator.size(ptr);

                        if (allocated_size > size) {
                            allocated_size = size;
                        }

                        copy(ptr, allocated_size, reallocated);
                        header->allocator.deallocate(ptr);
                        if (header->allocator.empty()) {
                            if (header_next) {
                                header_next->prev = header->prev;
                            }
                            else {
                                m_header_last = header->prev;
                            }
                            header->allocator.~Pool();
                            delete [] reinterpret_cast<std::uint8_t*>(header);
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
                delete [] reinterpret_cast<std::uint8_t*>(header);
            }
        }
    }
}

json::Size Block::size(const void* ptr) const noexcept {
    return m_header_last ?
        static_cast<Header*>(m_header_last)->allocator.size(ptr) : 0;
}
