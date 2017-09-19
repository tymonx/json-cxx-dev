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
 * @file json/allocator/concurrent_block.hpp
 *
 * @brief Interface
 */

#ifndef JSON_ALLOCATOR_CONCURRENT_BLOCK_HPP
#define JSON_ALLOCATOR_CONCURRENT_BLOCK_HPP

#include "json/allocator.hpp"
#include "json/allocator/block.hpp"

#include <mutex>

namespace json {
namespace allocator {

class ConcurrentBlock final : public Allocator {
public:
    static constexpr auto DEFAULT_SIZE{Block::DEFAULT_SIZE};

    ConcurrentBlock() noexcept = default;

    ConcurrentBlock(std::size_t block_size) noexcept;

    virtual void* allocate(std::size_t size) noexcept override;

    virtual void* reallocate(void* ptr, std::size_t size) noexcept override;

    virtual void deallocate(void* ptr) noexcept override;

    virtual ~ConcurrentBlock() noexcept override;
private:
    std::mutex m_mutex{};
    Block m_allocator{};
};

inline
ConcurrentBlock::ConcurrentBlock(std::size_t block_size) noexcept :
    m_allocator{block_size}
{ }

}
}

#endif /* JSON_ALLOCATOR_CONCURRENT_BLOCK_HPP */
