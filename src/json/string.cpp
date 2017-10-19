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
#include <functional>

using json::Size;
using json::String;
using json::Unicode;

static_assert(std::is_standard_layout<String>::value,
        "json::String is not a standard layout");

const json::Size String::npos = std::numeric_limits<json::Size>::max();

using UtfEncode = void* (*)(char32_t unicode, void* dst);

static std::uint16_t utf16_swap(std::uint16_t ch) noexcept {
    return std::uint16_t(ch << 8) | std::uint16_t(ch >> 8);
}

static std::uint32_t utf32_swap(std::uint32_t ch) noexcept {
    return std::uint32_t(ch << 24) | std::uint32_t(ch >> 24) |
        (std::uint32_t(ch >> 8) & 0x0000FF00) |
        (std::uint32_t(ch << 8) & 0x00FF0000);
}

static void* utf8_encode(char32_t ch, void* dst) noexcept {
    auto utf8 = reinterpret_cast<std::uint8_t*>(dst);

    if (ch < 0x80) {
        *utf8++ = std::uint8_t(ch);
    }
    else if (ch < 0x800) {
        *utf8++ = 0xC0 | std::uint8_t(ch >> 6);
        *utf8++ = 0x80 | std::uint8_t(ch & 0x3F);
    }
    else if (ch < 0x10000) {
        *utf8++ = 0xE0 | std::uint8_t(ch >> 12);
        *utf8++ = 0x80 | std::uint8_t((ch >> 6) & 0x3F);
        *utf8++ = 0x80 | std::uint8_t(ch & 0x3F);
    }
    else {
        *utf8++ = 0xF0 | std::uint8_t(ch >> 18);
        *utf8++ = 0x80 | std::uint8_t((ch >> 12) & 0x3F);
        *utf8++ = 0x80 | std::uint8_t((ch >> 6) & 0x3F);
        *utf8++ = 0x80 | std::uint8_t(ch & 0x3F);
    }

    return utf8;
}

static void* utf16be_encode(char32_t ch, void* dst) noexcept {
    auto utf16 = reinterpret_cast<std::uint16_t*>(dst);

    if (ch < 0x100000) {
        *utf16++ = std::uint16_t(ch);
    }
    else {
        ch -= 0x100000;
        *utf16++ = 0xD800 + std::uint16_t(ch >> 10);
        *utf16++ = 0xDC00 + std::uint16_t(ch & 0x3FF);
    }

    return utf16;
}

static void* utf16le_encode(char32_t ch, void* dst) noexcept {
    auto utf16 = reinterpret_cast<std::uint16_t*>(dst);

    if (ch < 0x100000) {
        *utf16++ = utf16_swap(std::uint16_t(ch));
    }
    else {
        ch -= 0x100000;
        *utf16++ = utf16_swap(0xD800 + std::uint16_t(ch >> 10));
        *utf16++ = utf16_swap(0xDC00 + std::uint16_t(ch & 0x3FF));
    }

    return utf16;
}

static void* utf32be_encode(char32_t ch, void* dst) noexcept {
    auto utf32 = reinterpret_cast<std::uint32_t*>(dst);

    *utf32++ = std::uint32_t(ch);

    return utf32;
}

static void* utf32le_encode(char32_t ch, void* dst) noexcept {
    auto utf32 = reinterpret_cast<std::uint32_t*>(dst);

    *utf32++ = utf32_swap(std::uint32_t(ch));

    return utf32;
}

static UtfEncode utf_encode(Unicode encoding) noexcept {
    auto callback = utf8_encode;

    switch (encoding) {
    case Unicode::UTF16:
    case Unicode::UTF16BE:
        callback = utf16be_encode;
        break;
    case Unicode::UTF16LE:
        callback = utf16le_encode;
        break;
    case Unicode::UTF32:
    case Unicode::UTF32BE:
        callback = utf32be_encode;
        break;
    case Unicode::UTF32LE:
        callback = utf32le_encode;
        break;
    case Unicode::UTF8:
    default:
        break;
    }

    return callback;
}

