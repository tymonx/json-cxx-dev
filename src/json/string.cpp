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
using json::StringBase;
using json::Unicode;
using json::Allocator;
using Iterator = json::String::iterator;

static_assert(std::is_standard_layout<String>::value,
        "json::String is not a standard layout");

const json::Size String::npos = std::numeric_limits<json::Size>::max();

using UtfEncode = void* (*)(char32_t ch, void* dst);
using UtfCopy = void (*)(Iterator first, Iterator last, void* dst);
using UtfBytes = Size (*)(char32_t ch);

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

static Size utf8_bytes(char32_t ch) noexcept {
    Size bytes;

    if (ch < 0x80) {
        bytes = 1;
    }
    else if (ch < 0x800) {
        bytes = 2;
    }
    else if (ch < 0x10000) {
        bytes = 3;
    }
    else if (ch < 0x110000) {
        bytes = 4;
    }
    else {
        bytes = 3;
    }

    return bytes;
}

static Size utf16_bytes(char32_t ch) noexcept {
    Size bytes;

    if ((ch < 0xD800) || (ch > 0xDFFF)) {
        bytes = 2;
    }
    else {
        bytes = 4;
    }

    return bytes;
}

static Size utf32_bytes(char32_t /* ch */) noexcept {
    return 4;
}

static UtfBytes utf_bytes_strategy(Unicode encoding) noexcept {
    UtfBytes bytes_strategy;

    switch (encoding) {
    case Unicode::UTF8:
        bytes_strategy = utf8_bytes;
        break;
    case Unicode::UTF16:
    case Unicode::UTF16BE:
    case Unicode::UTF16LE:
        bytes_strategy = utf16_bytes;
        break;
    case Unicode::UTF32:
    case Unicode::UTF32BE:
    case Unicode::UTF32LE:
    default:
        bytes_strategy = utf32_bytes;
        break;
    }

    return bytes_strategy;
}

static Size utf_bytes(Unicode encoding, char32_t ch) noexcept {
    return utf_bytes_strategy(encoding)(ch);
}

static char32_t utf_valid(char32_t ch) noexcept {
    return (((ch < 0xD800) || (ch > 0xDFFF)) && (ch < 0x110000))
        ? ch : 0xFFFD;
}

template<typename T>
static void utf_copy(Iterator first, Iterator last, void* dst) noexcept {
    std::copy(reinterpret_cast<const T*>(first.base()),
            reinterpret_cast<const T*>(last.base()),
            reinterpret_cast<T*>(dst));
}

template<Unicode Encoding>
static void utf_copy(Iterator first, Iterator last, void* dst) noexcept {
    auto encode = utf_encode(Encoding);

    while (first < last) {
        dst = encode(*first++, dst);
    }
}

static Unicode utf_normalize(Unicode encoding) noexcept {
    switch (encoding) {
    case Unicode::UTF16BE:
    case Unicode::UTF16LE:
        encoding = Unicode::UTF16;
        break;
    case Unicode::UTF32BE:
    case Unicode::UTF32LE:
        encoding = Unicode::UTF32;
        break;
    case Unicode::UTF8:
    case Unicode::UTF16:
    case Unicode::UTF32:
    default:
        break;
    }

    return encoding;
}

static Unicode utf_normalize(Iterator first, Iterator last) noexcept {
    Unicode encoding = first.unicode();

    if (Unicode::UTF16 == encoding) {
        encoding = ((first < last) && (0x0000FFFE == *first))
            ? Unicode::UTF16LE : Unicode::UTF16BE;
    }
    else if (Unicode::UTF32 == encoding) {
        encoding = ((first < last) && (0xFFFE0000 == *first))
            ? Unicode::UTF32LE : Unicode::UTF32BE;
    }

    return encoding;
}

static Size utf_length(Iterator first, Iterator last) noexcept {
    return (first < last) ? Size(std::uintptr_t(last.base()) -
            std::uintptr_t(first.base())) : 0;
}

static Size utf_length(Unicode encoding, Iterator first,
        Iterator last) noexcept {
    Size count = 0;

    if (utf_normalize(encoding) == utf_normalize(first.unicode())) {
        count = utf_length(first, last);
    }
    else {
        auto bytes_strategy = utf_bytes_strategy(encoding);

        while (first < last) {
            count += bytes_strategy(*first++);
        }
    }

    return count;
}

