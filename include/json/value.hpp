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
#include "value_iterator.hpp"

#include <utility>

namespace json {

class Value {
public:
    using value_type = Value;
    using size_type = Size;
    using reference = value_type&;
    using allocator_type = Allocator;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = ValueIterator<false>;
    using const_iterator = ValueIterator<true>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    enum Type {
        NIL,
        BOOLEAN,
        NUMBER,
        STRING,
        ARRAY,
        OBJECT
    };

    Value() noexcept;

    Value(Type value,
            allocator_type& alloc = Allocator::get_instance()) noexcept;

    Value(Null value) noexcept;

    Value(Bool value) noexcept;

    Value(String&& string) noexcept;

    Value(const String& string) noexcept;

    Value(Number&& number) noexcept;

    Value(const Number& number) noexcept;

    Value(Array&& array) noexcept;

    Value(const Array& array) noexcept;

    Value(Object&& object) noexcept;

    Value(const Object& object) noexcept;

    Value(Value&& other) noexcept;

    Value(Value&& other, pointer parent_ptr) noexcept;

    Value(const Value& other) noexcept;

    Value(const Value& other, pointer parent_ptr) noexcept;

    Value& operator=(Value&& value) noexcept;

    Value& operator=(const Value& value) noexcept;

    void assign(Value&& value) noexcept;

    void assign(const Value& value) noexcept;

    Type type() const noexcept;

    bool is_null() const noexcept;

    bool is_bool() const noexcept;

    bool is_number() const noexcept;

    bool is_string() const noexcept;

    bool is_array() const noexcept;

    bool is_object() const noexcept;

    bool is_array_item() const noexcept;

    bool is_object_item() const noexcept;

    Null as_null() const noexcept;

    Bool as_bool() const noexcept;

    const Number& as_number() const noexcept;

    const String& as_string() const noexcept;

    const Array& as_array() const noexcept;

    const Object& as_object() const noexcept;

    bool operator!() const noexcept;

    operator Null() const noexcept;

    operator Bool() const noexcept;

    operator const Number&() const noexcept;

    operator const String&() const noexcept;

    operator const Array&() const noexcept;

    operator const Object&() const noexcept;

    pointer parent() const noexcept;

    pointer root() const noexcept;

    bool is_root() const noexcept;

    void push_back(const value_type& value) noexcept;

    void push_back(value_type&& value) noexcept;

    template<typename... Args>
    void emplace_back(Args&&... args) noexcept;

    void pop_back() noexcept;

    iterator begin() noexcept;

    const_iterator begin() const noexcept;

    const_iterator cbegin() const noexcept;

    iterator end() noexcept;

    const_iterator end() const noexcept;

    const_iterator cend() const noexcept;

    reverse_iterator rbegin() noexcept;

    const_reverse_iterator rbegin() const noexcept;

    const_reverse_iterator crbegin() const noexcept;

    reverse_iterator rend() noexcept;

    const_reverse_iterator rend() const noexcept;

    const_reverse_iterator crend() const noexcept;

    ~Value() noexcept;
private:
    void destroy() noexcept;

    Type m_type{NIL};
    Value* m_parent{nullptr};

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
Value::Value(Null) noexcept
{ }

inline
Value::Value(Bool value) noexcept :
    m_type{BOOLEAN},
    m_boolean{value}
{ }

inline
Value::Value(Number&& number) noexcept :
    m_type{NUMBER},
    m_number{std::move(number)}
{ }

inline
Value::Value(const Number& number) noexcept :
    m_type{NUMBER},
    m_number{number}
{ }

inline
Value::Value(String&& string) noexcept :
    m_type{STRING},
    m_string{std::move(string)}
{ }

inline
Value::Value(const String& string) noexcept :
    m_type{STRING},
    m_string{string}
{ }

inline
Value::Value(Array&& array) noexcept :
    m_type{ARRAY},
    m_array{std::move(array)}
{ }

inline
Value::Value(const Array& array) noexcept :
    m_type{ARRAY},
    m_array{array}
{ }

inline
Value::Value(Object&& object) noexcept :
    m_type{OBJECT},
    m_object{std::move(object)}
{ }

inline
Value::Value(const Object& object) noexcept :
    m_type{OBJECT},
    m_object{object}
{ }

inline
Value::Value(Value&& other) noexcept {
    assign(std::move(other));
}

inline
Value::Value(Value&& other, pointer parent_ptr) noexcept {
    assign(std::move(other));
    m_parent = parent_ptr;
}

inline
Value::Value(const Value& other) noexcept {
    assign(other);
}

inline
Value::Value(const Value& other, pointer parent_ptr) noexcept {
    assign(other);
    m_parent = parent_ptr;
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
Value::is_array_item() const noexcept -> bool {
    return m_parent ? m_parent->is_array() : false;
}

inline auto
Value::is_object_item() const noexcept -> bool {
    return m_parent ? m_parent->is_object() : false;
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
Value::as_object() const noexcept -> const Object&  {
    return m_object;
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

inline auto
Value::parent() const noexcept -> pointer {
    return m_parent;
}

inline bool
Value::is_root() const noexcept {
    return nullptr == m_parent;
}

template<typename... Args> void
Value::emplace_back(Args&&... args) noexcept {
    push_back({std::forward<Args>(args)...});
}

inline auto
Value::begin() const noexcept -> const_iterator {
    return const_cast<Value*>(this)->begin();
}

inline auto
Value::cbegin() const noexcept -> const_iterator {
    return const_cast<Value*>(this)->begin();
}

inline auto
Value::end() noexcept -> iterator {
    return {};
}

inline auto
Value::end() const noexcept -> const_iterator {
    return {};
}

inline auto
Value::cend() const noexcept -> const_iterator {
    return {};
}

inline auto
Value::rbegin() const noexcept -> const_reverse_iterator {
    return const_cast<Value*>(this)->rbegin();
}

inline auto
Value::crbegin() const noexcept -> const_reverse_iterator {
    return const_cast<Value*>(this)->rbegin();
}

inline auto
Value::rend() noexcept -> reverse_iterator {
    return {};
}

inline auto
Value::rend() const noexcept -> const_reverse_iterator {
    return {};
}

inline auto
Value::crend() const noexcept -> const_reverse_iterator {
    return {};
}

}

#endif /* JSON_VALUE_HPP */
