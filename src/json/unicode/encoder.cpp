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

Encoder::Observer::~Observer() noexcept { }

void Encoder::encoding(Encoding encoding_type) noexcept {
    switch (encoding_type) {
    case Encoding::UTF8:
        m_state = &Encoder::encode_utf8_code;
        break;
    case Encoding::UTF16:
        m_state = &Encoder::encode_utf16_code;
        break;
    case Encoding::UTF32:
        m_state = &Encoder::encode_utf32_bom;
        break;
    case Encoding::UTF32_BE:
        m_state = &Encoder::encode_utf32_be_code;
        break;
    case Encoding::UTF32_LE:
        m_state = &Encoder::encode_utf32_le_code;
        break;
    default:
        break;
    }
}

void Encoder::encode_utf8_code(char32_t ch) noexcept {
    if (is_utf8_1_byte(ch)) {
        m_observer.get().unicode_encoded(ch);
    }
    else if (is_utf8_2_bytes(ch)) {
        m_observer.get().unicode_encoded(UTF8_2_BYTES_CODE |
                ((ch >> 6) & UTF8_2_BYTES_MASK));

        m_observer.get().unicode_encoded(UTF8_NEXT_BYTE_CODE |
                (ch & UTF8_NEXT_BYTE_MASK));
    }
    else if (is_utf8_3_bytes(ch)) {
        m_observer.get().unicode_encoded(UTF8_3_BYTES_CODE |
                ((ch >> 12) & UTF8_3_BYTES_MASK));

        m_observer.get().unicode_encoded(UTF8_NEXT_BYTE_CODE |
                ((ch >> 6) & UTF8_NEXT_BYTE_MASK));

        m_observer.get().unicode_encoded(UTF8_NEXT_BYTE_CODE |
                (ch & UTF8_NEXT_BYTE_MASK));
    }
    else if (is_utf8_4_bytes(ch)) {
        m_observer.get().unicode_encoded(UTF8_4_BYTES_CODE |
                ((ch >> 18) & UTF8_4_BYTES_MASK));

        m_observer.get().unicode_encoded(UTF8_NEXT_BYTE_CODE |
                ((ch >> 12) & UTF8_NEXT_BYTE_MASK));

        m_observer.get().unicode_encoded(UTF8_NEXT_BYTE_CODE |
                ((ch >> 6) & UTF8_NEXT_BYTE_MASK));

        m_observer.get().unicode_encoded(UTF8_NEXT_BYTE_CODE |
                (ch & UTF8_NEXT_BYTE_MASK));
    }
    else {
        m_observer.get().unicode_encoded(ch, Error::INVALID_CODE);
    }
}

void Encoder::encode_utf16_code(char32_t ch) noexcept {
    if (is_utf16_range(ch)) {
        if (is_base_multilingual_plane(ch)) {
            m_observer.get().unicode_encoded(ch);
        }
        else {
            m_observer.get().unicode_encoded(UTF16_HIGH_SURROGATE_MIN +
                    ((ch >> 10) & UTF16_HIGH_SURROGATE_MASK));

            m_observer.get().unicode_encoded(UTF16_LOW_SURROGATE_MIN +
                    (ch & UTF16_LOW_SURROGATE_MASK));
        }
    }
    else {
        m_observer.get().unicode_encoded(ch, Error::INVALID_CODE);
    }
}

void Encoder::encode_utf32_bom(char32_t ch) noexcept {
}

void Encoder::encode_utf32_be_code(char32_t ch) noexcept {
    if (is_utf32_range(ch)) {
        m_observer.get().unicode_encoded(ch);
    }
    else {
        m_observer.get().unicode_encoded(ch, Error::INVALID_CODE);
    }
}

void Encoder::encode_utf32_le_code(char32_t ch) noexcept {
    if (is_utf32_range(ch)) {
        m_observer.get().unicode_encoded(ch);
    }
    else {
        m_observer.get().unicode_encoded(ch, Error::INVALID_CODE);
    }
}
