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
 * @file json/string_iterator.cpp
 *
 * @brief Implementation
 */

#include "json/string_iterator.hpp"

using json::StringIterator;

static constexpr std::uint8_t UTF8_1_BYTE_MASK{0x80};
static constexpr std::uint8_t UTF8_1_BYTE_CODE{0x00};

static constexpr std::uint8_t UTF8_2_BYTES_MASK{0xE0};
static constexpr std::uint8_t UTF8_2_BYTES_CODE{0xC0};

static constexpr std::uint8_t UTF8_3_BYTES_MASK{0xF0};
static constexpr std::uint8_t UTF8_3_BYTES_CODE{0xE0};

static constexpr std::uint8_t UTF8_4_BYTES_MASK{0xF8};
static constexpr std::uint8_t UTF8_4_BYTES_CODE{0xF0};

static constexpr std::uint8_t UTF8_NEXT_BYTE_MASK{0xC0};
static constexpr std::uint8_t UTF8_NEXT_BYTE_CODE{0x80};

static constexpr std::uint16_t UTF16_HIGH_SURROGATE_MASK{0xFC00};
static constexpr std::uint16_t UTF16_HIGH_SURROGATE_CODE{0xD800};

static constexpr std::uint16_t UTF16_LOW_SURROGATE_MASK{0xFC00};
static constexpr std::uint16_t UTF16_LOW_SURROGATE_CODE{0xDC00};

static constexpr std::uint32_t SURROGATE_MASK{0xFFFFF800};
static constexpr std::uint32_t SURROGATE_CODE{0x0000D800};

static constexpr std::uint32_t REPLACEMENT_CHARACTER{0xFFFD};
static constexpr std::uint32_t UNICODE_MAX{0x10FFFF};

static constexpr std::uint32_t SMP_BASE{0x100000};

static const std::uint8_t* utf8(const void* data) noexcept {
    return reinterpret_cast<const std::uint8_t*>(data);
}

static bool is_utf8_next_byte(std::uint8_t ch) noexcept {
    return ((ch & UTF8_NEXT_BYTE_MASK) == UTF8_NEXT_BYTE_CODE);
}

static bool is_utf8_1_byte(const std::uint8_t* code) noexcept {
    return ((code[0] & UTF8_1_BYTE_MASK) == UTF8_1_BYTE_CODE);
}

static bool is_utf8_2_bytes(const std::uint8_t* code) noexcept {
    return ((code[0] & UTF8_2_BYTES_MASK) == UTF8_2_BYTES_CODE) &&
        is_utf8_next_byte(code[1]);
}

static bool is_utf8_3_bytes(const std::uint8_t* code) noexcept {
    return ((code[0] & UTF8_3_BYTES_MASK) == UTF8_3_BYTES_CODE) &&
        is_utf8_next_byte(code[1]) &&
        is_utf8_next_byte(code[2]);
}

static bool is_utf8_4_bytes(const std::uint8_t* code) noexcept {
    return ((code[0] & UTF8_4_BYTES_MASK) == UTF8_4_BYTES_CODE) &&
        is_utf8_next_byte(code[1]) &&
        is_utf8_next_byte(code[2]) &&
        is_utf8_next_byte(code[3]);
}

static std::uint32_t decode_utf8_1_byte(const std::uint8_t* code) noexcept {
    return std::uint32_t(code[0]);
}

static std::uint32_t decode_utf8_2_bytes(const std::uint8_t* code) noexcept {
    return std::uint32_t((~UTF8_2_BYTES_MASK & code[0]) << 6) |
        std::uint32_t(~UTF8_NEXT_BYTE_MASK & code[1]);
}

static std::uint32_t decode_utf8_3_bytes(const std::uint8_t* code) noexcept {
    return std::uint32_t((~UTF8_3_BYTES_MASK & code[0]) << 12) |
        std::uint32_t((~UTF8_NEXT_BYTE_MASK & code[1]) << 6) |
        std::uint32_t(~UTF8_NEXT_BYTE_MASK & code[2]);
}

static std::uint32_t decode_utf8_4_bytes(const std::uint8_t* code) noexcept {
    return std::uint32_t((~UTF8_4_BYTES_MASK & code[0]) << 18) |
        std::uint32_t((~UTF8_NEXT_BYTE_MASK & code[1]) << 12) |
        std::uint32_t((~UTF8_NEXT_BYTE_MASK & code[2]) << 6) |
        std::uint32_t(~UTF8_NEXT_BYTE_MASK & code[3]);
}

static const std::uint16_t* utf16(const void* data) noexcept {
    return reinterpret_cast<const std::uint16_t*>(data);
}

static std::uint16_t utf16_swap(std::uint16_t ch) noexcept {
    return std::uint16_t((ch << 8) | (ch >> 8));
}

