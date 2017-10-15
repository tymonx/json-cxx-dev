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
 * @file json/string_iterator.hpp
 *
 * @brief JSON string iterator interface
 */

#ifndef JSON_STRING_ITERATOR_HPP
#define JSON_STRING_ITERATOR_HPP

#include "types.hpp"
#include "unicode.hpp"

#include <iterator>

namespace json {

class StringIterator {
public:
    using value_type = char32_t;
    using pointer = const value_type*;
    using reference = const value_type&;
    using difference_type = Difference;
    using iterator_category = std::random_access_iterator_tag;

    StringIterator(Unicode code, const void* data) noexcept;

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

    Unicode unicode() const noexcept;

    const void* base() const noexcept;

    operator bool() const noexcept;

    bool operator<(const StringIterator& other) const noexcept;

    bool operator<=(const StringIterator& other) const noexcept;

    bool operator>(const StringIterator& other) const noexcept;

    bool operator>=(const StringIterator& other) const noexcept;

    bool operator==(const StringIterator& other) const noexcept;

    bool operator!=(const StringIterator& other) const noexcept;

    ~StringIterator() noexcept;
private:
    Unicode m_unicode;
    const void* m_data;
};

}

#endif /* JSON_STRING_ITERATOR_HPP */
