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
    using size_type = std::size_t;
    using value_type = char;
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

    allocator_type& m_allocator{Allocator::get_instance()};
    pointer m_data{nullptr};
    size_type m_size{0};
};

inline
String::String(const String& other) noexcept {
    insert(0, other);
}

inline
String::String(const String& other, allocator_type& alloc) noexcept :
    m_allocator{alloc}
{
    insert(0, other);
}

inline
String::String(String&& other) noexcept {
    assign(std::move(other));
}

inline
String::String(String&& other, allocator_type& alloc) noexcept :
    m_allocator{alloc}
{
    assign(std::move(other));
}

inline
String::String(allocator_type& alloc) noexcept :
    m_allocator{alloc}
{ }

inline
String::String(const String& other, size_type pos, size_type count,
        allocator_type& alloc) noexcept :
    m_allocator{alloc}
{
    insert(0, other, pos, count);
}

inline
String::String(size_type count, value_type ch, allocator_type& alloc) noexcept :
    m_allocator{alloc}
{
    insert(size_type(0), count, ch);
}

inline
String::String(const_pointer s, allocator_type& alloc) noexcept :
    m_allocator{alloc}
{
    insert(0, s);
}

inline
String::String(const_pointer s, size_type count,
        allocator_type& alloc) noexcept :
    m_allocator{alloc}
{
    insert(0, s, count);
}

template<typename InputIt>
String::String(InputIt first, InputIt last, allocator_type& alloc) noexcept :
    m_allocator{alloc}
{
    assign(first, last);
}

inline
String::String(std::initializer_list<value_type> ilist,
        allocator_type& alloc) noexcept :
    m_allocator{alloc}
{
    assign(ilist);
}

inline
String::~String() noexcept {
    allocator().deallocate(data());
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
String::assign(const_pointer s, std::size_t count) noexcept -> String& {
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
String::insert(size_type index, const_pointer s) noexcept -> String& {
    insert(index, {s, length(s)}, copy_n);
    return *this;
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
    return m_allocator;
}

inline auto
String::allocator() const noexcept -> const allocator_type& {
    return m_allocator;
}

inline
String::operator StringView() const noexcept {
    return StringView{m_data, m_size};
}

}

#endif /* JSON_STRING_HPP */
