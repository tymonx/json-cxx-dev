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
 * @file json/string_view.cpp
 *
 * @brief Implementation
 */

#include "json/string_view.hpp"
#include "json/string.hpp"

#include <limits>
#include <cstdint>
#include <algorithm>
#include <type_traits>

using json::StringView;

static_assert(std::is_standard_layout<StringView>::value,
        "json::StringView is not a standard layout");

static_assert(std::is_standard_layout<json::StringBase>::value,
        "json::StringBase is not a standard layout");

static auto pointer(const void* ptr) noexcept -> const std::uint8_t* {
    return reinterpret_cast<const std::uint8_t*>(ptr);
}

static void utf_copy(const void* first, const void* last, void* dest) noexcept {
    std::copy(pointer(first), pointer(last),
            reinterpret_cast<std::uint8_t*>(dest));
}

static auto utf_distance(const void* first, const void* last) noexcept ->
        StringView::size_type {
    return (first < last) ? StringView::size_type(
            pointer(last) - pointer(first)) : 0;
}

static constexpr auto STRING_MAX_SIZE{std::numeric_limits<
    StringView::size_type>::max() / 32};

const StringView::size_type StringView::npos{STRING_MAX_SIZE + 1};

StringView::StringView() noexcept :
    m_base{}
{ }

StringView::StringView(const void* str) noexcept :
    StringView{Unicode::UTF8, str}
{ }

StringView::StringView(Unicode code, const void* str) noexcept :
    m_base{code, const_cast<void*>(str), 0}
{
    if (str) {
        auto count = max_size();
        auto it = iterator{code, str};

        while (*it && count--) {
            ++it;
        }

        m_base.size = utf_distance(m_base.data, it.base());
    }
}

StringView::StringView(const void* str, size_type count) noexcept :
    StringView{Unicode::UTF8, str, count}
{ }

StringView::StringView(Unicode code, const void* str,
        size_type count) noexcept :
    m_base{code, const_cast<void*>(str), 0}
{
    if (str) {
        auto it = iterator{code, str};

        while (count--) {
            ++it;
        }

        m_base.size = utf_distance(m_base.data, it.base());
    }
}

StringView::StringView(iterator first, iterator last) noexcept :
    m_base{first.unicode(), const_cast<void*>(first.base()),
        utf_distance(first.base(), last.base())}
{ }

StringView::StringView(const String& other) noexcept :
    StringView{other.cbegin(), other.cend()}
{ }

StringView::StringView(StringView&& other) noexcept :
    m_base{other.m_base}
{ }

StringView::StringView(const StringView& other) noexcept :
    m_base{other.m_base}
{ }

auto StringView::operator=(StringView&& other) noexcept -> StringView& {
    m_base = other.m_base;
    return *this;
}

auto StringView::operator=(const StringView& other) noexcept -> StringView& {
    m_base = other.m_base;
    return *this;
}

auto StringView::empty() const noexcept -> bool {
    return (0 == m_base.size);
}

auto StringView::size() const noexcept -> size_type {
    value_type count = 0;

    auto it = cbegin();
    auto it_end = cend();

    while (it < it_end) {
        ++it;
        ++count;
    }

    return count;
}

auto StringView::length() const noexcept -> size_type {
    return size();
}

auto StringView::max_size() const noexcept -> size_type {
    return STRING_MAX_SIZE;
}

auto StringView::unicode() const noexcept -> Unicode {
    return Unicode(m_base.unicode);
}

auto StringView::at(difference_type n) const noexcept -> value_type {
    return (*this)[n];
}

auto StringView::operator[](difference_type n) const noexcept -> value_type {
    return *(cbegin() + n);
}

auto StringView::front() const noexcept -> value_type {
    return *cbegin();
}

auto StringView::back() const noexcept -> value_type {
    return *(--cend());
}

auto StringView::begin() const noexcept -> iterator {
    return cbegin();
}

auto StringView::cbegin() const noexcept -> iterator {
    return {unicode(), m_base.data};
}

auto StringView::end() const noexcept -> iterator {
    return cend();
}

auto StringView::cend() const noexcept -> iterator {
    return {unicode(), pointer(m_base.data) + m_base.size};
}

