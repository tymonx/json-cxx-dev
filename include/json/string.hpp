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
 * @file json/string.hpp
 *
 * @brief Interface
 */

#ifndef JSON_STRING_HPP
#define JSON_STRING_HPP

#include "json/allocator.hpp"

#include <limits>
#include <cstdint>
#include <cstddef>
#include <utility>
#include <initializer_list>

namespace json {

class String {
public:
    using size_type = std::size_t;
    using value_type = std::uint8_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using allocator_type = Allocator;

    static constexpr size_type npos = std::numeric_limits<size_type>::max();

    String() noexcept = default;

    String(allocator_type* alloc) noexcept;

    String(const String& other) noexcept;

    String(const String& other, allocator_type* alloc) noexcept;

    String(String&& other) noexcept;

    String(String&& other, allocator_type* alloc) noexcept;

    String(size_type count, value_type ch,
            allocator_type* alloc = Allocator::get_default()) noexcept;

    String(const_pointer s,
            allocator_type* alloc = Allocator::get_default()) noexcept;

    String(const_pointer s, size_type count,
            allocator_type* alloc = Allocator::get_default()) noexcept;

    String(const String& other, size_type pos, size_type count = npos,
            allocator_type* alloc = Allocator::get_default()) noexcept;

    String(std::initializer_list<value_type> init,
            allocator_type* alloc = Allocator::get_default()) noexcept;

    void shrink_to_fit() noexcept;

    void reserve(size_type new_capacity) noexcept;

    size_type capacity() const noexcept;

    bool empty() const noexcept;

    size_type size() const noexcept;

    size_type length() const noexcept;

    pointer data() noexcept;

    const_pointer data() const noexcept;

    static size_type length(const_pointer s) noexcept;

    void clear() noexcept;

    void resize(size_type count) noexcept;

    void resize(size_type count, value_type ch) noexcept;

    allocator_type* allocator() noexcept;

    size_type copy(pointer dest, size_type count,
            size_type pos = 0) const noexcept;

    void pop_back() noexcept;

    void push_back(value_type ch) noexcept;

    const_pointer c_str() noexcept;

    ~String() noexcept;
private:
    allocator_type* m_allocator{Allocator::get_default()};
    pointer m_data{nullptr};
    size_type m_size{0};
};

inline
String::String(const String& other) noexcept :
    String{other, other.m_allocator}
{ }

inline
String::String(const String& other, allocator_type* alloc) noexcept :
    String{other.data(), other.size(), alloc}
{ }

inline
String::String(String&& other) noexcept :
    String{std::move(other), other.m_allocator}
{ }

inline
String::String(allocator_type* alloc) noexcept :
    m_allocator{alloc}
{ }

inline
String::String(const_pointer s, allocator_type* alloc) noexcept :
    String{s, length(s), alloc}
{ }

inline
String::String(std::initializer_list<value_type> init,
        allocator_type* alloc) noexcept :
    String{init.begin(), init.size(), alloc}
{ }

inline
String::~String() noexcept {
    if (m_allocator) {
        m_allocator->deallocate(m_data);
    }
}

inline auto
String::empty() const noexcept -> bool {
    return (0 == m_size);
}

inline auto
String::size() const noexcept -> size_type {
    return m_size;
}

inline auto
String::length() const noexcept -> size_type {
    return m_size;
}

inline auto
String::data() noexcept -> pointer {
    return m_data;
}

inline auto
String::data() const noexcept -> const_pointer {
    return m_data;
}

inline void
String::pop_back() noexcept {
    if (m_size) { --m_size; }
}

inline auto
String::capacity() const noexcept -> size_type {
    return m_allocator ? m_allocator->size(m_data) : 0;
}

inline void
String::clear() noexcept {
    if (m_allocator) {
        m_allocator->deallocate(m_data);
    }
    m_size = 0;
}

inline auto
String::allocator() noexcept -> allocator_type* {
    return m_allocator;
}

}

#endif /* JSON_STRING_HPP */
