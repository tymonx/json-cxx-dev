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
 * @file json/utf8_iterator.hpp
 *
 * @brief JSON UTF-8 iterator interface
 */

#ifndef JSON_STRING_ITERATOR_HPP
#define JSON_STRING_ITERATOR_HPP

#include "types.hpp"
#include "unicode.hpp"

#include <type_traits>

namespace json {

class StringIterator {
public:
    using value_type = char32_t;
    using difference_type = Difference;

    template<typename T>
    using enable_utf8 = typename std::enable_if<
        (sizeof(T) == 1), char>::type;

    template<typename T>
    using enable_utf16 = typename std::enable_if<
        (sizeof(T) == 2), char16_t>::type;

    template<typename T>
    using enable_utf32 = typename std::enable_if<
        (sizeof(T) == 4), char32_t>::type;

    StringIterator() noexcept;

    StringIterator(Unicode unicode, const void* data) noexcept;

    template<typename T, enable_utf8<T> = 0>
    StringIterator(const T* data) noexcept;

    template<typename T, enable_utf16<T> = 0>
    StringIterator(const T* data) noexcept;

    template<typename T, enable_utf32<T> = 0>
    StringIterator(const T* data) noexcept;

    StringIterator(StringIterator&& other) noexcept;

    StringIterator(const StringIterator& other) noexcept;

    StringIterator& operator=(StringIterator&& other) noexcept;

    StringIterator& operator=(const StringIterator& other) noexcept;

    StringIterator& operator++() noexcept;

    StringIterator operator++(int) noexcept;

    StringIterator& operator--() noexcept;

    StringIterator operator--(int) noexcept;

    StringIterator operator+(difference_type n) const noexcept;

    StringIterator& operator+=(difference_type n) noexcept;

    StringIterator operator-(difference_type n) const noexcept;

    StringIterator& operator-=(difference_type n) noexcept;

    value_type operator[](difference_type n) const noexcept;

    value_type operator*() const noexcept;

    const void* base() const noexcept;

    operator bool() const noexcept;

    operator Unicode() const noexcept;

    bool operator<(const StringIterator& other) const noexcept;

    bool operator<=(const StringIterator& other) const noexcept;

    bool operator>(const StringIterator& other) const noexcept;

    bool operator>=(const StringIterator& other) const noexcept;

    bool operator==(const StringIterator& other) const noexcept;

    bool operator!=(const StringIterator& other) const noexcept;

    bool operator==(Unicode unicode) const noexcept;

    bool operator!=(Unicode unicode) const noexcept;

    ~StringIterator() noexcept;
private:
    Unicode m_unicode;
    const void* m_data;
};

template<typename T, StringIterator::enable_utf8<T>>
StringIterator::StringIterator(const T* data) noexcept :
    StringIterator{Unicode::UTF8, data}
{ }

template<typename T, StringIterator::enable_utf16<T>>
StringIterator::StringIterator(const T* data) noexcept :
    StringIterator{Unicode::UTF16, data}
{ }

template<typename T, StringIterator::enable_utf32<T>>
StringIterator::StringIterator(const T* data) noexcept :
    StringIterator{Unicode::UTF32, data}
{ }

bool operator==(Unicode unicode, const StringIterator& it) noexcept;

bool operator!=(Unicode unicode, const StringIterator& it) noexcept;

}

#endif /* JSON_STRING_ITERATOR_HPP */