static Size utf_bytes(Unicode encoding, char32_t ch) noexcept {
    Size bytes = 4;

    switch (encoding) {
    case Unicode::UTF8:
        if (ch < 0x80) {
            bytes = 1;
        }
        else if (ch < 0x800) {
            bytes = 2;
        }
        else if (ch < 0x100000) {
            bytes = 3;
        }
        break;
    case Unicode::UTF16:
    case Unicode::UTF16BE:
    case Unicode::UTF16LE:
        if ((ch < 0xD800) || (ch > 0xDFFF)) {
            bytes = 2;
        }
        break;
    case Unicode::UTF32:
    case Unicode::UTF32BE:
    case Unicode::UTF32LE:
    default:
        break;
    }

    return bytes;
}

static Unicode utf_encoding(Unicode encoding) noexcept {
    switch (encoding) {
    case Unicode::UTF16:
        encoding = Unicode::UTF16BE;
        break;
    case Unicode::UTF32:
        encoding = Unicode::UTF32BE;
        break;
    case Unicode::UTF16BE:
    case Unicode::UTF16LE:
    case Unicode::UTF32BE:
    case Unicode::UTF32LE:
    case Unicode::UTF8:
    default:
        break;
    }

    return encoding;
}

static Unicode utf_encoding(const String& other) noexcept {
    Unicode encoding = other.unicode();

    switch (encoding) {
    case Unicode::UTF16:
        if (!other.empty() && (0xFFFE == other.front())) {
            encoding = Unicode::UTF16LE;
        }
        else {
            encoding = Unicode::UTF16BE;
        }
        break;
    case Unicode::UTF32:
        if (!other.empty() && (0xFFFE0000 == other.front())) {
            encoding = Unicode::UTF32LE;
        }
        else {
            encoding = Unicode::UTF32BE;
        }
        break;
    case Unicode::UTF16BE:
    case Unicode::UTF16LE:
    case Unicode::UTF32BE:
    case Unicode::UTF32LE:
    case Unicode::UTF8:
    default:
        break;
    }

    return encoding;
}

static bool utf_length_equal(const String& other, Unicode encoding) noexcept {
    bool equal = false;

    switch (encoding) {
    case Unicode::UTF8:
        equal = (Unicode::UTF8 == other.unicode());
        break;
    case Unicode::UTF16:
    case Unicode::UTF16BE:
    case Unicode::UTF16LE:
        equal = (Unicode::UTF16 == other.unicode()) ||
            (Unicode::UTF16BE == other.unicode()) ||
            (Unicode::UTF16LE == other.unicode());
        break;
    case Unicode::UTF32:
    case Unicode::UTF32BE:
    case Unicode::UTF32LE:
        equal = (Unicode::UTF32 == other.unicode()) ||
            (Unicode::UTF32BE == other.unicode()) ||
            (Unicode::UTF32LE == other.unicode());
        break;
    default:
        break;
    }

    return equal;
}

static char32_t utf_valid(char32_t ch) noexcept {
    return (((ch < 0xD800) || (ch > 0xDFFF)) && (ch < 0x110000))
        ? ch : 0xFFFD;
}

static void* utf_fill(Unicode encoding, char32_t ch, Size count,
        void* dst) noexcept {
    auto encode = utf_encode(encoding);

    while (count--) {
        dst = encode(ch, dst);
    }

    return dst;
}

static Size utf_length(const String& other) noexcept {
    Size count = 0;

    auto it = other.cbegin();
    auto it_end = other.cend();

    while (it < it_end) {
        count += utf_bytes(other.unicode(), *it++);
    }

    return count;
}

static void utf_copy(const void* first, const void* last, void* dst) noexcept {
    std::copy(reinterpret_cast<const std::uint8_t*>(first),
            reinterpret_cast<const std::uint8_t*>(last),
            reinterpret_cast<std::uint8_t*>(dst));
}

static void utf_copy(Unicode encoding, String::iterator first,
        String::iterator last, void* dst) noexcept {
    encoding = utf_encoding(encoding);

    if (utf_encoding(first.unicode()) == encoding) {
        utf_copy(first.base(), last.base(), dst);
    }
    else {
        auto encode = utf_encode(encoding);

        while (first < last) {
            dst = encode(*first++, dst);
        }
    }
}

