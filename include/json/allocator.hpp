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
 * @file json/allocator.hpp
 *
 * @brief Interface
 */

#ifndef JSON_ALLOCATOR_HPP
#define JSON_ALLOCATOR_HPP

#include "types.hpp"

#include <cstddef>

namespace json {

class Allocator {
public:
    static Allocator& get_instance() noexcept;

    Allocator() noexcept = default;

    virtual void* allocate(Size size) noexcept = 0;

    template<typename T>
    T* allocate(Size n = 1) noexcept;

    virtual void* reallocate(void* ptr, Size size) noexcept = 0;

    template<typename T>
    T* reallocate(T* ptr, Size n = 1) noexcept;

    virtual void deallocate(void* ptr) noexcept = 0;

    template<typename T>
    void deallocate(T* ptr) noexcept;

    virtual Size size(const void* ptr) const noexcept = 0;

    virtual ~Allocator() noexcept;
private:
    Allocator(const Allocator&) = delete;
    Allocator& operator=(const Allocator&) = delete;
};

template<typename T> auto
Allocator::allocate(Size n) noexcept -> T* {
    return static_cast<T*>(allocate(sizeof(T) * n));
}

template<typename T> auto
Allocator::reallocate(T* ptr, Size n) noexcept -> T* {
    return static_cast<T*>(reallocate(static_cast<void*>(ptr), sizeof(T) * n));
}

template<typename T> void
Allocator::deallocate(T* ptr) noexcept {
    deallocate(static_cast<void*>(ptr));
}

}

#endif /* JSON_ALLOCATOR_HPP */