static Size utf_length(Unicode encoding, const String& other) noexcept {
    return utf_length(encoding, other.cbegin(), other.cend());
}

static UtfCopy utf8_copy_strategy(Unicode encoding) noexcept {
    bool copy_fast = (Unicode::UTF8 == encoding);
    return copy_fast ? utf_copy<std::uint8_t> : utf_copy<Unicode::UTF8>;
}

static UtfCopy utf16le_copy_strategy(Unicode encoding) noexcept {
    bool copy_fast = (Unicode::UTF16LE == encoding);
    return copy_fast ? utf_copy<std::uint16_t> : utf_copy<Unicode::UTF16LE>;
}

static UtfCopy utf16be_copy_strategy(Unicode encoding) noexcept {
    bool copy_fast = (Unicode::UTF16BE == encoding);
    return copy_fast ? utf_copy<std::uint16_t> : utf_copy<Unicode::UTF16BE>;
}

static UtfCopy utf32le_copy_strategy(Unicode encoding) noexcept {
    bool copy_fast = (Unicode::UTF32LE == encoding);
    return copy_fast ? utf_copy<std::uint32_t> : utf_copy<Unicode::UTF32LE>;
}

static UtfCopy utf32be_copy_strategy(Unicode encoding) noexcept {
    bool copy_fast = (Unicode::UTF32BE == encoding);
    return copy_fast ? utf_copy<std::uint32_t> : utf_copy<Unicode::UTF32BE>;
}

static UtfCopy utf_copy_strategy(Unicode encoding, Iterator first,
        Iterator last) noexcept {
    UtfCopy copy_strategy;

    switch (encoding) {
    case Unicode::UTF8:
        copy_strategy = utf8_copy_strategy(first.unicode());
        break;
    case Unicode::UTF16:
    case Unicode::UTF16BE:
        copy_strategy = utf16be_copy_strategy(utf_normalize(first, last));
        break;
    case Unicode::UTF16LE:
        copy_strategy = utf16le_copy_strategy(utf_normalize(first, last));
        break;
    case Unicode::UTF32:
    case Unicode::UTF32BE:
        copy_strategy = utf32be_copy_strategy(utf_normalize(first, last));
        break;
    case Unicode::UTF32LE:
        copy_strategy = utf32le_copy_strategy(utf_normalize(first, last));
        break;
    default:
        copy_strategy = nullptr;
        break;
    }

    return copy_strategy;
}

static UtfCopy utf_copy_strategy(Unicode encoding,
        const String& other) noexcept {
    return utf_copy_strategy(encoding, other.cbegin(), other.cend());
}

static Size length(Unicode encoding,
        const std::initializer_list<String::value_type>& ilist) noexcept {
    Size count = 0;
    auto bytes = utf_bytes_strategy(encoding);

    for (const auto& ch : ilist) {
        count += bytes(ch);
    }

    return count;
}

