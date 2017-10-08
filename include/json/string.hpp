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

#include "types.hpp"
#include "allocator.hpp"
#include "string_view.hpp"

#include <limits>
#include <cstdint>
#include <cstddef>
#include <utility>
#include <iterator>
#include <algorithm>
#include <initializer_list>

namespace json {

class String {
public:
    using size_type = Size;
    using value_type = Char;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using allocator_type = Allocator;
    using iterator = pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    static constexpr size_type npos = std::numeric_limits<size_type>::max();

    String() noexcept = default;

    String(allocator_type& alloc) noexcept;

    String(const String& other) noexcept;

    String(const String& other, allocator_type& alloc) noexcept;

    String(String&& other) noexcept;

    String(String&& other, allocator_type& alloc) noexcept;

    String(size_type count, value_type ch,
            allocator_type& alloc = Allocator::get_instance()) noexcept;

    String(const_pointer s,
            allocator_type& alloc = Allocator::get_instance()) noexcept;

    String(const_pointer s, size_type count,
            allocator_type& alloc = Allocator::get_instance()) noexcept;

    template<typename InputIt>
    String(InputIt first, InputIt last,
            allocator_type& alloc = Allocator::get_instance()) noexcept;

    String(const String& other, size_type pos, size_type count = npos,
            allocator_type& alloc = Allocator::get_instance()) noexcept;

    String(std::initializer_list<value_type> ilist,
            allocator_type& alloc = Allocator::get_instance()) noexcept;

    String& operator=(const String& other) noexcept;

    String& operator=(String&& other) noexcept;

    String& operator=(const_pointer s) noexcept;

    String& operator=(value_type ch) noexcept;

    String& operator=(std::initializer_list<value_type> ilist) noexcept;

    String& assign(size_type count, value_type ch) noexcept;

    String& assign(const String& str) noexcept;

    String& assign(const String& str, size_type pos,
            size_type count = npos) noexcept;

    String& assign(String&& str) noexcept;

    String& assign(const_pointer s) noexcept;

    String& assign(const_pointer s, size_type count) noexcept;

    String& assign(std::initializer_list<value_type> ilist) noexcept;

    template<typename InputIt>
    String& assign(InputIt first, InputIt last) noexcept;

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

    template<typename InputIt>
    iterator insert(const_iterator pos, InputIt first, InputIt last) noexcept;

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
    using Function = void(*)(const_pointer src, size_type count,
            pointer dst);

    static void copy_n(const_pointer src, size_type count,
            pointer dst) noexcept;

    static void move_n(const_pointer src, size_type count,
        pointer dst) noexcept;

    static void fill_n(const_pointer src, size_type count,
            pointer dst) noexcept;

    pointer insert(size_type index, const StringView& str,
            Function function) noexcept;

