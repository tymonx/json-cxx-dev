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
 * @file json/allocator/pool.hpp
 *
 * @brief Interface
 */

#ifndef JSON_ALLOCATOR_POOL_HPP
#define JSON_ALLOCATOR_POOL_HPP

#include "json/allocator.hpp"

#include <cstdint>

namespace json {
namespace allocator {

class Pool final : public Allocator {
public:
    static const Size MINIMAL_SIZE;

    Pool(void* memory, Size size) noexcept;

    Pool(void* memory_begin, void* memory_end) noexcept;

    virtual void* allocate(Size size) noexcept override;

    virtual void* reallocate(void* ptr, Size size) noexcept override;

    virtual void deallocate(void* ptr) noexcept override;

    virtual Size size(const void* ptr) const noexcept override;

    bool valid(const void* ptr) const noexcept;

    bool empty() const noexcept;

    virtual ~Pool() noexcept override;
private:
    Pool(const Pool&) = delete;
    Pool& operator=(const Pool&) = delete;

    void* m_memory_begin{nullptr};
    void* m_memory_end{nullptr};
    void* m_header_last{nullptr};
};

inline
Pool::Pool(void* memory, Size size) noexcept :
    Pool{memory, reinterpret_cast<void*>(std::uintptr_t(memory) + size)}
{ }

inline auto
Pool::valid(const void* ptr) const noexcept -> bool {
    return (ptr >= m_memory_begin) && (ptr < m_memory_end);
}

inline auto
Pool::empty() const noexcept -> bool {
    return (m_header_last < m_memory_begin);
}

}
}

#endif /* JSON_ALLOCATOR_POOL_HPP */
