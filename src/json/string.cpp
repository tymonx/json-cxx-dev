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
 * @file json/string.cpp
 *
 * @brief Implementation
 */

#include "json/string.hpp"
#include "json/string_view.hpp"
#include "json/allocator.hpp"

#include <limits>
#include <utility>
#include <algorithm>

using json::String;

static const String::value_type EMPTY_STRING[]{""};

static_assert(std::is_standard_layout<String>::value,
        "json::String is not a standard layout");

const json::Size String::npos = std::numeric_limits<json::Size>::max();

String::String() noexcept :
    String{Allocator::get_instance()}
{ }

String::String(allocator_type& alloc) noexcept :
    m_size{0},
    m_data{nullptr},
    m_allocator{&alloc}
{ }

String::String(const String& other) noexcept :
    String{other, const_cast<String&>(other).allocator()}
{ }

String::String(const String& other, allocator_type& alloc) noexcept :
    String{other.data(), other.size(), alloc}
{ }

String::String(String&& other) noexcept :
    m_size{other.size()},
    m_data{other.data()},
    m_allocator{&other.allocator()}
{
    other.m_size = 0;
    other.m_data = nullptr;
}

String::String(String&& other, allocator_type& alloc) noexcept :
    m_size{other.size()},
    m_data{other.data()},
    m_allocator{&other.allocator()}
{
    if (&other.allocator() == &alloc) {
        other.m_size = 0;
        other.m_data = nullptr;
    }
    else {
        m_data = allocator().allocate<value_type>(other.size());
        if (data()) {
            std::copy_n(other.data(), other.size(), data());
        }
        else {
            m_size = 0;
        }
    }
}

String::String(size_type count, value_type ch) noexcept :
    String{count, ch, Allocator::get_instance()}
{ }

String::String(size_type count, value_type ch, allocator_type& alloc) noexcept :
    m_size{count},
    m_data{nullptr},
    m_allocator{&alloc}
{
    m_data = allocator().allocate<value_type>(count);
    if (data()) {
        std::fill_n(data(), count, ch);
    }
    else {
        m_size = 0;
    }
}

String::String(const_pointer s) noexcept :
    String{s, Allocator::get_instance()}
{ }

String::String(const_pointer s, allocator_type& alloc) noexcept :
    String{s, length(s), alloc}
{ }

String::String(const_pointer s, size_type count) noexcept :
    String{s, count, Allocator::get_instance()}
{ }

String::String(const_pointer s, size_type count,
        allocator_type& alloc) noexcept :
    String{StringView{s, count}, alloc}
{ }

String::String(const_iterator first, const_iterator last) noexcept :
    String{first, last, Allocator::get_instance()}
{ }

String::String(const_iterator first, const_iterator last,
        allocator_type& alloc) noexcept :
    String{StringView{first, last}, alloc}
{ }

String::String(const String& other, size_type pos, size_type count) noexcept :
    String{other, pos, count, const_cast<String&>(other).allocator()}
{ }

String::String(const String& other, size_type pos, size_type count,
        allocator_type& alloc) noexcept :
    String{StringView{other}.subspan(pos, count), alloc}
{ }

String::String(std::initializer_list<value_type> ilist) noexcept :
    String{ilist, Allocator::get_instance()}
{ }

String::String(std::initializer_list<value_type> ilist,
        allocator_type& alloc) noexcept :
    String{ilist.begin(), ilist.size(), alloc}
{ }

String::String(const StringView& other) noexcept :
    String{other, Allocator::get_instance()}
{ }

String::String(const StringView& other, allocator_type& alloc) noexcept :
    m_size{other.size()},
    m_data{nullptr},
    m_allocator{&alloc}
{
    m_data = allocator().allocate<value_type>(other.size());
    if (data()) {
        std::copy_n(other.data(), other.size(), data());
    }
    else {
        m_size = 0;
    }
}

auto String::operator=(const String& other) noexcept -> String& {
    return assign(other);
}

