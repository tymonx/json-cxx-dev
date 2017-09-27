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
 *   span of conditions and the following disclaimer.
 *
 * @copright
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this span of conditions and the following disclaimer in the documentation
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
 * @file json/array.hpp
 *
 * @brief JSON array interface
 */

#ifndef JSON_ARRAY_HPP
#define JSON_ARRAY_HPP

#include "list.hpp"
#include "types.hpp"
#include "allocator.hpp"
#include "array_iterator.hpp"

namespace json {

class Array {
public:
    using value_type = Value;
    using size_type = Size;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = ArrayIterator<false>;
    using const_iterator = ArrayIterator<true>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    void push_back(const value_type& value) noexcept;

    void push_back(value_type&& value) noexcept;

    void pop_back() noexcept;

    void clear() noexcept;

    size_type size() const noexcept;

    bool empty() const noexcept;

    reference back() noexcept;

    const_reference back() const noexcept;

    iterator begin() noexcept;

    Allocator& allocator() noexcept;

    const_iterator begin() const noexcept;

    const_iterator cbegin() const noexcept;

    iterator end() noexcept;

    const_iterator end() const noexcept;

    const_iterator cend() const noexcept;

    reverse_iterator rbegin() noexcept;

    const_reverse_iterator rbegin() const noexcept;

    const_reverse_iterator crbegin() const noexcept;

    reverse_iterator rend() noexcept;

    const_reverse_iterator rend() const noexcept;

    const_reverse_iterator crend() const noexcept;

    ~Array() noexcept;
private:
    Allocator* m_allocator{&Allocator::get_instance()};
    List m_list{};
};

inline auto
Array::back() noexcept -> reference {
    return *iterator{&m_list.back()};
}

inline auto
Array::back() const noexcept -> const_reference {
    return *const_iterator{&m_list.back()};
}

inline auto
Array::allocator() noexcept -> Allocator& {
    return *m_allocator;
}

inline auto
Array::size() const noexcept -> size_type {
    return m_list.size();
}

inline auto
Array::empty() const noexcept -> bool {
    return m_list.empty();
}

inline auto
Array::begin() noexcept -> iterator {
    return m_list.begin();
}

inline auto
Array::begin() const noexcept -> const_iterator {
    return m_list.begin();
}

inline auto
Array::cbegin() const noexcept -> const_iterator {
    return m_list.cbegin();
}

inline auto
Array::end() noexcept -> iterator {
    return m_list.end();
}

inline auto
Array::end() const noexcept -> const_iterator {
    return m_list.end();
}

inline auto
Array::cend() const noexcept -> const_iterator {
    return m_list.cend();
}

inline auto
Array::rbegin() noexcept -> reverse_iterator {
    return m_list.rbegin();
}

inline auto
Array::rbegin() const noexcept -> const_reverse_iterator {
    return m_list.rbegin();
}

inline auto
Array::crbegin() const noexcept -> const_reverse_iterator {
    return m_list.crbegin();
}

inline auto
Array::rend() noexcept -> reverse_iterator {
    return m_list.rend();
}

inline auto
Array::rend() const noexcept -> const_reverse_iterator {
    return m_list.rend();
}

inline auto
Array::crend() const noexcept -> const_reverse_iterator {
    return m_list.crend();
}

}

#endif /* JSON_ARRAY_HPP */
