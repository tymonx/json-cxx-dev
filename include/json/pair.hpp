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
 * @file json/pair.hpp
 *
 * @brief JSON pair interface
 */

#ifndef JSON_PAIR_HPP
#define JSON_PAIR_HPP

#include "value.hpp"
#include "string.hpp"

namespace json {

class Pair {
public:
    Pair() noexcept;

    Pair(Pair&& other) noexcept = default;

    Pair(const Pair& other) noexcept = default;

    Pair(Pair&& other, Value* parent) noexcept;

    Pair(const Pair& other, Value* parent) noexcept;

    Pair(String&& str, Value&& val) noexcept;

    Pair(String&& str, const Value& val) noexcept;

    Pair(const String& str, Value&& val) noexcept;

    Pair(const String& str, const Value& val) noexcept;

    Pair& operator=(Value&& val) noexcept;

    Pair& operator=(const Value& val) noexcept;

    String& name() noexcept;

    const String& name() const noexcept;

    Value& value() noexcept;

    const Value& value() const noexcept;

    ~Pair() noexcept;
private:
    Value m_value{};
    String m_name{};
};

inline
Pair::Pair() noexcept
{ }

inline
Pair::Pair(Pair&& other, Value* parent) noexcept :
    m_value{std::move(other.m_value), parent},
    m_name{std::move(other.m_name)}
{ }

inline
Pair::Pair(const Pair& other, Value* parent) noexcept :
    m_value{other.m_value, parent},
    m_name{other.m_name}
{ }

inline
Pair::Pair(String&& str, Value&& val) noexcept :
    m_value{std::move(val)},
    m_name{std::move(str)}
{ }

inline
Pair::Pair(String&& str, const Value& val) noexcept :
    m_value{val},
    m_name{std::move(str)}
{ }

inline
Pair::Pair(const String& str, Value&& val) noexcept :
    m_value{std::move(val)},
    m_name{str}
{ }

inline
Pair::Pair(const String& str, const Value& val) noexcept :
    m_value{val},
    m_name{str}
{ }

inline auto
Pair::name() noexcept -> String& {
    return m_name;
}

inline auto
Pair::name() const noexcept -> const String& {
    return m_name;
}

inline auto
Pair::value() noexcept -> Value& {
    return m_value;
}

inline auto
Pair::value() const noexcept -> const Value& {
    return m_value;
}

inline auto
Pair::operator=(Value&& val) noexcept -> Pair& {
    m_value = std::move(val);
    return *this;
}

inline auto
Pair::operator=(const Value& val) noexcept -> Pair& {
    m_value = val;
    return *this;
}

}

#endif /* JSON_PAIR_HPP */