auto String::operator=(String&& other) noexcept -> String& {
    return assign(std::move(other));
}

auto String::operator=(const_pointer s) noexcept -> String& {
    return assign(s);
}

auto String::operator=(value_type ch) noexcept -> String& {
    return assign(1, ch);
}

auto String::operator=(
        std::initializer_list<value_type> ilist) noexcept -> String& {
    return assign(ilist);
}

auto String::operator=(const StringView& other) noexcept -> String& {
    return assign(other);
}

auto String::assign(size_type count, value_type ch) noexcept -> String& {
    resize(count);
    std::fill_n(data(), size(), ch);
    return *this;
}

auto String::assign(const String& other) noexcept -> String& {
    return assign(StringView{other});
}

auto String::assign(const String& other, size_type pos,
        size_type count) noexcept -> String& {
    return assign(StringView{other}.subspan(pos, count));
}

auto String::assign(String&& other) noexcept -> String& {
    if (&other != this) {
        if (&other.allocator() == &allocator()) {
            m_size = other.size();
            m_data = other.data();

            other.m_size = 0;
            other.m_data = nullptr;
        }
        else {
            assign(static_cast<const String&>(other));
        }
    }
    return *this;
}

auto String::assign(const_pointer s) noexcept -> String& {
    return assign(s, length(s));
}

auto String::assign(const_pointer s, size_type count) noexcept -> String& {
    return assign(StringView{s, count});
}

auto String::assign(
        std::initializer_list<value_type> ilist) noexcept -> String& {
    return assign(ilist.begin(), ilist.size());
}

auto String::assign(const_iterator first,
        const_iterator last) noexcept -> String& {
    return assign(StringView{first, last});
}

auto String::at(size_type pos) noexcept -> reference {
    return *(data() + pos);
}

auto String::at(size_type pos) const noexcept -> const_reference {
    return *(data() + pos);
}

auto String::operator[](size_type pos) noexcept -> reference {
    return *(data() + pos);
}

auto String::operator[](size_type pos) const noexcept -> const_reference {
    return *(data() + pos);
}

auto String::front() noexcept -> reference {
    return *data();
}

auto String::front() const noexcept -> const_reference {
    return *data();
}

auto String::back() noexcept -> reference {
    return *(data() + size() - 1);
}

auto String::back() const noexcept -> const_reference {
    return *(data() + size() - 1);
}

auto String::data() noexcept -> pointer {
    return m_data;
}

auto String::data() const noexcept -> const_pointer {
    return m_data;
}

auto String::c_str() noexcept -> const_pointer {
    String::const_pointer str = data();

    if (str[size()] != '\0') {
        if (size() < capacity()) {
            const_cast<pointer>(str)[size()] = '\0';
        }
        else {
            str = allocator().reallocate<value_type>(data(), size() + 1);
            if (str) {
                m_data = const_cast<pointer>(str);
                m_data[m_size++] = '\0';
            }
            else {
                str = EMPTY_STRING;
            }
        }
    }

    return str;
}

auto String::begin() noexcept -> iterator {
    return data();
}

auto String::begin() const noexcept -> const_iterator {
    return data();
}

auto String::cbegin() const noexcept -> const_iterator {
    return data();
}

auto String::end() noexcept -> iterator {
    return data() + size();
}

auto String::end() const noexcept -> const_iterator {
    return data() + size();
}

auto String::cend() const noexcept -> const_iterator {
    return data() + size();
}

auto String::rbegin() noexcept -> reverse_iterator {
    return reverse_iterator{data() + size() - 1};
}

auto String::rbegin() const noexcept -> const_reverse_iterator {
    return const_reverse_iterator{data() + size() - 1};
}

auto String::crbegin() const noexcept -> const_reverse_iterator {
    return const_reverse_iterator{data() + size() - 1};
}

auto String::rend() noexcept -> reverse_iterator {
    return reverse_iterator{data() - 1};
}

auto String::rend() const noexcept -> const_reverse_iterator {
    return const_reverse_iterator{data() - 1};
}

