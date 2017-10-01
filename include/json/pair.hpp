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

    Pair(String&& str) noexcept;

    Pair(String&& str, Value&& val) noexcept;

    Pair(String&& str, const Value& val) noexcept;

    Pair(const String& str) noexcept;

    Pair(const String& str, Value&& val) noexcept;

    Pair(const String& str, const Value& val) noexcept;

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
    String m_name{};
    Value m_value{};
};

inline
Pair::Pair(String&& str) noexcept :
    m_name{std::move(str)}
{ }

inline
Pair::Pair(String&& str, Value&& val) noexcept :
    m_name{std::move(str)},
    m_value{std::move(val)}
{ }

inline
Pair::Pair(String&& str, const Value& val) noexcept :
    m_name{std::move(str)},
    m_value{val}
{ }

inline
Pair::Pair(const String& str) noexcept :
    m_name{str}
{ }

inline
Pair::Pair(const String& str, Value&& val) noexcept :
    m_name{str},
    m_value{std::move(val)}
{ }

inline
Pair::Pair(const String& str, const Value& val) noexcept :
    m_name{str},
    m_value{val}
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
