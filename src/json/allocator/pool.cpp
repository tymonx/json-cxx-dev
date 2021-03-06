/*!
 * @copyright
 * Copyright 2017 Tymoteusz Blazejczyk
 *
 * @copyright
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * @copyright
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * @copyright
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @file json/allocator/pool.cpp
 *
 * @brief Implementation
 */

#include "json/allocator/pool.hpp"

#include <algorithm>

using json::Size;
using json::allocator::Pool;

static constexpr std::uintptr_t ALIGN_MAX = alignof(std::max_align_t);
static constexpr std::uintptr_t ALIGN_OFFSET = ALIGN_MAX - 1u;
static constexpr std::uintptr_t ALIGN_MASK = ~ALIGN_OFFSET;

struct Header {
    Header* prev{nullptr};
    Size size{0};
};

const Size Pool::MINIMAL_SIZE{2 * sizeof(Header)};

static inline void* add(const void* address, std::uintptr_t offset) noexcept {
    return reinterpret_cast<void*>(std::uintptr_t(address) + offset);
}

static inline void* align(const void* address) noexcept {
    return reinterpret_cast<void*>((std::uintptr_t(address) + sizeof(Header) +
                ALIGN_OFFSET) & ALIGN_MASK);
}

static inline Header* header_cast(const void* address) noexcept {
    return reinterpret_cast<Header*>(std::uintptr_t(address) - sizeof(Header));
}

static inline void copy(const void* src, Size len, void* dst) noexcept {
    std::copy_n(static_cast<const std::uint8_t*>(src), len,
            static_cast<std::uint8_t*>(dst));
}

static inline Size distance(const void* lhs, const void* rhs) noexcept {
    return Size(std::uintptr_t(rhs) - std::uintptr_t(lhs));
}

Pool::Pool(void* memory_begin, void* memory_end) noexcept :
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

void* Pool::allocate(Size size) noexcept {
    void* ptr = nullptr;

    if (size) {
        Header* header = static_cast<Header*>(m_header_last);
        void* address = nullptr;
        void* address_end = m_memory_end;

        while (header && !ptr) {
            address = align(add(header, sizeof(Header) + header->size));

            if (add(address, size) <= address_end) {
                ptr = address;
            }
            else {
                address_end = header;
                header = header->prev;
            }
        }

        if (ptr) {
            Header* header_prev = header;
            header = header_cast(address);
            header->size = size;
            header->prev = header_prev;

            if (address_end == m_memory_end) {
                m_header_last = header;
            }
            else {
                static_cast<Header*>(address_end)->prev = header;
            }
        }
    }

    return ptr;
}

void* Pool::reallocate(void* ptr, Size size) noexcept {
    if (ptr) {
        if (size) {
            auto header = header_cast(ptr);

            if (header->size < size) {
                void* address_end = static_cast<Header*>(m_memory_end);

                if (header != m_header_last) {
                    auto header_next = static_cast<Header*>(m_header_last);

                    while (header_next && (header_next->prev != header)) {
                        header_next = header_next->prev;
                    }

                    address_end = header_next;
                }

                if (distance(ptr, address_end) < size) {
                    auto allocated = allocate(size);
                    if (allocated) {
                        copy(ptr, header->size, allocated);
                        deallocate(ptr);
                    }
                    ptr = allocated;
                }
                else {
                    header->size = size;
                }
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

json::Size Pool::size(const void* ptr) const noexcept {
    return ptr ? header_cast(ptr)->size : 0;
}
