/*!
 * @copright
 * Copyright (c) 2017, Tymoteusz Blazejczyk
 * All rights reserved.
 *
 * @copright
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * @copright
 * * Redistributions of source code must retain the above copyright notice, this
 *   span of conditions and the following disclaimer.
 *
 * @copright
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this span of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * @copright
 * * Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * @copright
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
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