static bool is_utf16_high_surrogate(std::uint16_t ch) noexcept {
    return ((ch & UTF16_HIGH_SURROGATE_MASK) == UTF16_HIGH_SURROGATE_CODE);
}

static bool is_utf16_low_surrogate(std::uint16_t ch) noexcept {
    return ((ch & UTF16_LOW_SURROGATE_MASK) == UTF16_LOW_SURROGATE_CODE);
}

static bool is_utf16_surrogate(const std::uint16_t* code) noexcept {
    return is_utf16_high_surrogate(code[0]) && is_utf16_low_surrogate(code[1]);
}

static bool is_utf16le_surrogate(const std::uint16_t* code) noexcept {
    return is_utf16_high_surrogate(utf16_swap(code[0])) &&
        is_utf16_low_surrogate(utf16_swap(code[1]));
}

static std::uint32_t decode_utf16_surrogate(
        const std::uint16_t* code) noexcept {
    return SMP_BASE | std::uint32_t((code[0] -
                UTF16_HIGH_SURROGATE_CODE) << 10) |
        std::uint32_t(code[1] - UTF16_LOW_SURROGATE_CODE);
}

static std::uint32_t decode_utf16le_surrogate(
        const std::uint16_t* code) noexcept {
    return SMP_BASE | ((std::uint32_t(utf16_swap(code[0])) -
                UTF16_HIGH_SURROGATE_CODE) << 10) |
        (std::uint32_t(utf16_swap(code[1])) - UTF16_LOW_SURROGATE_CODE);
}

static const std::uint32_t* utf32(const void* data) noexcept {
    return reinterpret_cast<const std::uint32_t*>(data);
}

static std::uint32_t utf32_swap(std::uint32_t ch) noexcept {
    return (ch << 24) | (ch >> 24) |
        ((ch << 8) & 0xFF0000) | ((ch >> 8) & 0xFF00);
}

static bool is_valid(std::uint32_t ch) noexcept {
    return ((ch & SURROGATE_MASK) != SURROGATE_CODE) && (ch < UNICODE_MAX);
}

StringIterator::StringIterator() noexcept :
    m_unicode{Unicode::UTF8},
    m_data{nullptr}
{ }

StringIterator::StringIterator(Unicode unicode, const void* data) noexcept :
    m_unicode{unicode},
    m_data{data}
{ }

StringIterator::StringIterator(StringIterator&& other) noexcept :
    m_unicode{other.m_unicode},
    m_data{other.m_data}
{ }

StringIterator::StringIterator(const StringIterator& other) noexcept :
    m_unicode{other.m_unicode},
    m_data{other.m_data}
{ }

auto StringIterator::operator=(
        StringIterator&& other) noexcept -> StringIterator& {
    m_unicode = other.m_unicode;
    m_data = other.m_data;
    return *this;
}

auto StringIterator::operator=(
        const StringIterator& other) noexcept -> StringIterator& {
    m_unicode = other.m_unicode;
    m_data = other.m_data;
    return *this;
}

auto StringIterator::operator++() noexcept -> StringIterator& {
    switch (m_unicode) {
    case Unicode::UTF8:
        if (is_utf8_1_byte(utf8(m_data))) {
            m_data = utf8(m_data) + 1;
        }
        else if (is_utf8_2_bytes(utf8(m_data))) {
            m_data = utf8(m_data) + 2;
        }
        else if (is_utf8_3_bytes(utf8(m_data))) {
            m_data = utf8(m_data) + 3;
        }
        else if (is_utf8_4_bytes(utf8(m_data))) {
            m_data = utf8(m_data) + 4;
        }
        else {
            m_data = utf8(m_data) + 1;
        }

        break;
    case Unicode::UTF16:
    case Unicode::UTF16BE:
        if (is_utf16_surrogate(utf16(m_data))) {
            m_data = utf16(m_data) + 2;
        }
        else {
            m_data = utf16(m_data) + 1;
        }

        break;
    case Unicode::UTF16LE:
        if (is_utf16le_surrogate(utf16(m_data))) {
            m_data = utf16(m_data) + 2;
        }
        else {
            m_data = utf16(m_data) + 1;
        }

        break;
    case Unicode::UTF32:
    case Unicode::UTF32BE:
    case Unicode::UTF32LE:
        m_data = utf32(m_data) + 1;
        break;
    default:
        break;
    }

    return *this;
}

auto StringIterator::operator++(int) noexcept -> StringIterator {
    StringIterator tmp{*this};
    ++(*this);
    return tmp;
}

