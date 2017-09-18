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
 * @file json/allocator.hpp
 *
 * @brief Interface
 */

#ifndef JSON_ALLOCATOR_HPP
#define JSON_ALLOCATOR_HPP

#include <cstddef>

namespace json {

class Allocator {
public:
    Allocator() noexcept = default;

    virtual void* allocate(std::size_t size) noexcept = 0;

    template<typename T>
    T* allocate(std::size_t n) noexcept;

    virtual void* reallocate(void* ptr, std::size_t size) noexcept = 0;

    template<typename T>
    T* reallocate(T* ptr, std::size_t n) noexcept;

    virtual void deallocate(void* ptr) noexcept = 0;

    template<typename T>
    void deallocate(T* ptr) noexcept;

    virtual ~Allocator() noexcept;
private:
    Allocator(const Allocator&) = delete;
    Allocator& operator=(const Allocator&) = delete;
};

template<typename T> auto
Allocator::allocate(std::size_t n) noexcept -> T* {
    return static_cast<T*>(allocate(sizeof(T) * n));
}

template<typename T> auto
Allocator::reallocate(T* ptr, std::size_t n) noexcept -> T* {
    return static_cast<T*>(reallocate(static_cast<void*>(ptr), sizeof(T) * n));
}

template<typename T> void
Allocator::deallocate(T* ptr) noexcept {
    deallocate(static_cast<void*>(ptr));
}

}

#endif /* JSON_ALLOCATOR_HPP */
