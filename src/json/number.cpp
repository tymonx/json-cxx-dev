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
 *   list of conditions and the following disclaimer.
 *
 * @copright
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
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
