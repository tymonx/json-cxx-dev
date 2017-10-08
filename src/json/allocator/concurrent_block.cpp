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
 * @file json/allocator/concurrent_block.cpp
 *
 * @brief Implementation
 */

#include "json/allocator/concurrent_block.hpp"

using json::allocator::ConcurrentBlock;

ConcurrentBlock::~ConcurrentBlock() noexcept { }

void* ConcurrentBlock::allocate(Size size) noexcept {
    std::lock_guard<std::mutex> lock{m_mutex};

    return m_allocator.allocate(size);
}

void* ConcurrentBlock::reallocate(void* ptr, Size size) noexcept {
    std::lock_guard<std::mutex> lock{m_mutex};

    return m_allocator.reallocate(ptr, size);
}

void ConcurrentBlock::deallocate(void* ptr) noexcept {
    std::lock_guard<std::mutex> lock{m_mutex};

    m_allocator.deallocate(ptr);
}

json::Size ConcurrentBlock::size(const void* ptr) const noexcept {
    return m_allocator.size(ptr);
}