    allocator_type* m_allocator{&Allocator::get_instance()};
    pointer m_data{nullptr};
    size_type m_size{0};
};

inline
String::String(const String& other) noexcept :
    String{other, const_cast<String&>(other).allocator()}
{ }

inline
String::String(const String& other, allocator_type& alloc) noexcept :
    m_allocator{&alloc}
{
    insert(0, other);
}

inline
String::String(String&& other, allocator_type& alloc) noexcept :
    m_allocator{&alloc}
{
    assign(std::move(other));
}

inline
String::String(allocator_type& alloc) noexcept :
    m_allocator{&alloc}
{ }

inline
String::String(const String& other, size_type pos, size_type count,
        allocator_type& alloc) noexcept :
    m_allocator{&alloc}
{
    insert(0, other, pos, count);
}

inline
String::String(size_type count, value_type ch, allocator_type& alloc) noexcept :
    m_allocator{&alloc}
{
    insert(size_type(0), count, ch);
}

inline
String::String(const_pointer s, allocator_type& alloc) noexcept :
    m_allocator{&alloc}
{
    insert(0, s);
}

inline
String::String(const_pointer s, size_type count,
        allocator_type& alloc) noexcept :
    m_allocator{&alloc}
{
    insert(0, s, count);
}

template<typename InputIt>
String::String(InputIt first, InputIt last, allocator_type& alloc) noexcept :
    m_allocator{&alloc}
{
    assign(first, last);
}

inline auto
String::operator=(const String& other) noexcept -> String& {
    return assign(other);
}


inline auto
String::operator=(String&& other) noexcept -> String& {
    return assign(other);
}

inline auto
String::operator=(const_pointer s) noexcept -> String& {
    return assign(s);
}

inline auto
String::operator=(value_type ch) noexcept -> String& {
    return assign(1, ch);
}

inline auto
String::operator=(
        std::initializer_list<value_type> ilist) noexcept -> String& {
    return assign(ilist);
}

inline auto
String::assign(const String& other) noexcept -> String& {
    return assign(other.data(), other.size());
}

inline auto
String::assign(const_pointer s) noexcept -> String& {
    return assign(s, length(s));
}

inline auto
String::assign(size_type count, value_type ch) noexcept -> String& {
    clear();
    insert(0, StringView{&ch, count}, fill_n);
    return *this;
}

inline auto
String::assign(const String& other, size_type pos,
        size_type count) noexcept -> String& {
    clear();
    insert(0, StringView{other}.subspan(pos, count), copy_n);
    return *this;
}

inline auto
String::assign(const_pointer s, size_type count) noexcept -> String& {
    clear();
    insert(0, StringView{s, count}, copy_n);
    return *this;
}

inline auto
String::assign(
        std::initializer_list<value_type> ilist) noexcept -> String& {
    return assign(ilist.begin(), ilist.size());
}

template<> inline auto
String::assign<String::const_pointer>(const_pointer first,
        const_pointer last) noexcept -> String& {
    clear();
    insert(0, StringView{first, last}, copy_n);
    return *this;
}

template<typename InputIt> auto
String::assign(InputIt first, InputIt last) noexcept -> String& {
    resize(std::distance(first, last));
    std::copy_n(first, size(), data());
    return *this;
}

inline auto
String::at(size_type pos) noexcept -> reference {
    return m_data[pos];
}

inline auto
String::at(size_type pos) const noexcept -> const_reference {
    return m_data[pos];
}

inline auto
String::operator[](size_type pos) noexcept -> reference {
    return m_data[pos];
}

inline auto
String::operator[](size_type pos) const noexcept -> const_reference {
    return m_data[pos];
}

inline auto
String::front() noexcept -> reference {
    return m_data[0];
}

inline auto
String::front() const noexcept -> const_reference {
    return m_data[0];
}

inline auto
String::back() noexcept -> reference {
    return m_data[size() - 1];
}

inline auto
String::back() const noexcept -> const_reference {
    return m_data[size() - 1];
}

inline auto
String::data() noexcept -> pointer {
    return m_data;
}

inline auto
String::data() const noexcept -> const_pointer {
    return m_data;
}

inline auto
String::begin() noexcept -> iterator {
    return m_data;
}

inline auto
String::begin() const noexcept -> const_iterator {
    return m_data;
}

inline auto
String::cbegin() const noexcept -> const_iterator {
    return m_data;
}

inline auto
String::end() noexcept -> iterator {
    return m_data + m_size;
}

inline auto
String::end() const noexcept -> const_iterator {
    return m_data + m_size;
}

inline auto
String::cend() const noexcept -> const_iterator {
    return m_data + m_size;
}

inline auto
String::rbegin() noexcept -> reverse_iterator {
    return reverse_iterator(end() - 1);
}

inline auto
String::rbegin() const noexcept -> const_reverse_iterator {
    return const_reverse_iterator(cend() - 1);
}

inline auto
String::crbegin() const noexcept -> const_reverse_iterator {
    return const_reverse_iterator(cend() - 1);
}

inline auto
String::rend() noexcept -> reverse_iterator {
    return reverse_iterator(begin() - 1);
}

inline auto
String::rend() const noexcept -> const_reverse_iterator {
    return const_reverse_iterator(cbegin() - 1);
}

inline auto
String::crend() const noexcept -> const_reverse_iterator {
    return const_reverse_iterator(cbegin() - 1);
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

inline void
String::pop_back() noexcept {
    if (!empty()) {
        --m_size;
    }
}

inline void
String::clear() noexcept {
    m_size = 0;
}

inline auto
String::insert(size_type index, const String& str) noexcept -> String& {
    return insert(index, str.data(), str.size());
}

inline auto
String::insert(const_iterator pos, value_type ch) noexcept -> iterator {
    return insert(pos, 1, ch);
}

inline auto
String::insert(const_iterator pos,
        std::initializer_list<value_type> ilist) noexcept -> String& {
    return insert(size_type(pos - cbegin()), ilist.begin(), ilist.size());
}

inline auto
String::insert(size_type index, size_type count,
        value_type ch) noexcept -> String& {
    insert(index, StringView{&ch, count}, fill_n);
    return *this;
}

inline auto
String::insert(size_type index, const_pointer s,
        size_type count) noexcept -> String& {
    insert(index, StringView{s, count}, copy_n);
    return *this;
}

inline auto
String::insert(size_type index, const String& str,
        size_type index_str, size_type count) noexcept -> String& {
    insert(index, StringView{str}.subspan(index_str, count), copy_n);
    return *this;
}

template<> inline
String::iterator String::insert(const_iterator pos,
        const_iterator first, const_iterator last) noexcept {
    return insert(size_type(pos - cbegin()), StringView{first, last}, copy_n);
}

inline auto
String::insert(const_iterator pos, size_type count,
        value_type ch) noexcept -> iterator {
    return insert(size_type(pos - cbegin()), StringView{&ch, count}, fill_n);
}

inline auto
String::erase(const_iterator position) noexcept -> iterator {
    return erase(position, position + 1);
}

inline auto
String::allocator() noexcept -> allocator_type& {
    return *m_allocator;
}

inline auto
String::allocator() const noexcept -> const allocator_type& {
    return *m_allocator;
}

inline
String::operator StringView() const noexcept {
    return StringView{m_data, m_size};
}

}

#endif /* JSON_STRING_HPP */