auto String::crend() const noexcept -> const_reverse_iterator {
    return const_reverse_iterator{data() - 1};
}

void String::shrink_to_fit() noexcept {
    if (capacity() > size()) {
        auto ptr = allocator().reallocate(data(), size());
        if (ptr) {
            m_data = ptr;
        }
    }
}

void String::reserve(size_type new_capacity) noexcept {
    if (capacity() < new_capacity) {
        auto ptr = allocator().reallocate(data(), new_capacity);
        if (ptr) {
            m_data = ptr;
        }
    }
}

auto String::capacity() const noexcept -> size_type {
    return allocator().size(data());
}

auto String::empty() const noexcept -> bool {
    return (0 == size());
}

auto String::size() const noexcept -> size_type {
    return m_size;
}

auto String::length() const noexcept -> size_type {
    return m_size;
}

auto String::length(const_pointer s) noexcept -> size_type {
    size_type count = 0;

    if (s) {
        while (*s && (count < npos)) {
            ++s;
            ++count;
        }
    }

    return count;
}

void String::clear() noexcept {
    m_size = 0;
}

String::~String() noexcept {
    allocator().deallocate(data());
}

#if 0

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

void String::resize(size_type count) noexcept {

}

void resize(size_type count, value_type ch) noexcept;

allocator_type& allocator() noexcept;

const allocator_type& allocator() const noexcept;

size_type copy(pointer dest, size_type count,
        size_type pos = 0) const noexcept;

void pop_back() noexcept;

void push_back(value_type ch) noexcept;

operator StringView() const noexcept;

~String() noexcept;


String::String(const String& other) noexcept :
    String{other, const_cast<String&>(other).allocator()}
{ }

String::String(const String& other, allocator_type& alloc) noexcept :
    m_allocator{&alloc}
{
    insert(0, other);
}

String::String(String&& other, allocator_type& alloc) noexcept :
    m_allocator{&alloc}
{
    assign(std::move(other));
}

String::String(allocator_type& alloc) noexcept :
    m_allocator{&alloc}
{ }

String::String(const String& other, size_type pos, size_type count,
        allocator_type& alloc) noexcept :
    m_allocator{&alloc}
{
    insert(0, other, pos, count);
}

String::String(size_type count, value_type ch, allocator_type& alloc) noexcept :
    m_allocator{&alloc}
{
    insert(size_type(0), count, ch);
}

String::String(const_pointer s, allocator_type& alloc) noexcept :
    m_allocator{&alloc}
{
    insert(0, s);
}

String::String(const_pointer s, size_type count,
        allocator_type& alloc) noexcept :
    m_allocator{&alloc}
{
    insert(0, s, count);
}

String::String(String&& other) noexcept {
    assign(std::move(other));
}

String::String(std::initializer_list<value_type> ilist,
        allocator_type& alloc) noexcept :
    m_allocator{&alloc}
{
    insert(0, {ilist.begin(), ilist.size()}, copy_n);
}

String::~String() noexcept {
    allocator().deallocate(data());
}

String::pointer String::insert(size_type index, const StringView& str,
        Function function) noexcept {
    if (index <= size()) {
        auto total_size = size() + str.size();
        resize(total_size);
        if (size() == total_size) {
            auto pos = data() + index;
            move_n(pos, str.size(), pos + index);
            function(str.data(), str.size(), pos);
        }
    }
    else {
        index = size();
    }
    return data() + index;
}

String& String::assign(String&& other) noexcept {
    if (&other != this) {
        if (&other.allocator() == &allocator()) {
            allocator().deallocate(data());

            m_data = other.data();
            m_size = other.size();
            other.m_data = nullptr;
            other.m_size = 0;
        }
        else {
            assign(std::cref(other));
        }
    }

    return *this;
}

String::size_type String::capacity() const noexcept {
    return allocator().size(data());
}

void String::shrink_to_fit() noexcept {
    if (capacity() > size()) {
        auto ptr = allocator().reallocate(data(), size());
        if (ptr) {
            m_data = ptr;
        }
    }
}

