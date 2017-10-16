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
 * @file json/string.hpp
 *
 * @brief Interface
 */

#ifndef JSON_STRING_HPP
#define JSON_STRING_HPP

#include "string_base.hpp"
#include "string_iterator.hpp"
#include "string_reverse_iterator.hpp"

#include <iterator>
#include <initializer_list>

namespace json {

class Allocator;
class StringView;

class String {
public:
    using size_type = Size;
    using value_type = Char;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using allocator_type = Allocator;
    using iterator = StringIterator;
    using reverse_iterator = StringReverseIterator;

    static const size_type npos;

    String() noexcept;

    String(allocator_type& alloc) noexcept;

    String(const String& other) noexcept;

    String(const String& other, allocator_type& alloc) noexcept;

    String(String&& other) noexcept;

    String(String&& other, allocator_type& alloc) noexcept;

    String(size_type count, value_type ch) noexcept;

    String(size_type count, value_type ch, allocator_type& alloc) noexcept;

    String(const_pointer s) noexcept;

    String(const_pointer s, allocator_type& alloc) noexcept;

    String(const_pointer s, size_type count) noexcept;

    String(const_pointer s, size_type count, allocator_type& alloc) noexcept;

    String(iterator first, iterator last) noexcept;

    String(iterator first, iterator last, allocator_type& alloc) noexcept;

    String(const String& other, size_type pos, size_type count = npos) noexcept;

    String(const String& other, size_type pos, size_type count,
            allocator_type& alloc) noexcept;

    String(std::initializer_list<value_type> ilist) noexcept;

    String(std::initializer_list<value_type> ilist,
            allocator_type& alloc) noexcept;

    String(const StringView& other) noexcept;

    String(const StringView& other, allocator_type& alloc) noexcept;

    String& operator=(const String& other) noexcept;

    String& operator=(String&& other) noexcept;

    String& operator=(const_pointer s) noexcept;

    String& operator=(value_type ch) noexcept;

    String& operator=(std::initializer_list<value_type> ilist) noexcept;

    String& operator=(const StringView& other) noexcept;

    String& assign(size_type count, value_type ch) noexcept;

    String& assign(const String& other) noexcept;

    String& assign(const String& other, size_type pos,
            size_type count = npos) noexcept;

    String& assign(String&& other) noexcept;

    String& assign(const_pointer s) noexcept;

    String& assign(const_pointer s, size_type count) noexcept;

    String& assign(std::initializer_list<value_type> ilist) noexcept;

    String& assign(iterator first, iterator last) noexcept;

    String& assign(const StringView& other) noexcept;

    reference at(size_type pos) noexcept;

    const_reference at(size_type pos) const noexcept;

    reference operator[](size_type pos) noexcept;

    const_reference operator[](size_type pos) const noexcept;

    reference front() noexcept;

    const_reference front() const noexcept;

    reference back() noexcept;

    const_reference back() const noexcept;

    pointer data() noexcept;

    const_pointer data() const noexcept;

    const_pointer c_str() noexcept;

    iterator begin() noexcept;

    iterator begin() const noexcept;

    iterator cbegin() const noexcept;

    iterator end() noexcept;

    iterator end() const noexcept;

    iterator cend() const noexcept;

    reverse_iterator rbegin() noexcept;

    reverse_iterator rbegin() const noexcept;

    reverse_iterator crbegin() const noexcept;

    reverse_iterator rend() noexcept;

    reverse_iterator rend() const noexcept;

    reverse_iterator crend() const noexcept;

    void shrink_to_fit() noexcept;

    void reserve(size_type new_capacity) noexcept;

    size_type capacity() const noexcept;

    bool empty() const noexcept;

    size_type size() const noexcept;

    size_type length() const noexcept;

    static size_type length(const_pointer s) noexcept;

    void clear() noexcept;

    String& insert(size_type index, size_type count, value_type ch) noexcept;

    String& insert(size_type index, const_pointer s) noexcept;

    String& insert(size_type index, const_pointer s, size_type count) noexcept;

    String& insert(size_type index, const String& str) noexcept;

    String& insert(size_type index, const String& str,
            size_type index_str, size_type count = npos) noexcept;

    iterator insert(const_iterator pos, size_type count,
            value_type ch) noexcept;

    String& insert(const_iterator pos,
            std::initializer_list<value_type> ilist) noexcept;

    iterator insert(const_iterator pos, value_type ch) noexcept;

    iterator insert(const_iterator pos, const_iterator first,
            const_iterator last) noexcept;

    String& erase(size_type index = 0, size_type count = npos) noexcept;

    iterator erase(const_iterator position) noexcept;

    iterator erase(const_iterator first, const_iterator last) noexcept;

    void resize(size_type count) noexcept;

    void resize(size_type count, value_type ch) noexcept;

    allocator_type& allocator() noexcept;

    const allocator_type& allocator() const noexcept;

    size_type copy(pointer dest, size_type count,
            size_type pos = 0) const noexcept;

    void pop_back() noexcept;

    void push_back(value_type ch) noexcept;

    operator StringView() const noexcept;

    ~String() noexcept;
private:
    StringBase m_base;
    allocator_type* m_allocator;
};

}

#endif /* JSON_STRING_HPP */
