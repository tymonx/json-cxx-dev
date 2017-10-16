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

class StringBase {
public:
    StringBase() noexcept;

    StringBase(Unicode code, void* ptr, Size count) noexcept;

    StringBase(StringBase&& other) noexcept;

    StringBase(const StringBase& other) noexcept;

    StringBase& operator=(StringBase&& other) noexcept;

    StringBase& operator=(const StringBase& other) noexcept;

    StringBase& operator=(void* ptr) noexcept;

    template<typename T>
    StringBase& operator=(T* ptr) noexcept;

    StringBase& operator=(Size count) noexcept;

    StringBase& operator=(Unicode code) noexcept;

    Size size() const noexcept;

    Unicode unicode() const noexcept;

    operator void*() noexcept;

    template<typename T>
    operator T*() noexcept;

    operator const void*() const noexcept;

    template<typename T>
    operator const T*() const noexcept;

    ~StringBase() noexcept;
private:
    static constexpr auto UNICODE_WIDTH{3};
    static constexpr auto SIZE_WIDTH{(8 * sizeof(Size)) - UNICODE_WIDTH};

    struct Fields {
        Size m_unicode : UNICODE_WIDTH;
        Size m_size : SIZE_WIDTH;
    } m_fields;
    void* m_data;
};

template<typename T> auto
StringBase::operator=(T* ptr) noexcept -> StringBase& {
    return this->operator=(static_cast<void*>(ptr));
}

template<typename T>
StringBase::operator T*() noexcept {
    return reinterpret_cast<T*>(this->operator void*());
}

template<typename T>
StringBase::operator const T*() const noexcept {
    return reinterpret_cast<const T*>(this->operator const void*());
}

}

#endif /* JSON_STRING_BASE_HPP */