auto StringIterator::operator--() noexcept -> StringIterator& {
    switch (m_unicode) {
    case Unicode::UTF8:
        if (is_utf8_1_byte(utf8(m_data) - 1)) {
            m_data = utf8(m_data) - 1;
        }
        else if (is_utf8_2_bytes(utf8(m_data) - 2)) {
            m_data = utf8(m_data) - 2;
        }
        else if (is_utf8_3_bytes(utf8(m_data) - 3)) {
            m_data = utf8(m_data) - 3;
        }
        else if (is_utf8_4_bytes(utf8(m_data) - 4)) {
            m_data = utf8(m_data) - 4;
        }
        else {
            m_data = utf8(m_data) - 1;
        }

        break;
    case Unicode::UTF16:
    case Unicode::UTF16BE:
        if (is_utf16_surrogate(utf16(m_data) - 2)) {
            m_data = utf16(m_data) - 2;
        }
        else {
            m_data = utf16(m_data) - 1;
        }

        break;
    case Unicode::UTF16LE:
        if (is_utf16le_surrogate(utf16(m_data) - 2)) {
            m_data = utf16(m_data) - 2;
        }
        else {
            m_data = utf16(m_data) - 1;
        }

        break;
    case Unicode::UTF32:
    case Unicode::UTF32BE:
    case Unicode::UTF32LE:
        m_data = utf32(m_data) - 1;
        break;
    default:
        break;
    }

    return *this;
}

auto StringIterator::operator--(int) noexcept -> StringIterator {
    StringIterator tmp{*this};
    --(*this);
    return tmp;
}

auto StringIterator::operator+(
        difference_type n) const noexcept -> StringIterator {
    StringIterator it{*this};

    if (n > 0) {
        while (n--) {
            ++it;
        }
    }
    else if (n < 0) {
         while (n++) {
            --it;
        }
    }

    return it;
}

auto StringIterator::operator+=(difference_type n) noexcept -> StringIterator& {
    return (*this = (*this + n));
}

auto StringIterator::operator-(
        difference_type n) const noexcept -> StringIterator {
    return (*this) + (-n);
}

auto StringIterator::operator-=(difference_type n) noexcept -> StringIterator& {
    return (*this) += (-n);
}

auto StringIterator::operator[](
        difference_type n) const noexcept -> value_type {
    return *((*this) + n);
}

auto StringIterator::operator*() const noexcept -> value_type {
    value_type value;

    switch (m_unicode) {
    case Unicode::UTF8:
        if (is_utf8_1_byte(utf8(m_data))) {
            value = decode_utf8_1_byte(utf8(m_data));
        }
        else if (is_utf8_2_bytes(utf8(m_data))) {
            value = decode_utf8_2_bytes(utf8(m_data));
        }
        else if (is_utf8_3_bytes(utf8(m_data))) {
            value = decode_utf8_3_bytes(utf8(m_data));
        }
        else if (is_utf8_4_bytes(utf8(m_data))) {
            value = decode_utf8_4_bytes(utf8(m_data));
        }
        else {
            value = REPLACEMENT_CHARACTER;
        }

        break;
    case Unicode::UTF16:
    case Unicode::UTF16BE:
        if (is_utf16_surrogate(utf16(m_data))) {
            value = decode_utf16_surrogate(utf16(m_data));
        }
        else {
            value = *utf16(m_data);
        }

        break;
    case Unicode::UTF16LE:
        if (is_utf16le_surrogate(utf16(m_data))) {
            value = decode_utf16le_surrogate(utf16(m_data));
        }
        else {
            value = utf16_swap(*utf16(m_data));
        }

        break;
    case Unicode::UTF32:
    case Unicode::UTF32BE:
        value = *utf32(m_data);
        break;
    case Unicode::UTF32LE:
        value = utf32_swap(*utf32(m_data));
        break;
    default:
        value = REPLACEMENT_CHARACTER;
        break;
    }

    if (!is_valid(value)) {
        value = REPLACEMENT_CHARACTER;
    }

    return value;
}

auto StringIterator::base() const noexcept -> const void* {
    return m_data;
}

StringIterator::operator bool() const noexcept {
    return (nullptr != m_data);
}

StringIterator::operator Unicode() const noexcept {
    return m_unicode;
}

bool StringIterator::operator<(const StringIterator& other) const noexcept {
    return base() < other.base();
}

bool StringIterator::operator<=(const StringIterator& other) const noexcept {
    return base() <= other.base();
}

bool StringIterator::operator>(const StringIterator& other) const noexcept {
    return base() > other.base();
}

bool StringIterator::operator>=(const StringIterator& other) const noexcept {
    return base() >= other.base();
}

bool StringIterator::operator==(const StringIterator& other) const noexcept {
    return base() == other.base();
}

bool StringIterator::operator!=(const StringIterator& other) const noexcept {
    return base() != other.base();
}

bool StringIterator::operator==(Unicode unicode) const noexcept {
    return m_unicode == unicode;
}

bool StringIterator::operator!=(Unicode unicode) const noexcept {
    return m_unicode != unicode;
}

bool json::operator==(Unicode unicode, const StringIterator& it) noexcept {
    return (it == unicode);
}

bool json::operator!=(Unicode unicode, const StringIterator& it) noexcept {
    return (it != unicode);
}

StringIterator::~StringIterator() noexcept { }
