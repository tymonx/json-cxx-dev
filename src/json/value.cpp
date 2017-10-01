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
 * @file json/value.cpp
 *
 * @brief Implementation
 */

#include "json/value.hpp"

#include <new>
#include <type_traits>

using json::Value;

static_assert(std::is_standard_layout<Value>::value,
        "json::Value is not a standard layout");

Value::Value(Type value) noexcept :
    m_type{value}
{
    switch (type()) {
    case BOOLEAN:
        m_boolean = false;
        break;
    case STRING:
        new (&m_string) String();
        break;
    case NUMBER:
        new (&m_number) Number();
        break;
    case ARRAY:
        new (&m_array) Array();
        break;
    case OBJECT:
        new (&m_object) Object();
        break;
    case NIL:
    default:
        break;
    }
}

void Value::assign(const Value& other) noexcept {
    destroy();
    m_type = other.type();

    switch (type()) {
    case BOOLEAN:
        m_boolean = other.m_boolean;
        break;
    case STRING:
        new (&m_string) String(other.m_string);
        break;
    case NUMBER:
        new (&m_number) Number(other.m_number);
        break;
    case ARRAY:
        new (&m_array) Array(other.m_array);
        break;
    case OBJECT:
        new (&m_object) Object(other.m_object);
        break;
    case NIL:
    default:
        break;
    }
}

void Value::assign(Value&& other) noexcept {
    if (this != &other) {
        if (type() != other.type()) {
            destroy();
            m_type = other.type();
        }

        switch (type()) {
        case BOOLEAN:
            m_boolean = other.m_boolean;
            break;
        case STRING:
            new (&m_string) String(std::move(other.m_string));
            break;
        case NUMBER:
            new (&m_number) Number(std::move(other.m_number));
            break;
        case ARRAY:
            new (&m_array) Array(std::move(other.m_array));
            break;
        case OBJECT:
            new (&m_object) Object(std::move(other.m_object));
            break;
        case NIL:
        default:
            break;
        }
    }
}

void Value::destroy() noexcept {
    switch (type()) {
    case STRING:
        m_string.~String();
        break;
    case ARRAY:
        m_array.~Array();
        break;
    case OBJECT:
        m_object.~Object();
        break;
    case NUMBER:
        m_number.~Number();
        break;
    case NIL:
    case BOOLEAN:
    default:
        break;
    }
}

Value::operator Bool() const noexcept {
    bool value = false;

    switch (type()) {
    case BOOLEAN:
        value = m_boolean;
        break;
    case STRING:
        value = !m_string.empty();
        break;
    case NUMBER:
        value = bool(m_number);
        break;
    case ARRAY:
        value = !m_array.empty();
        break;
    case OBJECT:
        value = !m_object.empty();
        break;
    case NIL:
    default:
        break;
    }

    return value;
}
