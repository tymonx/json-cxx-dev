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
 * @file json/allocator.cpp
 *
 * @brief Implementation
 */

#include "json/allocator.hpp"

using json::Allocator;

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