auto StringView::rbegin() const noexcept -> reverse_iterator {
    return crbegin();
}

auto StringView::crbegin() const noexcept -> reverse_iterator {
    return --end();
}

auto StringView::rend() const noexcept -> reverse_iterator {
    return crend();
}

auto StringView::crend() const noexcept -> reverse_iterator {
    return iterator{unicode(), pointer(m_base.data) - 1};
}

auto StringView::substr(size_type pos,
        size_type count) const noexcept -> StringView {
    auto it_first = cbegin();
    auto it_end = cend();

    while ((it_first < it_end) && pos--) {
        ++it_first;
    }

    auto it_last = (npos == count) ? it_end : it_first;

    while ((it_last < it_end) && count--) {
        ++it_last;
    }

    return {it_first, it_last};
}

auto StringView::copy(void* dest) const noexcept -> size_type {
    size_type copied = 0;

    if (dest) {
        copied = length();
        utf_copy(cbegin().base(), cend().base(), dest);
    }

    return copied;
}

auto StringView::compare(const StringView& other) const noexcept -> int {
    int result = 0;

    auto it1 = cbegin();
    auto it2 = other.cbegin();
    auto it1_end = cend();
    auto it2_end = other.cend();
    size_type length1 = 0;
    size_type length2 = 0;

    while ((it1 < it1_end) && (it2 < it2_end) && !result) {
        ++it1;
        ++it2;
        ++length1;
        ++length2;

        auto value1 = *it1;
        auto value2 = *it2;

        if (value1 < value2) {
            result = -1;
        }
        else if (value1 > value2) {
            result = 1;
        }
    }

    if (!result) {
        while (it1 < it1_end) {
            ++it1;
            ++length1;
        }

        while (it2 < it2_end) {
            ++it2;
            ++length2;
        }

        if (length1 < length2) {
            result = -1;
        }
        else if (length1 > length2) {
            result = 1;
        }
    }

    return result;
}

void StringView::remove_prefix(size_type count) noexcept {
    auto it = cbegin();
    auto it_end = cend();

    while ((it < it_end) && count--) {
        ++it;
    }

    m_base.data = const_cast<void*>(it.base());
    m_base.size = utf_distance(m_base.data, it_end.base());
}

void StringView::remove_suffix(size_type count) noexcept {
    auto it_begin = cbegin();
    auto it = cend();

    while ((it_begin < it) && count--) {
        --it;
    }

    m_base.size = utf_distance(m_base.data, it.base());
}

void StringView::swap(StringView& other) noexcept {
    auto tmp{*this};
    *this = other;
    other = tmp;
}

auto StringView::find(const StringView& other,
        size_type pos) const noexcept -> size_type {
    bool found = false;
    size_type index = 0;

    auto it = cbegin();
    auto it1_end = cend();

    while ((it < it1_end) && pos--) {
        ++index;
        ++it;
    }

    auto it2_end = other.cend();

    if (!other.empty()) {
        while (it < it1_end) {
            auto it1 = it;
            auto it2 = other.cbegin();

            while (it2 < it2_end) {
                if (it1 < it1_end) {
                    if (*it1 == *it2) {
                        ++it2;
                        found = (it2 >= it2_end);
                        ++it1;
                    }
                    else {
                        it2 = it2_end;
                    }
                }
                else {
                    it2 = it2_end;
                }
            }

            if (found) {
                it = it1_end;
            }
            else {
                ++index;
                ++it;
            }
        }
    }
    else {
        found = (it >= it1_end);
    }

    return found ? index : npos;
}

bool StringView::operator<(const StringView& other) const noexcept {
    return (compare(other) < 0);
}

bool StringView::operator<=(const StringView& other) const noexcept {
    return (compare(other) <= 0);
}

bool StringView::operator>(const StringView& other) const noexcept {
    return (compare(other) > 0);
}

bool StringView::operator>=(const StringView& other) const noexcept {
    return (compare(other) >= 0);
}

bool StringView::operator==(const StringView& other) const noexcept {
    return (compare(other) == 0);
}

bool StringView::operator!=(const StringView& other) const noexcept {
    return (compare(other) != 0);
}

StringView::~StringView() noexcept { }
