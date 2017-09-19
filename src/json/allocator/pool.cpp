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

struct Header {
    Header* prev{nullptr};
    std::size_t size{0};
};

const std::size_t Pool::MINIMAL_SIZE{2 * sizeof(Header)};

static inline std::uintptr_t align(std::uintptr_t address) noexcept {
    return (address + sizeof(Header) + ALIGN_OFFSET) & ALIGN_MASK;
}

static inline Header* header_cast(std::uintptr_t address) noexcept {
    return reinterpret_cast<Header*>(address - sizeof(Header));
}

static inline Header* header_cast(const void* address) noexcept {
    return header_cast(std::uintptr_t(address));
}

Pool::Pool(std::uintptr_t memory_begin, std::uintptr_t memory_end) noexcept :
    m_memory_begin{align(memory_begin)},
    m_memory_end{memory_end},
    m_header_last{header_cast(m_memory_begin)}
{
    if (m_memory_begin < m_memory_end) {
        *static_cast<Header*>(m_header_last) = {};
    }
    else {
        m_header_last = nullptr;
    }
}

Pool::~Pool() noexcept { }

std::size_t Pool::size(const void* ptr) const noexcept {
    return header_cast(ptr)->size;
}

void* Pool::allocate(std::size_t size) noexcept {
    void* ptr = nullptr;

    if (size) {
        Header* header_next = nullptr;
        Header* header = static_cast<Header*>(m_header_last);
        std::uintptr_t address = 0;
        std::uintptr_t address_end = m_memory_end;

        while (header && !ptr) {
            address = align(std::uintptr_t(header) + sizeof(Header) +
                    header->size);

            if ((address + size) <= address_end) {
                ptr = reinterpret_cast<void*>(address);
            }
            else {
                address_end = std::uintptr_t(header);
                header_next = header;
                header = header->prev;
            }
        }

        if (ptr) {
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
        }
    }

    return ptr;
}

void* Pool::reallocate(void* ptr, std::size_t size) noexcept {
    if (ptr) {
        if (size) {
            auto header = header_cast(ptr);

            if (header->size < size) {
                auto allocated = allocate(size);
                if (allocated) {
                    std::memcpy(allocated, ptr, header->size);
                    deallocate(ptr);
                }
                ptr = allocated;
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
        Header* it = static_cast<Header*>(m_header_last);

        while (it && (it != header)) {
            header_next = it;
            it = it->prev;
        }

        if (it) {
            if (header_next) {
                header_next->prev = header->prev;
            }
            else {
                m_header_last = header->prev;
            }
        }
    }
}
