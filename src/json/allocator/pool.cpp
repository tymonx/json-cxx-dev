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
 * @file json/allocator/pool.cpp
 *
 * @brief Implementation
 */

#include "json/allocator/pool.hpp"

#include <cstring>

using json::allocator::Pool;

static constexpr std::uintptr_t ALIGN_MAX = alignof(std::max_align_t);
static constexpr std::uintptr_t ALIGN_OFFSET = ALIGN_MAX - 1u;
static constexpr std::uintptr_t ALIGN_MASK = ~ALIGN_OFFSET;

struct Pool::Header {
    Header* prev{nullptr};
    std::size_t size{0};
};

std::uintptr_t Pool::align(std::uintptr_t address) noexcept {
    return (address + sizeof(Header) + ALIGN_OFFSET) & ALIGN_MASK;
}

std::uintptr_t Pool::align(void* address) noexcept {
    return align(std::uintptr_t(address));
}

Pool::Header* Pool::header_cast(std::uintptr_t address) noexcept {
    return reinterpret_cast<Header*>(address - sizeof(Header));
}

Pool::Header* Pool::header_cast(void* address) noexcept {
    return header_cast(std::uintptr_t(address));
}

Pool::Pool(std::uintptr_t memory_begin, std::uintptr_t memory_end) noexcept :
    m_memory_begin{align(memory_begin)},
    m_memory_end{memory_end},
    m_header_last{header_cast(m_memory_begin)}
{
    if (m_memory_begin < m_memory_end) {
        *m_header_last = {};
    }
    else {
        m_header_last = nullptr;
    }
}

Pool::~Pool() noexcept { }

void* Pool::allocate(std::size_t size) noexcept {
    void* ptr = nullptr;

    if (size) {
        Header* header_next = nullptr;
        Header* header = m_header_last;

        while (header && !ptr) {
            auto address_end = header_next ?
                std::uintptr_t(header_next) : m_memory_end;

            auto address = align(std::uintptr_t(header) + sizeof(Header) +
                    header->size);

            if ((address + size) <= address_end) {
                Header* header_prev = header;
                header = header_cast(address);
                header->size = size;
                header->prev = header_prev;

                if (header_next) {
                    header_next->prev = header;
                }
                else {
                    m_header_last = header;
                }

                ptr = reinterpret_cast<void*>(address);
            }
            else {
                header_next = header;
                header = header->prev;
            }
        }
    }

    return ptr;
}

void* Pool::reallocate(void* ptr, std::size_t size) noexcept {
    if (ptr) {
        if (size) {
            auto header = header_cast(ptr);

            if (header->size < size) {
                auto tmp = allocate(size);
                if (tmp) {
                    std::memcpy(tmp, ptr, header->size);
                    deallocate(ptr);
                }
                ptr = tmp;
            }
            else {
                header->size = size;
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

void Pool::deallocate(void* ptr) noexcept {
    if (ptr) {
        Header* header_next = nullptr;
        Header* header = header_cast(ptr);
        Header* it = m_header_last;

        while (it) {
            if (it == header) {
                if (header_next) {
                    header_next->prev = header->prev;
                }
                else {
                    m_header_last = header->prev;
                }

                it = nullptr;
            }
            else {
                header_next = it;
                it = it->prev;
            }
        }
    }
}