void String::reserve(size_type new_capacity) noexcept {
    if (capacity() < new_capacity) {
        auto ptr = allocator().reallocate(data(), new_capacity);
        if (ptr) {
            m_data = ptr;
        }
    }
}

void String::resize(size_type count) noexcept {
    if (capacity() < count) {
        auto ptr = allocator().reallocate(data(), count);
        if (ptr) {
            m_data = ptr;
            m_size = count;
        }
    }
    else {
        m_size = count;
    }
}

void String::resize(size_type count, value_type ch) noexcept {
    auto offset = m_size;

    resize(count);

    if (offset < size()) {
        fill_n(&ch, size() - offset, data() + offset);
    }
}

String::size_type String::copy(pointer dest, size_type count,
        size_type pos) const noexcept {
    if (dest) {
        auto str = StringView{*this}.subspan(pos, count);
        copy_n(str.data(), str.size(), dest);
    }
    else {
        count = 0;
    }
    return count;
}

void String::push_back(value_type ch) noexcept {
    if (size() < capacity()) {
        m_data[m_size++] = ch;
    }
    else {
        auto ptr = allocator().allocate<value_type>(size() + 1);
        if (ptr) {
            m_data = ptr;
            m_data[m_size++] = ch;
        }
    }
}

String::const_pointer String::c_str() noexcept {
    String::const_pointer str = data();

    if (m_data[size()] != '\0') {
        if (size() < capacity()) {
            m_data[size()] = '\0';
        }
        else {
            str = allocator().reallocate<value_type>(data(), size() + 1);
            if (str) {
                m_data = const_cast<String::pointer>(str);
                m_data[size()] = '\0';
            }
            else {
                str = EMPTY_STRING;
            }
        }
    }

    return str;
}

auto String::erase(const_iterator first,
        const_iterator last) noexcept -> iterator {
    return {};
}

String& String::erase(size_type index, size_type count) noexcept {
    if (index <= size()) {
        auto total_size = size() - index;

        if (count > total_size) {
            count = total_size;
        }

        auto pos = data() + index;
        copy_n(pos + count, count, pos);

        m_size -= count;
    }
    return *this;
}

auto String::operator=(const String& other) noexcept -> String& {
    return assign(other);
}

auto String::operator=(String&& other) noexcept -> String& {
    return assign(other);
}

auto String::operator=(const_pointer s) noexcept -> String& {
    return assign(s);
}

auto String::operator=(value_type ch) noexcept -> String& {
    return assign(1, ch);
}

auto String::operator=(std::initializer_list<value_type> ilist) noexcept -> String& {
    return assign(ilist);
}

auto String::assign(const String& other) noexcept -> String& {
    return assign(other.data(), other.size());
}

auto String::assign(const_pointer s) noexcept -> String& {
    return assign(s, length(s));
}

auto String::assign(size_type count, value_type ch) noexcept -> String& {
    clear();
    insert(0, StringView{&ch, count}, fill_n);
    return *this;
}

auto String::assign(const String& other, size_type pos,
        size_type count) noexcept -> String& {
    clear();
    insert(0, StringView{other}.subspan(pos, count), copy_n);
    return *this;
}

auto String::assign(const_pointer s, size_type count) noexcept -> String& {
    clear();
    insert(0, StringView{s, count}, copy_n);
    return *this;
}

auto String::assign(std::initializer_list<value_type> ilist) noexcept ->
        String& {
    return assign(ilist.begin(), ilist.size());
}

auto String::assign(const_iterator first,
        const_iterator last) noexcept -> String& {
    clear();
    insert(0, StringView{first, last}, copy_n);
    return *this;
}

auto String::at(size_type pos) noexcept -> reference {
    return m_data[pos];
}

auto String::at(size_type pos) const noexcept -> const_reference {
    return m_data[pos];
}

auto String::operator[](size_type pos) noexcept -> reference {
    return m_data[pos];
}

