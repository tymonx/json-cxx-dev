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
 * @file json/allocator.cpp
 *
 * @brief Implementation
 */

#include "json/allocator.hpp"

using json::Allocator;

bool Allocator::bad_allocation() const noexcept {
    return m_bad_allocation;
}

bool Allocator::bad_allocation(std::uintptr_t) noexcept {
    auto tmp = m_bad_allocation;
    m_bad_allocation = false;
    return tmp;
}

Allocator::~Allocator() noexcept { }

#if defined(JSON_ALLOCATOR_BLOCK)

#include "json/allocator/block.hpp"

Allocator& Allocator::get_instance() noexcept {
    static allocator::Block instance;
    return instance;
}

#elif defined(JSON_ALLOCATOR_POOL)

#include "json/allocator/pool.hpp"

#include <array>
#include <cstdint>

#ifndef JSON_ALLOCATOR_POOL_SIZE
    #define JSON_ALLOCATOR_POOL_SIZE 4096
#endif

static std::array<std::uint8_t, JSON_ALLOCATOR_POOL_SIZE> g_memory;

Allocator& Allocator::get_instance() noexcept {
    static allocator::Pool instance{g_memory.data(), g_memory.size()};
    return instance;
}

#elif defined(JSON_ALLOCATOR_STANDARD)

#include "json/allocator/standard.hpp"

Allocator& Allocator::get_instance() noexcept {
    static allocator::Standard instance;
    return instance;
}

#else

#include "json/allocator/concurrent_block.hpp"

Allocator& Allocator::get_instance() noexcept {
    static allocator::ConcurrentBlock instance;
    return instance;
}

#endif
