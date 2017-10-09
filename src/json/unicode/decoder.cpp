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
 * @file json/unicode/parser.cpp
 *
 * @brief JSON Unicode decoder implementation
 */

#include "json/unicode/decoder.hpp"

#include "common.hpp"

using json::unicode::Decoder;

namespace json {
namespace unicode {

static inline bool is_utf8_1_byte(char32_t ch) noexcept {
    return ((ch & ~UTF8_1_BYTES_MASK) == UTF8_1_BYTES_CODE);
}

static inline bool is_utf8_2_byte(char32_t ch) noexcept {
    return ((ch & ~UTF8_2_BYTES_MASK) == UTF8_2_BYTES_CODE);
}

static inline bool is_utf8_3_byte(char32_t ch) noexcept {
    return ((ch & ~UTF8_3_BYTES_MASK) == UTF8_3_BYTES_CODE);
}

static inline bool is_utf8_4_byte(char32_t ch) noexcept {
    return ((ch & ~UTF8_4_BYTES_MASK) == UTF8_4_BYTES_CODE);
}

static inline bool is_utf8_next(char32_t ch) noexcept {
    return ((ch & ~UTF8_NEXT_BYTE_MASK) == UTF8_NEXT_BYTE_CODE);
}

static inline bool is_unicode_range(char32_t ch) noexcept {
    return ((ch < UTF16_HIGH_SURROGATE_MIN) || (ch > UTF16_LOW_SURROGATE_MAX))
        && (ch <= UNICODE_MAX);
}

static inline bool is_utf16_low_surrogate(char32_t ch) noexcept {
    return (ch >= UTF16_LOW_SURROGATE_MIN) && (ch <= UTF16_LOW_SURROGATE_MAX);
}

static inline bool is_utf16_high_surrogate(char32_t ch) noexcept {
    return (ch >= UTF16_HIGH_SURROGATE_MIN) && (ch <= UTF16_HIGH_SURROGATE_MAX);
}

}
}

Decoder::Observer::~Observer() noexcept { }

void Decoder::encoding(Encoding encoding_type) noexcept {
    switch (encoding_type) {
    case Encoding::UTF8:
        m_state = &Decoder::decode_utf8_code1;
        break;
    case Encoding::UTF16:
        m_state = &Decoder::decode_utf16_bom;
        break;
    case Encoding::UTF16_BE:
        m_state = &Decoder::decode_utf16_be_code1;
        break;
    case Encoding::UTF16_LE:
        m_state = &Decoder::decode_utf16_le_code1;
        break;
    case Encoding::UTF32:
        m_state = &Decoder::decode_utf32_bom;
        break;
    case Encoding::UTF32_BE:
        m_state = &Decoder::decode_utf32_be_code;
        break;
    case Encoding::UTF32_LE:
        m_state = &Decoder::decode_utf32_le_code;
        break;
    default:
        break;
    }
}

void Decoder::decoded(char32_t ch) noexcept {
    if (is_unicode_range(ch)) {
        m_observer.get().unicode_decoded(ch);
    }
    else {
        m_observer.get().unicode_decoded(ch, Error::INVALID_CODE);
    }
}

void Decoder::decode_utf8_code1(char32_t ch) noexcept {
    if (is_utf8_1_byte(ch)) {
        m_observer.get().unicode_decoded(ch);
    }
    else if (is_utf8_2_byte(ch)) {
        m_state = &Decoder::decode_utf8_code4;
        m_unicode = (ch & UTF8_2_BYTES_MASK);
    }
    else if (is_utf8_3_byte(ch)) {
        m_state = &Decoder::decode_utf8_code3;
        m_unicode = (ch & UTF8_3_BYTES_MASK);
    }
    else if (is_utf8_4_byte(ch)) {
        m_state = &Decoder::decode_utf8_code2;
        m_unicode = (ch & UTF8_4_BYTES_MASK);
    }
    else {
        m_observer.get().unicode_decoded(ch, Error::INVALID_UTF8_FIRST_CODE);
    }
}

