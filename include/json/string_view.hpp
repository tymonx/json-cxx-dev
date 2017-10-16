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
 * @file json/string_view.hpp
 *
 * @brief JSON string view interface
 */

#ifndef JSON_STRING_VIEW_HPP
#define JSON_STRING_VIEW_HPP

#include "string_base.hpp"
#include "string_iterator.hpp"
#include "string_reverse_iterator.hpp"

#include <type_traits>

namespace json {

class StringView {
public:
    using value_type = char32_t;
    using size_type = Size;
    using iterator = StringIterator;
    using reverse_iterator = StringReverseIterator;
    using difference_type = Difference;

    template<typename T, Size N>
    using enable_utf = typename std::enable_if<(sizeof(T) == (N/8)), int>::type;

    static const size_type npos;

    StringView() noexcept;

    template<typename T, enable_utf<T, 8> = 0>
    StringView(const T* data) noexcept;

    template<typename T, enable_utf<T, 8> = 0>
    StringView(const T* data, size_type count) noexcept;

    template<typename T, enable_utf<T, 16> = 0>
    StringView(const T* data) noexcept;

    template<typename T, enable_utf<T, 16> = 0>
    StringView(const T* data, size_type count) noexcept;

    template<typename T, enable_utf<T, 32> = 0>
    StringView(const T* data) noexcept;

    template<typename T, enable_utf<T, 32> = 0>
    StringView(const T* data, size_type count) noexcept;

    StringView(const void* str) noexcept;

    StringView(Unicode code, const void* str) noexcept;

    StringView(const void* str, size_type count) noexcept;

    StringView(Unicode code, const void* str, size_type count) noexcept;

    StringView(iterator first, iterator last) noexcept;

    StringView(StringView&& other) noexcept;

    StringView(const StringView& other) noexcept;

    StringView& operator=(StringView&& other) noexcept;

    StringView& operator=(const StringView& other) noexcept;

    bool empty() const noexcept;

    size_type size() const noexcept;

    size_type length() const noexcept;

    size_type max_size() const noexcept;

    Unicode unicode() const noexcept;

    value_type at(difference_type n) const noexcept;

    value_type operator[](difference_type n) const noexcept;

    value_type front() const noexcept;

    value_type back() const noexcept;

    iterator begin() const noexcept;

    iterator cbegin() const noexcept;

    iterator end() const noexcept;

    iterator cend() const noexcept;

    reverse_iterator rbegin() const noexcept;

    reverse_iterator crbegin() const noexcept;

    reverse_iterator rend() const noexcept;

    reverse_iterator crend() const noexcept;

    StringView substr(size_type pos = 0, size_type count = npos) const noexcept;

    size_type copy(void* dest) const noexcept;

    int compare(const StringView& other) const noexcept;

    void remove_prefix(size_type count) noexcept;

    void remove_suffix(size_type count) noexcept;

    void swap(StringView& other) noexcept;

    size_type find(const StringView& other, size_type pos = 0) const noexcept;

    bool operator<(const StringView& other) const noexcept;

    bool operator<=(const StringView& other) const noexcept;

    bool operator>(const StringView& other) const noexcept;

    bool operator>=(const StringView& other) const noexcept;

    bool operator==(const StringView& other) const noexcept;

    bool operator!=(const StringView& other) const noexcept;

    ~StringView() noexcept;
private:
    StringBase m_base;
};

template<typename T, StringView::enable_utf<T, 8>>
StringView::StringView(const T* data) noexcept :
    StringView{Unicode::UTF8, data}
{ }

template<typename T, StringView::enable_utf<T, 8>>
StringView::StringView(const T* data, size_type count) noexcept :
    StringView{Unicode::UTF8, data, count}
{ }

template<typename T, StringView::enable_utf<T, 16>>
StringView::StringView(const T* data) noexcept :
    StringView{Unicode::UTF16, data}
{ }

template<typename T, StringView::enable_utf<T, 16>>
StringView::StringView(const T* data, size_type count) noexcept :
    StringView{Unicode::UTF16, data, count}
{ }

template<typename T, StringView::enable_utf<T, 32>>
StringView::StringView(const T* data) noexcept :
    StringView{Unicode::UTF32, data}
{ }

template<typename T, StringView::enable_utf<T, 32>>
StringView::StringView(const T* data, size_type count) noexcept :
    StringView{Unicode::UTF32, data, count}
{ }

}

#endif /* JSON_STRING_VIEW_HPP */
