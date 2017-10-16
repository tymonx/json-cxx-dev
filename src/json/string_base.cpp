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
 * @file json/string_base.cpp
 *
 * @brief Implementation
 */

#include "json/string_base.hpp"

#include <type_traits>

using json::StringBase;

static_assert(std::is_standard_layout<StringBase>::value,
        "json::StringBase is not a standard layout");

StringBase::StringBase() noexcept :
    m_fields{Size(Unicode::UTF8), 0},
    m_data{nullptr}
{ }

StringBase::StringBase(Unicode code, void* ptr, Size count) noexcept :
    m_fields{Size(code), count},
    m_data{ptr}
{ }

StringBase::StringBase(StringBase&& other) noexcept :
    m_fields{other.m_fields},
    m_data{other.m_data}
{ }

StringBase::StringBase(const StringBase& other) noexcept :
    m_fields{other.m_fields},
    m_data{other.m_data}
{ }

auto StringBase::operator=(StringBase&& other) noexcept -> StringBase& {
    m_fields = other.m_fields;
    m_data = other.m_data;
    return *this;
}

auto StringBase::operator=(const StringBase& other) noexcept -> StringBase& {
    m_fields = other.m_fields;
    m_data = other.m_data;
    return *this;
}

StringBase& StringBase::operator=(void* ptr) noexcept {
    m_data = ptr;
    return *this;
}

StringBase& StringBase::operator=(Size count) noexcept {
    m_fields.m_size = count;
    return *this;
}

StringBase& StringBase::operator=(Unicode code) noexcept {
    m_fields.m_unicode = Size(code);
    return *this;
}

auto StringBase::size() const noexcept -> Size {
    return m_fields.m_size;
}

auto StringBase::unicode() const noexcept -> Unicode {
    return Unicode(m_fields.m_unicode);
}

StringBase::operator void*() noexcept {
    return m_data;
}

StringBase::operator const void*() const noexcept {
    return m_data;
}

StringBase::~StringBase() noexcept { }
