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
 * @file json/number.hpp
 *
 * @brief JSON number interface
 */

#ifndef JSON_NUMBER_HPP
#define JSON_NUMBER_HPP

#include "types.hpp"

#include <type_traits>

namespace json {

class Number {
public:
    template<typename T>
    using enable_int = typename std::enable_if<
            std::is_integral<T>::value && std::is_signed<T>::value, Int
        >::type;

    template<typename T>
    using enable_uint = typename std::enable_if<
            std::is_integral<T>::value && !std::is_signed<T>::value, Uint
        >::type;

    template<typename T>
    using enable_double = typename std::enable_if<
            std::is_floating_point<T>::value, char
        >::type;

    enum Type {
        INT,
        UINT,
        DOUBLE
    };

    Number() noexcept = default;

    Number(Type value) noexcept;

    template<typename T, enable_int<T> = 0>
    Number(T value) noexcept;

    template<typename T, enable_uint<T> = 0>
    Number(T value) noexcept;

    template<typename T, enable_double<T> = 0>
    Number(T value) noexcept;

    Number& operator++() noexcept;

    Number& operator--() noexcept;

    Number operator++(int) noexcept;

    Number operator--(int) noexcept;

    Number& operator+=(const Number& other) noexcept;

    Number& operator-=(const Number& other) noexcept;

    Number& operator*=(const Number& other) noexcept;

    Number& operator/=(const Number& other) noexcept;

    bool operator==(const Number& other) const noexcept;

    bool operator!=(const Number& other) const noexcept;

    bool operator<(const Number& other) const noexcept;

    bool operator>(const Number& other) const noexcept;

    bool operator<=(const Number& other) const noexcept;

    bool operator>=(const Number& other) const noexcept;

    Number operator+() const noexcept;

    Number operator-() const noexcept;

    Number operator+(const Number& other) const noexcept;

    Number operator-(const Number& other) const noexcept;

    Number operator*(const Number& other) const noexcept;

    Number operator/(const Number& other) const noexcept;

    Number operator%(const Number& other) const noexcept;

    operator Int() const noexcept;

    operator Uint() const noexcept;

    operator Double() const noexcept;

    operator bool() const noexcept;

    bool operator!() const noexcept;

    bool is_signed() const noexcept;

    bool is_unsigned() const noexcept;

    bool is_integral() const noexcept;

    bool is_floating_point() const noexcept;

    Type type() const noexcept;
private:
    Type m_type{INT};

    union {
        Int m_int{0};
        Uint m_uint;
        Double m_double;
    };
};

template<typename T, Number::enable_int<T>> inline
Number::Number(T value) noexcept :
    m_type{INT},
    m_int{Int(value)}
{ }

template<typename T, Number::enable_uint<T>> inline
Number::Number(T value) noexcept :
    m_type{UINT},
    m_uint{Uint(value)}
{ }

template<typename T, Number::enable_double<T>> inline
Number::Number(T value) noexcept :
    m_type{DOUBLE},
    m_double{Double(value)}
{ }

inline auto
Number::operator+=(const Number& other) noexcept -> Number& {
    return *this = (*this + other);
}

inline auto
Number::operator-=(const Number& other) noexcept -> Number& {
    return *this = (*this - other);
}

inline auto
Number::operator*=(const Number& other) noexcept -> Number& {
    return *this = (*this * other);
}

inline auto
Number::operator/=(const Number& other) noexcept -> Number& {
    return *this = (*this / other);
}

inline auto
Number::type() const noexcept -> Type {
    return m_type;
}

inline bool
Number::is_signed() const noexcept {
    return (INT == type()) || (DOUBLE == type());
}

inline bool
Number::is_unsigned() const noexcept {
    return (UINT == type());
}

inline bool
Number::is_integral() const noexcept {
    return (INT == type()) || (UINT == type());
}

inline bool
Number::is_floating_point() const noexcept {
    return (DOUBLE == type());
}

inline
Number::operator bool() const noexcept {
    return !!*this;
}

inline bool
Number::operator!=(const Number& other) const noexcept {
    return !(*this == other);
}

inline bool
Number::operator>(const Number& other) const noexcept {
    return (other < *this);
}

inline bool
Number::operator>=(const Number& other) const noexcept {
    return !(*this < other);
}

inline bool
Number::operator<=(const Number& other) const noexcept {
    return !(other < *this);
}

inline auto
Number::operator+() const noexcept -> Number {
    return *this;
}

inline auto
Number::operator-() const noexcept -> Number {
    return (Number() - *this);
}

inline auto
Number::operator++() noexcept -> Number& {
    return (*this += 1u);
}

inline auto
Number::operator--() noexcept -> Number& {
    return (*this -= 1u);
}

inline auto
Number::operator++(int) noexcept -> Number {
    auto value = *this;
    *this += 1u;
    return value;
}

inline auto
Number::operator--(int) noexcept -> Number {
    auto value = *this;
    *this -= 1u;
    return value;
}

}

#endif /* JSON_NUMBER_HPP */