auto String::operator[](size_type pos) const noexcept -> const_reference {
    return m_data[pos];
}

auto String::front() noexcept -> reference {
    return m_data[0];
}

auto String::front() const noexcept -> const_reference {
    return m_data[0];
}

auto String::back() noexcept -> reference {
    return m_data[size() - 1];
}

auto String::back() const noexcept -> const_reference {
    return m_data[size() - 1];
}

auto String::data() noexcept -> pointer {
    return m_data;
}

auto String::data() const noexcept -> const_pointer {
    return m_data;
}

auto String::begin() noexcept -> iterator {
    return m_data;
}

auto String::begin() const noexcept -> const_iterator {
    return m_data;
}

auto String::cbegin() const noexcept -> const_iterator {
    return m_data;
}

auto String::end() noexcept -> iterator {
    return m_data + m_size;
}

auto String::end() const noexcept -> const_iterator {
    return m_data + m_size;
}

auto String::cend() const noexcept -> const_iterator {
    return m_data + m_size;
}

auto String::rbegin() noexcept -> reverse_iterator {
    return reverse_iterator(end() - 1);
}

auto String::rbegin() const noexcept -> const_reverse_iterator {
    return const_reverse_iterator(cend() - 1);
}

auto String::crbegin() const noexcept -> const_reverse_iterator {
    return const_reverse_iterator(cend() - 1);
}

auto String::rend() noexcept -> reverse_iterator {
    return reverse_iterator(begin() - 1);
}

auto String::rend() const noexcept -> const_reverse_iterator {
    return const_reverse_iterator(cbegin() - 1);
}

auto String::crend() const noexcept -> const_reverse_iterator {
    return const_reverse_iterator(cbegin() - 1);
}

auto String::empty() const noexcept -> bool {
    return (0 == m_size);
}

auto String::size() const noexcept -> size_type {
    return m_size;
}

auto String::length() const noexcept -> size_type {
    return m_size;
}

void String::pop_back() noexcept {
    if (!empty()) {
        --m_size;
    }
}

void String::clear() noexcept {
    m_size = 0;
}

auto String::insert(size_type index, const String& str) noexcept -> String& {
    return insert(index, str.data(), str.size());
}

auto String::insert(const_iterator pos, value_type ch) noexcept -> iterator {
    return insert(pos, 1, ch);
}

auto String::insert(const_iterator pos,
        std::initializer_list<value_type> ilist) noexcept -> String& {
    return insert(size_type(pos - cbegin()), ilist.begin(), ilist.size());
}

auto String::insert(size_type index, size_type count,
        value_type ch) noexcept -> String& {
    insert(index, StringView{&ch, count}, fill_n);
    return *this;
}

auto String::insert(size_type index, const_pointer s,
        size_type count) noexcept -> String& {
    insert(index, StringView{s, count}, copy_n);
    return *this;
}

auto String::insert(size_type index, const String& str,
        size_type index_str, size_type count) noexcept -> String& {
    insert(index, StringView{str}.subspan(index_str, count), copy_n);
    return *this;
}

auto String::insert(const_iterator pos,
        const_iterator first, const_iterator last) noexcept -> iterator {
    return insert(size_type(pos - cbegin()), StringView{first, last}, copy_n);
}

auto String::insert(const_iterator pos, size_type count,
        value_type ch) noexcept -> iterator {
    return insert(size_type(pos - cbegin()), StringView{&ch, count}, fill_n);
}

auto String::erase(const_iterator position) noexcept -> iterator {
    return erase(position, position + 1);
}

auto String::allocator() noexcept -> allocator_type& {
    return *m_allocator;
}

auto String::allocator() const noexcept -> const allocator_type& {
    return *m_allocator;
}

String::operator StringView() const noexcept {
    return StringView{m_data, m_size};
}

String::size_type String::length(const_pointer s) noexcept {
    size_type count = 0;

    if (s) {
        while (*s && (count < npos)) {
            ++s;
            ++count;
        }
    }

    return count;
}

#endif
