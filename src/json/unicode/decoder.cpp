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
 * @file json/unicode/parser.cpp
 *
 * @brief JSON Unicode decoder implementation
 */

#include "json/unicode/decoder.hpp"

using json::unicode::Decoder;

static constexpr char32_t UTF8_BYTE1_MASK{0x7F};
static constexpr char32_t UTF8_BYTE2_MASK{0x1F};
static constexpr char32_t UTF8_BYTE3_MASK{0x0F};
static constexpr char32_t UTF8_BYTE4_MASK{0x07};
static constexpr char32_t UTF8_BYTEN_MASK{0x3F};

static constexpr char32_t UTF16_LOW_SURROGATE_MIN{0xDC00};
static constexpr char32_t UTF16_LOW_SURROGATE_MAX{0xDFFF};

static constexpr char32_t UTF16_HIGH_SURROGATE_MIN{0xD800};
static constexpr char32_t UTF16_HIGH_SURROGATE_MAX{0xDBFF};

static constexpr char32_t UNICODE_MAX{0x10FFF};

static inline bool is_utf8_1_byte(char32_t ch) noexcept {
    return ((ch & ~UTF8_BYTE1_MASK) == 0x00);
}

static inline bool is_utf8_2_byte(char32_t ch) noexcept {
    return ((ch & ~UTF8_BYTE2_MASK) == 0xC0);
}

static inline bool is_utf8_3_byte(char32_t ch) noexcept {
    return ((ch & ~UTF8_BYTE3_MASK) == 0xE0);
}

static inline bool is_utf8_4_byte(char32_t ch) noexcept {
    return ((ch & ~UTF8_BYTE4_MASK) == 0xF0);
}

static inline bool is_utf8_next(char32_t ch) noexcept {
    return ((ch & ~UTF8_BYTEN_MASK) == 0x80);
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

void Decoder::dummy_event_handler(void*, char32_t) noexcept { }

void Decoder::dummy_error_handler(void*, char32_t, Error) noexcept { }

Decoder::Decoder(void* ctx, EventHandler event_handler,
        ErrorHandler error_handler) noexcept :
    m_context{ctx},
    m_event_handler{event_handler ? event_handler : dummy_event_handler},
    m_error_handler{error_handler ? error_handler : dummy_error_handler}
{ }

void Decoder::set(Type decode_type) noexcept {
    switch (decode_type) {
    case UTF8:
        m_state = &Decoder::decode_utf8_code1;
        break;
    case UTF16:
        m_state = &Decoder::decode_utf16_code1;
        break;
    case UTF32:
        m_state = &Decoder::decode_utf32_code;
        break;
    default:
        break;
    }
}

void Decoder::decoded(char32_t ch) noexcept {
    if (is_unicode_range(ch)) {
        m_event_handler(m_context, ch);
    }
    else {
        m_error_handler(m_context, ch, INVALID_CODE);
    }
}

void Decoder::decode_utf8_code1(char32_t ch) noexcept {
    if (is_utf8_1_byte(ch)) {
        m_event_handler(m_context, ch);
    }
    else if (is_utf8_2_byte(ch)) {
        m_state = &Decoder::decode_utf8_code4;
        m_unicode = (ch & UTF8_BYTE2_MASK);
    }
    else if (is_utf8_3_byte(ch)) {
        m_state = &Decoder::decode_utf8_code3;
        m_unicode = (ch & UTF8_BYTE3_MASK);
    }
    else if (is_utf8_4_byte(ch)) {
        m_state = &Decoder::decode_utf8_code2;
        m_unicode = (ch & UTF8_BYTE4_MASK);
    }
    else {
        m_error_handler(m_context, ch, INVALID_UTF8_FIRST_CODE);
    }
}

void Decoder::decode_utf8_code2(char32_t ch) noexcept {
    if (is_utf8_next(ch)) {
        m_state = &Decoder::decode_utf8_code3;
        m_unicode = (m_unicode << 6) | (ch & UTF8_BYTEN_MASK);
    }
    else {
        m_state = &Decoder::decode_utf8_code1;
        m_error_handler(m_context, ch, INVALID_UTF8_NEXT_CODE);
    }
}

void Decoder::decode_utf8_code3(char32_t ch) noexcept {
    if (is_utf8_next(ch)) {
        m_state = &Decoder::decode_utf8_code4;
        m_unicode = (m_unicode << 6) | (ch & UTF8_BYTEN_MASK);
    }
    else {
        m_state = &Decoder::decode_utf8_code1;
        m_error_handler(m_context, ch, INVALID_UTF8_NEXT_CODE);
    }
}

void Decoder::decode_utf8_code4(char32_t ch) noexcept {
    m_state = &Decoder::decode_utf8_code1;

    if (is_utf8_next(ch)) {
        ch = (m_unicode << 6) | (ch & UTF8_BYTEN_MASK);
        decoded(ch);
    }
    else {
        m_error_handler(m_context, ch, INVALID_UTF8_NEXT_CODE);
    }
}

void Decoder::decode_utf16_code1(char32_t ch) noexcept {
    if (is_utf16_high_surrogate(ch)) {
        m_state = &Decoder::decode_utf16_code1;
        m_unicode = (ch - UTF16_HIGH_SURROGATE_MIN);
    }
    else {
        decoded(ch);
    }
}

void Decoder::decode_utf16_code2(char32_t ch) noexcept {
    m_state = &Decoder::decode_utf16_code1;

    if (is_utf16_low_surrogate(ch)) {
        ch = (m_unicode << 10) | (ch - UTF16_LOW_SURROGATE_MIN);
        decoded(ch);
    }
    else {
        m_error_handler(m_context, ch, INVALID_UTF16_SURROGATE);
    }
}

void Decoder::decode_utf32_code(char32_t ch) noexcept {
    decoded(ch);
}
