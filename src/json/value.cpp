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
 * @file json/value.cpp
 *
 * @brief Implementation
 */

#include "json/value.hpp"
#include "json/pair.hpp"

#include <new>
#include <type_traits>

using json::Value;

static_assert(std::is_standard_layout<Value>::value,
        "json::Value is not a standard layout");

Value::Value(Type value, allocator_type& alloc) noexcept :
    m_type{value}
{
    switch (type()) {
    case BOOLEAN:
        m_boolean = false;
        break;
    case STRING:
        new (&m_string) String{alloc};
        break;
    case NUMBER:
        new (&m_number) Number{};
        break;
    case ARRAY:
        new (&m_array) Array{alloc};
        break;
    case OBJECT:
        new (&m_object) Object{alloc};
        break;
    case NIL:
    default:
        break;
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

void Value::assign(const Value& other) noexcept {
    destroy();

    m_type = other.type();
    m_parent = other.parent();

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
        m_parent = other.parent();

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

        other.m_type = NIL;
    }
}

Value::pointer Value::root() const noexcept {
    auto it = m_parent;

    while (it) {
        it = it->parent();
    }

    return it;
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

void Value::push_back(const value_type& value) noexcept {
    if (is_array()) {
        m_array.emplace_back(value, this);
    }
    else if (is_object()) {
        m_object.emplace_back(value, this);
    }
    else {
        Array array;

        if (!is_null()) {
            array.emplace_back(std::move(*this), this);
        }

        array.emplace_back(value, this);

        if (is_null()) {
            m_type = ARRAY;
            new (&m_array) Array(std::move(array));
        }
    }
}

void Value::push_back(value_type&& value) noexcept {
    if (is_array()) {
        m_array.emplace_back(std::move(value), this);
    }
    else if (is_object()) {
        m_object.emplace_back(std::move(value), this);
    }
    else {
        Array array;

        if (!is_null()) {
            array.emplace_back(std::move(*this), this);
        }

        array.emplace_back(std::move(value), this);

        if (is_null()) {
            m_type = ARRAY;
            new (&m_array) Array(std::move(array));
        }
    }
}

void Value::pop_back() noexcept {
    if (is_array()) {
        m_array.pop_back();
    }
    else if (is_object()) {
        m_object.pop_back();
    }
}

Value::iterator Value::begin() noexcept {
    iterator it;

    if (is_array()) {
        it = m_array.begin();
    }
    else if (is_object()) {
        it = m_object.begin();
    }

    return it;
}

Value::reverse_iterator Value::rbegin() noexcept {
    reverse_iterator it;

    if (is_array()) {
        it = m_array.rbegin();
    }
    else if (is_object()) {
        it = m_object.rbegin();
    }

    return it;
}
