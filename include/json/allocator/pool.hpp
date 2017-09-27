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