String::String() noexcept :
    String{Unicode::UTF8, Allocator::get_instance()}
{ }

String::String(Unicode encoding) noexcept :
    String{encoding, Allocator::get_instance()}
{ }

String::String(allocator_type& alloc) noexcept :
    String{Unicode::UTF8, alloc}
{ }

String::String(Unicode encoding, allocator_type& alloc) noexcept :
    m_base{encoding, nullptr, 0},
    m_allocator{&alloc}
{ }

String::String(const String& other) noexcept :
    String{other.unicode(), other, const_cast<String&>(other).allocator()}
{ }

String::String(Unicode encoding, const String& other) noexcept :
    String{encoding, other, const_cast<String&>(other).allocator()}
{ }

String::String(const String& other, allocator_type& alloc) noexcept :
    String{other.unicode(), other, alloc}
{ }

String::String(Unicode encoding, const String& other,
        allocator_type& alloc) noexcept :
    m_base{encoding, nullptr, other.m_base.size},
    m_allocator{&alloc}
{
    if (!utf_length_equal(other, encoding)) {
        m_base.size = utf_length(other);
    }

    m_base.data = allocator().allocate(other.m_base.size);
    if (m_base.data) {
        utf_copy(encoding, other.cbegin(), other.cend(), m_base.data);
    }
    else {
        m_base.size = 0;
    }
}

String::String(String&& other) noexcept :
    m_base{other.m_base},
    m_allocator{&other.allocator()}
{
    other.m_base.data = nullptr;
}

String::String(Unicode encoding, String&& other) noexcept :
    m_base{encoding, nullptr, 0},
    m_allocator{&other.allocator()}
{
    if (other.unicode() == encoding) {
        other.m_base.data = nullptr;
    }
    else {
        (*this) = std::cref(other);
    }
}

String::String(String&& other, allocator_type& alloc) noexcept :
    m_base{other.m_base},
    m_allocator{&other.allocator()}
{
    if (&other.allocator() == &alloc) {
        other.m_base.data = nullptr;
    }
    else {
        (*this) = std::cref(other);
    }
}

String::String(size_type count, value_type ch) noexcept :
    String{count, ch, Allocator::get_instance()}
{ }

