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
    Pair() noexcept = default;

    Pair(Pair&& other) noexcept = default;

    Pair(const Pair& other) noexcept = default;

    Pair(String&& str) noexcept;

    Pair(String&& str, Value&& val) noexcept;

    Pair(String&& str, const Value& val) noexcept;

    Pair(const String& str) noexcept;

    Pair(const String& str, Value&& val) noexcept;

    Pair(const String& str, const Value& val) noexcept;

    Pair(Value&& val) noexcept;

    Pair(const Value& val) noexcept;

    Pair& operator=(Value&& val) noexcept;

    Pair& operator=(const Value& val) noexcept;

    operator const String&() const noexcept;

    operator String&() noexcept;

    operator Value&() noexcept;

    operator const Value&() const noexcept;

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
Pair::Pair(String&& str) noexcept :
    m_name{std::move(str)}
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
Pair::Pair(const String& str) noexcept :
    m_name{str}
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

inline
Pair::Pair(const Value& val) noexcept :
    m_value{val}
{ }

inline
Pair::Pair(Value&& val) noexcept :
    m_value{std::move(val)}
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

inline
Pair::operator String&() noexcept {
    return m_name;
}

inline
Pair::operator const String&() const noexcept {
    return m_name;
}

inline
Pair::operator Value&() noexcept {
    return m_value;
}

inline
Pair::operator const Value&() const noexcept {
    return m_value;
}

}

#endif /* JSON_PAIR_HPP */
