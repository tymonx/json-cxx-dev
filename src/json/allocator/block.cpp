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
#include <cstring>

using json::allocator::Block;

struct Block::Header {
    Header* prev;
    std::uint8_t* block;
    Pool allocator;
};

const std::uintptr_t Block::ALIGN_MAX = alignof(Header);
const std::uintptr_t Block::ALIGN_OFFSET = ALIGN_MAX - 1u;
const std::uintptr_t Block::ALIGN_MASK = ~ALIGN_OFFSET;
const std::size_t Block::MINIMAL_SIZE = 4 * sizeof(Header);

std::uintptr_t Block::align(std::uintptr_t address) noexcept {
    return (address + sizeof(Header) + ALIGN_OFFSET) & ALIGN_MASK;
}

Block::Header* Block::header_cast(std::uintptr_t address) noexcept {
    return reinterpret_cast<Header*>(address - sizeof(Header));
}

Block::~Block() noexcept { }

void* Block::allocate(std::size_t size) noexcept {
    void* ptr = nullptr;

    if (size) {
        auto header = m_header_last;

        while (header && !ptr) {
            ptr = header->allocator.allocate(size);
            header = header->prev;
        }

        if (!ptr) {
            auto block_size = MINIMAL_SIZE + size;

            if (block_size < m_block_size) {
                block_size = m_block_size;
            }

            auto block = new (std::nothrow) std::uint8_t[block_size];

            if (block) {
                auto memory_begin = align(std::uintptr_t(block));
                auto memory_end = std::uintptr_t(block) + block_size;

                header = header_cast(memory_begin);
                header->block = block;
                header->prev = m_header_last;
                m_header_last = header;
                new (&header->allocator) Pool(memory_begin, memory_end);
            }
        }
    }

    return ptr;
}

void* Block::reallocate(void* ptr, std::size_t size) noexcept {
    if (ptr) {
        if (size) {
            Header* header = m_header_last;
            Header* header_next = nullptr;

            while (header) {
                if (header->allocator.is_valid(ptr)) {
                    auto reallocated = header->allocator.reallocate(ptr, size);
                    if (!reallocated) {
                        reallocated = allocate(size);
                        if (reallocated) {
                            std::memcpy(reallocated, ptr, size);
                            header->allocator.deallocate(ptr);
                            if (header->allocator.empty()) {
                                if (header_next) {
                                    header_next->prev = header->prev;
                                }
                                header->allocator.~Pool();
                                delete [] header->block;
                            }
                        }
                    }
                    ptr = reallocated;
                    header = nullptr;
                }
                else {
                    header_next = header;
                    header = header->prev;
                }
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
        Header* header = m_header_last;
        Header* header_next = nullptr;

        while (header) {
            if (header->allocator.is_valid(ptr)) {
                header->allocator.deallocate(ptr);
                if (header->allocator.empty()) {
                    if (header_next) {
                        header_next->prev = header->prev;
                    }
                    header->allocator.~Pool();
                    delete [] header->block;
                }
                header = nullptr;
            }
            else {
                header_next = header;
                header = header->prev;
            }
        }
    }
}
