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
 * @file json/utf8_iterator.cpp
 *
 * @brief Implementation
 */

#include "json/utf8_iterator.hpp"

using json::UTF8Iterator;

static constexpr char32_t BYTE_2_MASK{0x1F};
static constexpr char32_t BYTE_2_CODE{0xC0};

static constexpr char32_t BYTE_3_MASK{0x0F};
static constexpr char32_t BYTE_3_CODE{0xE0};

static constexpr char32_t BYTE_4_MASK{0x07};
static constexpr char32_t BYTE_4_CODE{0xF0};

static bool is_1_byte(char32_t ch) noexcept {
    return (ch < 0x80);
}

static bool is_2_bytes(char32_t ch) noexcept {
    return (ch & ~BYTE_2_MASK) == BYTE_2_CODE;
}

static bool is_3_bytes(char32_t ch) noexcept {
    return (ch & ~BYTE_3_MASK) == BYTE_3_CODE;
}

static bool is_4_bytes(char32_t ch) noexcept {
    return (ch & ~BYTE_4_MASK) == BYTE_4_CODE;
}

UTF8Iterator::UTF8Iterator() noexcept :
    m_data{nullptr}
{ }

UTF8Iterator::UTF8Iterator(char* ptr) noexcept :
    m_data{ptr}
{ }

UTF8Iterator::UTF8Iterator(UTF8Iterator&& other) noexcept :
    m_data{other.m_data}
{
    other.m_data = nullptr;
}

UTF8Iterator::UTF8Iterator(const UTF8Iterator& other) noexcept :
    m_data{other.m_data}
{ }

auto UTF8Iterator::operator=(UTF8Iterator&& other) noexcept -> UTF8Iterator& {
    if (this != &other) {
        m_data = other.m_data;
        other.m_data = nullptr;
    }
    return *this;
}

auto UTF8Iterator::operator=(
        const UTF8Iterator& other) noexcept -> UTF8Iterator& {
    m_data = other.m_data;
    return *this;
}

auto UTF8Iterator::operator++() noexcept -> UTF8Iterator& {
    auto ch = char32_t(*m_data);

    if (is_1_byte(ch)) {
        ++m_data;
    }
    else if (is_2_bytes(ch)) {
        m_data += 2;
    }
    else if (is_3_bytes(ch)) {
        m_data += 3;
    }
    else if (is_4_bytes(ch)) {
        m_data += 4;
    }

    return *this;
}

UTF8Iterator operator++(int) noexcept;

UTF8Iterator& operator--() noexcept;

UTF8Iterator operator--(int) noexcept;

UTF8Iterator operator+(difference_type n) const noexcept;

UTF8Iterator& operator+=(difference_type n) noexcept;

UTF8Iterator operator-(difference_type n) const noexcept;

UTF8Iterator& operator-=(difference_type n) noexcept;

value_type operator[](difference_type n) const noexcept;

value_type operator*() const noexcept;

operator bool() const noexcept;

~UTF8Iterator() noexcept;

