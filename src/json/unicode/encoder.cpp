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
 * @file json/unicode/encoder.cpp
 *
 * @brief JSON Unicode encoder implementation
 */

#include "json/unicode/encoder.hpp"

#include "common.hpp"

using json::unicode::Encoder;

namespace json {
namespace unicode {

static inline bool is_utf16_surrogate(char32_t ch) noexcept {
    return (ch >= UTF16_HIGH_SURROGATE_MIN) &&
        (ch <= UTF16_LOW_SURROGATE_MAX);
}

static inline bool is_utf8_1_byte(char32_t ch) noexcept {
    return (ch <= UTF8_1_BYTES_MAX);
}

static inline bool is_utf8_2_bytes(char32_t ch) noexcept {
    return (ch <= UTF8_2_BYTES_MAX);
}

static inline bool is_utf8_3_bytes(char32_t ch) noexcept {
    return (ch <= UTF8_3_BYTES_MAX) && !is_utf16_surrogate(ch);
}

static inline bool is_utf8_4_bytes(char32_t ch) noexcept {
    return (ch >= UTF8_4_BYTES_MIN) && (ch <= UTF8_4_BYTES_MAX);
}

static inline bool is_utf16_range(char32_t ch) noexcept {
    return (ch <= UNICODE_MAX) && !is_utf16_surrogate(ch);
}

static inline bool is_utf32_range(char32_t ch) noexcept {
    return (ch <= UNICODE_MAX) && !is_utf16_surrogate(ch);
}

static inline bool is_base_multilingual_plane(char32_t ch) noexcept {
    return (ch < SUPPLEMENTARY_MULTILINGUAL_PLANE);
}

}
}

char32_t Encoder::unicode_big_endian(char32_t ch) noexcept {
    return ch;
}

char32_t Encoder::utf16_little_endian(char32_t ch) noexcept {
    return char16_t(char16_t(ch) << 8) | char16_t(char16_t(ch) >> 8);
}

char32_t Encoder::utf32_little_endian(char32_t ch) noexcept {
    return (ch << 24) | (ch >> 24) | ((ch >> 8) & 0x0000FF00) |
        ((ch << 8) & 0x00FF0000);
}

Encoder::Observer::~Observer() noexcept { }

void Encoder::encoding(Encoding encoding_type, ByteOrder byte_order) noexcept {
    switch (encoding_type) {
    case Encoding::UTF8:
        m_byte_order = unicode_big_endian;
        m_state = &Encoder::encode_utf8_code;
        break;
    case Encoding::UTF16:
        if (ByteOrder::BIG_ENDIAN_ORDER == byte_order) {
            m_byte_order = unicode_big_endian;
        }
        else {
            m_byte_order = utf16_little_endian;
        }
        m_state = &Encoder::encode_utf16_code;
        break;
    case Encoding::UTF16_BE:
        m_byte_order = unicode_big_endian;
        m_state = &Encoder::encode_utf16_code;
        break;
    case Encoding::UTF16_LE:
        m_byte_order = utf16_little_endian;
        m_state = &Encoder::encode_utf16_code;
        break;
    case Encoding::UTF32:
        if (ByteOrder::BIG_ENDIAN_ORDER == byte_order) {
            m_byte_order = unicode_big_endian;
        }
        else {
            m_byte_order = utf16_little_endian;
        }
        m_byte_order = unicode_big_endian;
        m_state = &Encoder::encode_utf32_code;
        break;
    case Encoding::UTF32_BE:
        m_byte_order = unicode_big_endian;
        m_state = &Encoder::encode_utf32_code;
        break;
    case Encoding::UTF32_LE:
        m_byte_order = utf32_little_endian;
        m_state = &Encoder::encode_utf32_code;
        break;
    default:
        break;
    }
}

void Encoder::insert_byte_order_mark() noexcept {
    write(UNICODE_BOM);
}

void Encoder::write(char32_t ch) noexcept {
    m_observer.get().unicode_encoded(m_byte_order(ch));
}

void Encoder::write(char32_t ch, Error error) noexcept {
    m_observer.get().unicode_encoded(m_byte_order(ch), error);
}

void Encoder::encode_utf8_code(char32_t ch) noexcept {
    if (is_utf8_1_byte(ch)) {
        write(ch);
    }
    else if (is_utf8_2_bytes(ch)) {
        write(UTF8_2_BYTES_CODE | (ch >> 6));
        write(UTF8_NEXT_BYTE_CODE | (ch & UTF8_NEXT_BYTE_MASK));
    }
    else if (is_utf8_3_bytes(ch)) {
        write(UTF8_3_BYTES_CODE | (ch >> 12));
        write(UTF8_NEXT_BYTE_CODE | ((ch >> 6) & UTF8_NEXT_BYTE_MASK));
        write(UTF8_NEXT_BYTE_CODE | (ch & UTF8_NEXT_BYTE_MASK));
    }
    else if (is_utf8_4_bytes(ch)) {
        write(UTF8_4_BYTES_CODE | (ch >> 18));
        write(UTF8_NEXT_BYTE_CODE | ((ch >> 12) & UTF8_NEXT_BYTE_MASK));
        write(UTF8_NEXT_BYTE_CODE | ((ch >> 6) & UTF8_NEXT_BYTE_MASK));
        write(UTF8_NEXT_BYTE_CODE | (ch & UTF8_NEXT_BYTE_MASK));
    }
    else {
        write(ch, Error::INVALID_CODE);
    }
}

void Encoder::encode_utf16_code(char32_t ch) noexcept {
    if (is_utf16_range(ch)) {
        if (is_base_multilingual_plane(ch)) {
            write(ch);
        }
        else {
            write(UTF16_HIGH_SURROGATE_MIN + ((ch >> 10) &
                        UTF16_HIGH_SURROGATE_MASK));

            write(UTF16_LOW_SURROGATE_MIN + (ch & UTF16_LOW_SURROGATE_MASK));
        }
    }
    else {
        write(ch, Error::INVALID_CODE);
    }
}

void Encoder::encode_utf32_code(char32_t ch) noexcept {
    if (is_utf32_range(ch)) {
        write(ch);
    }
    else {
        write(ch, Error::INVALID_CODE);
    }
}
