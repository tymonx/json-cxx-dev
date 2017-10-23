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
 * @file json/allocator/standard.cpp
 *
 * @brief Implementation
 */

#include "json/allocator/standard.hpp"

#include <cstdlib>

using json::allocator::Standard;

Standard::~Standard() noexcept { }

void* Standard::allocate(Size size) noexcept {
    void* ptr;

    if (size) {
        ptr = std::malloc(size);
        if (!ptr) {
            raise_bad_allocation();
        }
    }
    else {
        ptr = nullptr;
    }

    return ptr;
}

void* Standard::reallocate(void* ptr, Size size) noexcept {
    if (size) {
        ptr = std::realloc(ptr, size);
        if (!ptr) {
            raise_bad_allocation();
        }
    }
    else if (ptr) {
        ptr = std::realloc(ptr, size);
    }
    else {
        ptr = nullptr;
    }

    return ptr;
}

void Standard::deallocate(void* ptr) noexcept {
    std::free(ptr);
}

json::Size Standard::size(const void* /* ptr */) const noexcept {
    return 0;
}
