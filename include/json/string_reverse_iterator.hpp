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
 * @file json/string_reverse_iterator.hpp
 *
 * @brief JSON string reverse iterator interface
 */

#ifndef JSON_STRING_REVERSE_ITERATOR_HPP
#define JSON_STRING_REVERSE_ITERATOR_HPP

#include "string_iterator.hpp"

namespace json {

class StringReverseIterator {
public:
    using value_type = StringIterator::value_type;
    using pointer = StringIterator::pointer;
    using reference = StringIterator::reference;
    using difference_type = StringIterator::difference_type;
    using iterator_category = StringIterator::iterator_category;

    StringReverseIterator(const StringIterator& other) noexcept;

    StringReverseIterator(StringReverseIterator&& other) noexcept;

    StringReverseIterator(const StringReverseIterator& other) noexcept;

    StringReverseIterator& operator=(StringReverseIterator&& other) noexcept;

    StringReverseIterator& operator=(
            const StringReverseIterator& other) noexcept;

    StringReverseIterator& operator++() noexcept;

    StringReverseIterator operator++(int) noexcept;

    StringReverseIterator& operator--() noexcept;

    StringReverseIterator operator--(int) noexcept;

    StringReverseIterator operator+(difference_type n) const noexcept;

    StringReverseIterator& operator+=(difference_type n) noexcept;

    StringReverseIterator operator-(difference_type n) const noexcept;

    StringReverseIterator& operator-=(difference_type n) noexcept;

    value_type operator[](difference_type n) const noexcept;

    value_type operator*() const noexcept;

    Unicode unicode() const noexcept;

    const void* base() const noexcept;

    operator bool() const noexcept;

    bool operator<(const StringReverseIterator& other) const noexcept;

    bool operator<=(const StringReverseIterator& other) const noexcept;

    bool operator>(const StringReverseIterator& other) const noexcept;

    bool operator>=(const StringReverseIterator& other) const noexcept;

    bool operator==(const StringReverseIterator& other) const noexcept;

    bool operator!=(const StringReverseIterator& other) const noexcept;

    ~StringReverseIterator() noexcept;
private:
    StringIterator m_iterator;
};

}

#endif /* JSON_STRING_REVERSE_ITERATOR_HPP */