static void resize(Allocator& allocator, StringBase& base,
        Size count) noexcept {
    if (count > allocator.size(base.data)) {
        auto ptr = allocator.reallocate(base.data, count);
        if (ptr) {
            base.data = ptr;
            base.size = count;
        }
    }
    else {
        base.size = count;
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
    String{other.cbegin(), other.cend(), const_cast<String&>(other).allocator()}
{ }

String::String(const String& other, allocator_type& alloc) noexcept :
    String{other.cbegin(), other.cend(), alloc}
{ }

String::String(String&& other) noexcept :
    m_base{other.m_base},
    m_allocator{&other.allocator()}
{
    other.m_base.data = nullptr;
}

String::String(String&& other, allocator_type& alloc) noexcept :
    m_base{other.m_base},
    m_allocator{&other.allocator()}
{
    if (&other.allocator() == &alloc) {
        other.m_base.data = nullptr;
        other.m_base.size = 0;
    }
    else {
        m_base = {other.unicode(), nullptr, 0};
        (*this) = std::cref(other);
    }
}

String::String(size_type count, value_type ch) noexcept :
    String{Unicode::UTF8, count, ch, Allocator::get_instance()}
{ }

String::String(Unicode encoding, size_type count, value_type ch) noexcept :
    String{encoding, count, ch, Allocator::get_instance()}
{ }

String::String(size_type count, value_type ch, allocator_type& alloc) noexcept :
    String{Unicode::UTF8, count, ch, alloc}
{ }

String::String(Unicode encoding, size_type count, value_type ch,
        allocator_type& alloc) noexcept :
    m_base{encoding, nullptr, 0},
    m_allocator{&alloc}
{
    assign(count, ch);
}

String::String(const void* str) noexcept :
    String{StringView{str}, Allocator::get_instance()}
{ }

String::String(const void* str, allocator_type& alloc) noexcept :
    String{StringView{str}, alloc}
{ }

String::String(Unicode encoding, const void* str,
        allocator_type& alloc) noexcept :
    String{StringView{encoding, str}, alloc}
{ }

String::String(const void* str, size_type count) noexcept :
    String{StringView{str, count}, Allocator::get_instance()}
{ }

String::String(const void* str, size_type count,
        allocator_type& alloc) noexcept :
    String{StringView{str, count}, alloc}
{ }

String::String(Unicode encoding, const void* str, size_type count,
        allocator_type& alloc) noexcept :
    String{StringView{encoding, str, count}, alloc}
{ }

String::String(iterator first, iterator last) noexcept :
    String{first, last, Allocator::get_instance()}
{ }

String::String(iterator first, iterator last, allocator_type& alloc) noexcept :
    m_base{first.unicode(), nullptr, utf_length(first, last)},
    m_allocator{&alloc}
{
    assign(first, last);
}

String::String(const String& other, size_type pos, size_type count) noexcept :
    String{StringView{other}.substr(pos, count),
        const_cast<String&>(other).allocator()}
{ }

String::String(const String& other, size_type pos, size_type count,
        allocator_type& alloc) noexcept :
    String{StringView{other}.substr(pos, count), alloc}
{ }

String::String(std::initializer_list<value_type> ilist) noexcept :
    String{ilist, Allocator::get_instance()}
{ }

String::String(Unicode encoding,
        std::initializer_list<value_type> ilist) noexcept :
    String{encoding, ilist, Allocator::get_instance()}
{ }

String::String(std::initializer_list<value_type> ilist,
        allocator_type& alloc) noexcept :
    String{Unicode::UTF8, ilist, alloc}
{ }

String::String(Unicode encoding, std::initializer_list<value_type> ilist,
        allocator_type& alloc) noexcept :
    m_base{encoding, nullptr, 0},
    m_allocator{&alloc}
{
    assign(ilist);
}

String::String(const StringView& other) noexcept :
    String{other.cbegin(), other.cend(), Allocator::get_instance()}
{ }

String::String(const StringView& other, allocator_type& alloc) noexcept :
    String{other.cbegin(), other.cend(), alloc}
{ }

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
    ::resize(allocator(), m_base, utf_bytes(unicode(), ch) * count);

    ch = utf_valid(ch);
    void* dst = m_base.data;
    void* dst_end = reinterpret_cast<std::uint8_t*>(dst) + m_base.size;
    auto encode = utf_encode(unicode());

    while (count-- && (dst < dst_end)) {
        dst = encode(ch, dst);
    }

    return *this;
}

auto String::assign(const String& other) noexcept -> String& {
    return assign(StringView{other});
}

auto String::assign(const String& other, size_type pos,
        size_type count) noexcept -> String& {
    return assign(StringView{other}.substr(pos, count));
}

auto String::assign(String&& other) noexcept -> String& {
    if (&other != this) {
        if (&other.allocator() == &allocator()) {
            allocator().deallocate(m_base.data);
            m_base = other.m_base;
            other.m_base.data = nullptr;
            other.m_base.size = 0;
        }
        else {
            assign(static_cast<const String&>(other));
        }
    }
    return *this;
}

auto String::assign(const_pointer s) noexcept -> String& {
    return assign(StringView{s});
}

auto String::assign(const_pointer s, size_type count) noexcept -> String& {
    return assign(StringView{s, count});
}

auto String::assign(
        std::initializer_list<value_type> ilist) noexcept -> String& {
    ::resize(allocator(), m_base, ::length(unicode(), ilist));

    void* dst = m_base.data;
    void* dst_end = reinterpret_cast<std::uint8_t*>(m_base.data) + m_base.size;

    auto it = ilist.begin();
    auto count = ilist.size();
    auto encode = utf_encode(unicode());

    while (count-- && (dst < dst_end)) {
        dst = encode(utf_valid(*it++), dst);
    }

    return *this;
}

auto String::assign(iterator first, iterator last) noexcept -> String& {
    ::resize(allocator(), m_base, utf_length(unicode(), first, last));

    auto copy_strategy = utf_copy_strategy(unicode(), first, last);

    m_base.data = allocator().allocate(m_base.size);
    if (m_base.data) {
        copy_strategy(first, last, m_base.data);
    }
    else {
        m_base.size = 0;
    }

    return *this;
}

auto String::assign(const StringView& other) noexcept -> String& {
    return assign(other.cbegin(), other.cend());
}

auto String::unicode() const noexcept -> Unicode {
    return Unicode(m_base.unicode);
}

auto String::at(size_type pos) const noexcept -> value_type {
    return cbegin()[StringIterator::difference_type(pos)];
}

auto String::operator[](size_type pos) const noexcept -> value_type {
    return cbegin()[StringIterator::difference_type(pos)];
}

auto String::front() const noexcept -> value_type {
    return *cbegin();
}

auto String::back() const noexcept -> value_type {
    return *(--cend());
}

auto String::begin() const noexcept -> iterator {
    return cbegin();
}

auto String::cbegin() const noexcept -> iterator {
    return {unicode(), m_base.data};
}

auto String::end() const noexcept -> iterator {
    return cend();
}

auto String::cend() const noexcept -> iterator {
    return {unicode(),
        reinterpret_cast<const std::uint8_t*>(m_base.data) + m_base.size};
}

auto String::rbegin() const noexcept -> reverse_iterator {
    return crbegin();
}

auto String::crbegin() const noexcept -> reverse_iterator {
    return --cend();
}

auto String::rend() const noexcept -> reverse_iterator {
    return crend();
}

auto String::crend() const noexcept -> reverse_iterator {
    return --cbegin();
}

void String::shrink_to_fit() noexcept {
    if (allocator().size(m_base.data) > m_base.size) {
        auto ptr = allocator().reallocate(m_base.data, m_base.size);
        if (ptr) {
            m_base.data = ptr;
        }
    }
}

void String::reserve(size_type new_capacity) noexcept {
    new_capacity *= 4;

    if (allocator().size(m_base.data) < new_capacity) {
        auto ptr = allocator().reallocate(m_base.data, new_capacity);
        if (ptr) {
            m_base.data = ptr;
        }
    }
}

auto String::capacity() const noexcept -> size_type {
    return allocator().size(m_base.data) / 4;
}

auto String::empty() const noexcept -> bool {
    return (0 == m_base.size);
}

auto String::size() const noexcept -> size_type {
    return length();
}

auto String::length() const noexcept -> size_type {
    return utf_length(unicode(), cbegin(), cend());
}

void String::clear() noexcept {
    m_base.size = 0;
}

void String::resize(size_type count) noexcept {
    resize(count, 0xFFFD);
}

void String::resize(size_type count, value_type ch) noexcept {
    auto old_size = m_base.size;
    count = utf_bytes(unicode(), ch) + m_base.size;
    ::resize(allocator(), m_base, count);
    if (count == m_base.size) {
        void* it = reinterpret_cast<std::uint8_t*>(m_base.data) + old_size;
        void* it_end = reinterpret_cast<std::uint8_t*>(m_base.data) + count;
        auto encode = utf_encode(unicode());
        ch = utf_valid(ch);

        while (it < it_end) {
            it = encode(ch, it);
        }
    }
}

auto String::allocator() noexcept -> allocator_type& {
    return *m_allocator;
}

auto String::allocator() const noexcept -> const allocator_type& {
    return *m_allocator;
}

void String::pop_back() noexcept {
    if (!empty()) {
        auto it = --cend();
        m_base.size = Size(std::uintptr_t(it.base()) -
                std::uintptr_t(m_base.data));
    }
}

void String::push_back(value_type ch) noexcept {
    auto old_size = m_base.size;
    auto count = utf_bytes(unicode(), ch) + m_base.size;
    ::resize(allocator(), m_base, count);
    if (count == m_base.size) {
        auto ptr = reinterpret_cast<std::uint8_t*>(m_base.data) + old_size;
        utf_encode(unicode())(utf_valid(ch), ptr);
    }
}

String::operator StringView() const noexcept {
    return StringView{*this};
}

String::~String() noexcept {
    allocator().deallocate(m_base.data);
}
