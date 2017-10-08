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
 * @file json/number.cpp
 *
 * @brief Implementation
 */

#include "json/number.hpp"

#include <new>
#include <cmath>
#include <limits>

using json::Number;

static_assert(std::is_standard_layout<Number>::value,
        "json::Number is not a standard layout");

static inline bool is_equal(json::Double lhs, json::Double rhs) noexcept {
    return std::abs(lhs - rhs) <= std::numeric_limits<json::Double>::epsilon();
}

bool Number::operator!() const noexcept {
    return (*this == Number());
}

Number::Number(Type value) noexcept :
    m_type{value}
{
    switch (type()) {
    case INT:
        new (&m_int) Int{0};
        break;
    case UINT:
        new (&m_uint) Uint{0u};
        break;
    case DOUBLE:
        new (&m_double) Double{0.0};
        break;
    default:
        break;
    }
}

Number::operator Int() const noexcept {
    Int value = 0;

    switch (type()) {
    case INT:
        value = m_int;
        break;
    case UINT:
        value = Int(m_uint);
        break;
    case DOUBLE:
        value = Int(m_double);
        break;
    default:
        break;
    }

    return value;
}

Number::operator Uint() const noexcept {
    Uint value = 0;

    switch (type()) {
    case INT:
        value = Uint(m_int);
        break;
    case UINT:
        value = m_uint;
        break;
    case DOUBLE:
        value = Uint(m_double);
        break;
    default:
        break;
    }

    return value;
}

Number::operator Double() const noexcept {
    Double value = 0;

    switch (type()) {
    case INT:
        value = Double(m_int);
        break;
    case UINT:
        value = Double(m_uint);
        break;
    case DOUBLE:
        value = m_double;
        break;
    default:
        break;
    }

    return value;
}

Number Number::operator+(const Number& other) const noexcept {
    Number value;

    if ((DOUBLE == type()) || (DOUBLE == other.type())) {
        value = Double(*this) + Double(other);
    }
    else if ((INT == type()) || (INT == other.type())) {
        value = Int(*this) + Int(other);
    }
    else {
        value = Uint(*this) + Uint(other);
    }

    return value;
}

Number Number::operator-(const Number& other) const noexcept {
    Number value;

    if ((DOUBLE == type()) || (DOUBLE == other.type())) {
        value = Double(*this) - Double(other);
    }
    else if ((INT == type()) || (INT == other.type())) {
        value = Int(*this) - Int(other);
    }
    else {
        value = Uint(*this) - Uint(other);
    }

    return value;
}

Number Number::operator*(const Number& other) const noexcept {
    Number value;

    if ((DOUBLE == type()) || (DOUBLE == other.type())) {
        value = Double(*this) * Double(other);
    }
    else if ((INT == type()) || (INT == other.type())) {
        value = Int(*this) * Int(other);
    }
    else {
        value = Uint(*this) * Uint(other);
    }

    return value;
}

Number Number::operator/(const Number& other) const noexcept {
    Number value;

    if (other) {
        if ((DOUBLE == type()) || (DOUBLE == other.type())) {
            value = Double(*this) / Double(other);
        }
        else if ((INT == type()) || (INT == other.type())) {
            value = Int(*this) / Int(other);
        }
        else {
            value = Uint(*this) / Uint(other);
        }
    }

    return value;
}

Number Number::operator%(const Number& other) const noexcept {
    Number value;

    if (other) {
        if ((DOUBLE == type()) || (DOUBLE == other.type())) {
            value = std::fmod(Double(*this), Double(other));
        }
        else if ((INT == type()) || (INT == other.type())) {
            value = Int(*this) % Int(other);
        }
        else {
            value = Uint(*this) % Uint(other);
        }
    }

    return value;
}

bool Number::operator==(const Number& other) const noexcept {
    bool value;

    if ((DOUBLE == type()) || (DOUBLE == other.type())) {
        value = is_equal(Double(*this), Double(other));
    }
    else if ((INT == type()) || (INT == other.type())) {
        value = Int(*this) == Int(other);
    }
    else {
        value = Uint(*this) == Uint(other);
    }

    return value;
}

bool Number::operator<(const Number& other) const noexcept {
    bool value;

    if ((DOUBLE == type()) || (DOUBLE == other.type())) {
        value = Double(*this) < Double(other);
    }
    else if ((INT == type()) || (INT == other.type())) {
        value = Int(*this) < Int(other);
    }
    else {
        value = Uint(*this) < Uint(other);
    }

    return value;
}
