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
 * @file json/value.hpp
 *
 * @brief JSON value interface
 */

#ifndef JSON_VALUE_HPP
#define JSON_VALUE_HPP

#include "types.hpp"
#include "array.hpp"
#include "string.hpp"
#include "number.hpp"
#include "object.hpp"

namespace json {

class Value {
public:
    enum Type {
        NIL,
        BOOLEAN,
        NUMBER,
        STRING,
        ARRAY,
        OBJECT
    };

    Value() noexcept;

    Value(Type value) noexcept;

    Value(Null value) noexcept;

    Value(Bool value) noexcept;

    Value(Value&& other) noexcept;

    Value(const Value& other) noexcept;

    Value& operator=(Value&&) noexcept;

    Value& operator=(const Value&) noexcept;

    void assign(Value&&) noexcept;

    void assign(const Value&) noexcept;

    void assign(Null value) noexcept;

    void assign(Bool value) noexcept;

    void assign(const Number& value) noexcept;

    void assign(Number&& value) noexcept;

    void assign(const String& value) noexcept;

    void assign(String&& value) noexcept;

    void assign(const Array& value) noexcept;

    void assign(Array&& value) noexcept;

    Type type() const noexcept;

    bool is_null() const noexcept;

    bool is_bool() const noexcept;

    bool is_number() const noexcept;

    bool is_string() const noexcept;

    bool is_array() const noexcept;

    bool is_object() const noexcept;

    Null as_null() const noexcept;

    Bool as_bool() const noexcept;

    const Number& as_number() const noexcept;

    const String& as_string() const noexcept;

    const Array& as_array() const noexcept;

    const Array& as_object() const noexcept;

    bool operator!() const noexcept;

    operator Null() const noexcept;

    operator bool() const noexcept;

    operator const Number&() const noexcept;

    operator const String&() const noexcept;

    operator const Array&() const noexcept;

    operator const Object&() const noexcept;

    ~Value() noexcept;
private:
    void destroy() noexcept;

    Type m_type{NIL};

    union {
        Bool m_boolean;
        Number m_number;
        String m_string;
        Array m_array;
        Object m_object;
    };
};

inline
Value::~Value() noexcept {
    destroy();
}

inline
Value::Value() noexcept
{ }

inline
Value::Value(Value&& other) noexcept {
    assign(std::move(other));
}

inline
Value::Value(const Value& other) noexcept {
    assign(other);
}

inline auto
Value::operator=(Value&& other) noexcept -> Value& {
    assign(std::move(other));
    return *this;
}

inline auto
Value::operator=(const Value& other) noexcept -> Value& {
    assign(other);
    return *this;
}

inline auto
Value::is_null() const noexcept -> bool {
    return (type() == NIL);
}

inline auto
Value::is_bool() const noexcept -> bool {
    return (type() == BOOLEAN);
}

inline auto
Value::is_number() const noexcept -> bool {
    return (type() == NUMBER);
}

inline auto
Value::is_string() const noexcept -> bool {
    return (type() == STRING);
}

inline auto
Value::is_array() const noexcept -> bool {
    return (type() == ARRAY);
}

inline auto
Value::is_object() const noexcept -> bool {
    return (type() == OBJECT);
}

inline auto
Value::as_null() const noexcept -> Null {
    return nullptr;
}

inline auto
Value::as_bool() const noexcept -> Bool {
    return m_boolean;
}

inline auto
Value::as_number() const noexcept -> const Number&  {
    return m_number;
}

inline auto
Value::as_string() const noexcept -> const String&  {
    return m_string;
}

inline auto
Value::as_array() const noexcept -> const Array&  {
    return m_array;
}

inline auto
Value::as_object() const noexcept -> const Array&  {
    return m_array;
}

inline
Value::operator Null() const noexcept {
    return nullptr;
}

inline
Value::operator const Number&() const noexcept {
    return m_number;
}

inline
Value::operator const String&() const noexcept {
    return m_string;
}

inline
Value::operator const Array&() const noexcept {
    return m_array;
}

inline
Value::operator const Object&() const noexcept {
    return m_object;
}

inline auto
Value::type() const noexcept -> Type {
    return m_type;
}

inline auto
Value::operator!() const noexcept -> bool {
    return !bool(*this);
}

}

#endif /* JSON_VALUE_HPP */
