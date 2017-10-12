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

#ifndef JSON_UTF8_ITERATOR_HPP
#define JSON_UTF8_ITERATOR_HPP

#include "types.hpp"

namespace json {

class UTF8Iterator {
public:
    using value_type = char32_t;
    using difference_type = Difference;

    UTF8Iterator() noexcept;

    UTF8Iterator(char* ptr) noexcept;

    UTF8Iterator(UTF8Iterator&& other) noexcept;

    UTF8Iterator(const UTF8Iterator& other) noexcept;

    UTF8Iterator& operator=(UTF8Iterator&& other) noexcept;

    UTF8Iterator& operator=(const UTF8Iterator& other) noexcept;

    UTF8Iterator& operator++() noexcept;

    UTF8Iterator operator++(int) noexcept;

    UTF8Iterator& operator--() noexcept;

    UTF8Iterator operator--(int) noexcept;

    UTF8Iterator operator+(difference_type n) const noexcept;

    UTF8Iterator& operator+=(difference_type n) noexcept;

    UTF8Iterator operator-(difference_type n) const noexcept;

    UTF8Iterator& operator-=(difference_type n) noexcept;

    value_type operator[](difference_type n) const noexcept;

    value_type operator*() const noexcept;

    operator bool() const noexcept;

    ~UTF8Iterator() noexcept;
private:
    char* m_data;
};

}

#endif /* JSON_UTF8_ITERATOR_HPP */