void Decoder::decode_utf8_code2(char32_t ch) noexcept {
    if (is_utf8_next(ch)) {
        m_state = &Decoder::decode_utf8_code3;
        m_unicode <<= 6;
        m_unicode |= (ch & UTF8_NEXT_BYTE_MASK);
    }
    else {
        m_state = &Decoder::decode_utf8_code1;
        m_observer.get().unicode_decoded(ch, Error::INVALID_UTF8_NEXT_CODE);
    }
}

void Decoder::decode_utf8_code3(char32_t ch) noexcept {
    if (is_utf8_next(ch)) {
        m_state = &Decoder::decode_utf8_code4;
        m_unicode <<= 6;
        m_unicode |= (ch & UTF8_NEXT_BYTE_MASK);
    }
    else {
        m_state = &Decoder::decode_utf8_code1;
        m_observer.get().unicode_decoded(ch, Error::INVALID_UTF8_NEXT_CODE);
    }
}

void Decoder::decode_utf8_code4(char32_t ch) noexcept {
    m_state = &Decoder::decode_utf8_code1;

    if (is_utf8_next(ch)) {
        ch &= UTF8_NEXT_BYTE_MASK;
        ch |= (m_unicode << 6);
        decoded(ch);
    }
    else {
        m_observer.get().unicode_decoded(ch, Error::INVALID_UTF8_NEXT_CODE);
    }
}

void Decoder::decode_utf16_bom(char32_t ch) noexcept {
    if (UTF16_BOM_LE == ch) {
        m_state = &Decoder::decode_utf16_le_code1;
    }
    else {
        m_state = &Decoder::decode_utf16_be_code1;
        if (UTF16_BOM_BE != ch) {
            (*this.*m_state)(ch);
        }
    }
}

void Decoder::decode_utf16_be_code1(char32_t ch) noexcept {
    if (is_utf16_high_surrogate(ch)) {
        m_state = &Decoder::decode_utf16_be_code2;
        m_unicode = (ch - UTF16_HIGH_SURROGATE_MIN);
    }
    else {
        decoded(ch);
    }
}

void Decoder::decode_utf16_code2(char32_t ch) noexcept {
    if (is_utf16_low_surrogate(ch)) {
        ch -= UTF16_LOW_SURROGATE_MIN;
        ch |= (m_unicode << 10);
        ch += SUPPLEMENTARY_MULTILINGUAL_PLANE;
        decoded(ch);
    }
    else {
        m_observer.get().unicode_decoded(ch, Error::INVALID_UTF16_SURROGATE);
    }
}

void Decoder::decode_utf16_be_code2(char32_t ch) noexcept {
    m_state = &Decoder::decode_utf16_be_code1;
    decode_utf16_code2(ch);
}

void Decoder::decode_utf16_le_code1(char32_t ch) noexcept {
    decode_utf16_be_code1(utf16_swap(char16_t(ch)));

    if (m_state == &Decoder::decode_utf16_be_code2) {
        m_state = &Decoder::decode_utf16_le_code2;
    }
}

void Decoder::decode_utf16_le_code2(char32_t ch) noexcept {
    m_state = &Decoder::decode_utf16_le_code1;
    decode_utf16_code2(utf16_swap(char16_t(ch)));
}

void Decoder::decode_utf32_bom(char32_t ch) noexcept {
    if (UTF32_BOM_LE == ch) {
        m_state = &Decoder::decode_utf32_le_code;
    }
    else {
        m_state = &Decoder::decode_utf32_be_code;
        if (UTF32_BOM_BE != ch) {
            (*this.*m_state)(ch);
        }
    }
}

void Decoder::decode_utf32_be_code(char32_t ch) noexcept {
    decoded(ch);
}

void Decoder::decode_utf32_le_code(char32_t ch) noexcept {
    decoded(utf32_swap(ch));
}