String::String(size_type count, value_type ch, allocator_type& alloc) noexcept :
    m_base{Unicode::UTF8, nullptr, 0},
    m_allocator{&alloc}
{
    ch = utf_valid(ch);

    m_base.size = utf_bytes(Unicode::UTF8, ch) * count;
    m_base.data = allocator().allocate(m_base.size);
    if (m_base.data) {
        utf_fill(unicode(), ch, count, m_base.data);
    }
    else {
        m_base.size = 0;
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

String::String(iterator first, iterator last) noexcept :
    String{first, last, Allocator::get_instance()}
{ }

String::String(iterator first, iterator last,
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
    m_base{other.unicode(), nullptr, other.size()},
    m_allocator{&alloc}
{
    m_base = allocator().allocate<value_type>(other.size());
    if (data()) {
        //std::copy_n(other.data(), other.size(), data());
    }
    else {
        m_base = size_type(0);
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
            m_base = other.m_base;
            m_base = nullptr;
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

auto String::assign(iterator first,
        iterator last) noexcept -> String& {
    return assign(StringView{first, last});
}

auto String::unicode() const noexcept {
    return Unicode(m_base.unicode);
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
    return m_base;
}

auto String::data() const noexcept -> const_pointer {
    return m_base;
}

auto String::begin() noexcept -> iterator {
    return data();
}

auto String::begin() const noexcept -> iterator {
    return data();
}

auto String::cbegin() const noexcept -> iterator {
    return data();
}

auto String::end() noexcept -> iterator {
    return data() + size();
}

auto String::end() const noexcept -> iterator {
    return data() + size();
}

auto String::cend() const noexcept -> iterator {
    return data() + size();
}

auto String::rbegin() noexcept -> reverse_iterator {
    return reverse_iterator{data() + size() - 1};
}

auto String::rbegin() const noexcept -> reverse_iterator {
    return reverse_iterator{data() + size() - 1};
}

auto String::crbegin() const noexcept -> reverse_iterator {
    return reverse_iterator{data() + size() - 1};
}

auto String::rend() noexcept -> reverse_iterator {
    return reverse_iterator{data() - 1};
}

auto String::rend() const noexcept -> reverse_iterator {
    return reverse_iterator{data() - 1};
}

auto String::crend() const noexcept -> reverse_iterator {
    return reverse_iterator{data() - 1};
}

void String::shrink_to_fit() noexcept {
    if (capacity() > size()) {
        auto ptr = allocator().reallocate(data(), size());
        if (ptr) {
            m_base = ptr;
        }
    }
}

void String::reserve(size_type new_capacity) noexcept {
    if (capacity() < new_capacity) {
        auto ptr = allocator().reallocate(data(), new_capacity);
        if (ptr) {
            m_base = ptr;
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
    return m_base.size();
}

auto String::length() const noexcept -> size_type {
    return m_base.size();
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
    m_base = size_type(0);
}

auto String::insert(size_type index, size_type count,
        value_type ch) noexcept -> String& {
    auto old_size = size();
    auto new_size = old_size + count;
    resize(new_size);

    if (size() == new_size) {
        if (index > old_size) {
            index = old_size;
        }

        std::move_backward(data() + index, data() + old_size,
                data() + new_size);
        std::fill_n(data() + index, count, ch);
    }

    return *this;
}

#if 0

String& insert(size_type index, size_type count, value_type ch) noexcept;

String& insert(size_type index, const_pointer s) noexcept;

String& insert(size_type index, const_pointer s, size_type count) noexcept;

String& insert(size_type index, const String& str) noexcept;

String& insert(size_type index, const String& str,
        size_type index_str, size_type count = npos) noexcept;

iterator insert(iterator pos, size_type count,
        value_type ch) noexcept;

String& insert(iterator pos,
        std::initializer_list<value_type> ilist) noexcept;

iterator insert(iterator pos, value_type ch) noexcept;

iterator insert(iterator pos, iterator first,
        iterator last) noexcept;

String& erase(size_type index = 0, size_type count = npos) noexcept;

iterator erase(iterator position) noexcept;

iterator erase(iterator first, iterator last) noexcept;

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

auto String::erase(iterator first,
        iterator last) noexcept -> iterator {
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

auto String::assign(iterator first,
        iterator last) noexcept -> String& {
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

auto String::begin() const noexcept -> iterator {
    return m_data;
}

auto String::cbegin() const noexcept -> iterator {
    return m_data;
}

auto String::end() noexcept -> iterator {
    return m_data + m_size;
}

auto String::end() const noexcept -> iterator {
    return m_data + m_size;
}

auto String::cend() const noexcept -> iterator {
    return m_data + m_size;
}

auto String::rbegin() noexcept -> reverse_iterator {
    return reverse_iterator(end() - 1);
}

auto String::rbegin() const noexcept -> reverse_iterator {
    return reverse_iterator(cend() - 1);
}

auto String::crbegin() const noexcept -> reverse_iterator {
    return reverse_iterator(cend() - 1);
}

auto String::rend() noexcept -> reverse_iterator {
    return reverse_iterator(begin() - 1);
}

auto String::rend() const noexcept -> reverse_iterator {
    return reverse_iterator(cbegin() - 1);
}

auto String::crend() const noexcept -> reverse_iterator {
    return reverse_iterator(cbegin() - 1);
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

auto String::insert(iterator pos, value_type ch) noexcept -> iterator {
    return insert(pos, 1, ch);
}

auto String::insert(iterator pos,
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

auto String::insert(iterator pos,
        iterator first, iterator last) noexcept -> iterator {
    return insert(size_type(pos - cbegin()), StringView{first, last}, copy_n);
}

auto String::insert(iterator pos, size_type count,
        value_type ch) noexcept -> iterator {
    return insert(size_type(pos - cbegin()), StringView{&ch, count}, fill_n);
}

auto String::erase(iterator position) noexcept -> iterator {
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
