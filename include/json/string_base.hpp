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
 * @file json/string_base.hpp
 *
 * @brief JSON string base interface
 */

#ifndef JSON_STRING_BASE_HPP
#define JSON_STRING_BASE_HPP

#include "types.hpp"
#include "unicode.hpp"

namespace json {

struct StringBase {
    static constexpr auto UNICODE_WIDTH{3};
    static constexpr auto SIZE_WIDTH{(8 * sizeof(Size)) - UNICODE_WIDTH};

    StringBase() noexcept = default;

    StringBase(const StringBase& other) noexcept = default;

    StringBase(StringBase&& other) noexcept = default;

    StringBase& operator=(const StringBase& other) noexcept = default;

    StringBase& operator=(StringBase&& other) noexcept = default;

    StringBase(Unicode code, void* ptr, Size count) noexcept;

    Size unicode : UNICODE_WIDTH;
    Size size : SIZE_WIDTH;
    void* data;
};

inline
StringBase::StringBase(Unicode code, void* ptr, Size count) noexcept :
    unicode{Size(code)},
    size{count},
    data{ptr}
{ }

}

#endif /* JSON_STRING_BASE_HPP */
