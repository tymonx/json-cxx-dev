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
 * @file json/unicode/parser.hpp
 *
 * @brief JSON Unicode decoder interface
 */

#ifndef JSON_UNICODE_DECODER_HPP
#define JSON_UNICODE_DECODER_HPP

namespace json {
namespace unicode {

class Decoder {
public:
    enum Type {
        UTF8,
        UTF16,
        UTF32
    };

    enum Error {
        INVALID_UTF8_FIRST_CODE,
        INVALID_UTF8_NEXT_CODE,
        INVALID_UTF16_SURROGATE,
        INVALID_CODE
    };

    using EventHandler = void (*)(void* ctx, char32_t ch);
    using ErrorHandler = void (*)(void* ctx, char32_t ch, Error error);

    Decoder(void* ctx, EventHandler event_handler,
            ErrorHandler error_handler) noexcept;

    void set(Type value) noexcept;

    void decode(char ch) noexcept;

    void decode(char16_t ch) noexcept;

    void decode(char32_t ch) noexcept;
private:
    using StateHandler = void (Decoder::*)(char32_t ch);

    static void dummy_event_handler(void*, char32_t) noexcept;

    static void dummy_error_handler(void*, char32_t, Error) noexcept;

    void decode_utf8_code1(char32_t ch) noexcept;

    void decode_utf8_code2(char32_t ch) noexcept;

    void decode_utf8_code3(char32_t ch) noexcept;

    void decode_utf8_code4(char32_t ch) noexcept;

    void decode_utf16_code1(char32_t ch) noexcept;

    void decode_utf16_code2(char32_t ch) noexcept;

    void decode_utf32_code(char32_t ch) noexcept;

    void decoded(char32_t ch) noexcept;

    char32_t m_unicode{};
    void* m_context{nullptr};
    EventHandler m_event_handler{dummy_event_handler};
    ErrorHandler m_error_handler{dummy_error_handler};
    StateHandler m_state{&Decoder::decode_utf8_code1};
};

inline void
Decoder::decode(char ch) noexcept {
    (*this.*m_state)(char32_t(ch));
}

inline void
Decoder::decode(char16_t ch) noexcept {
    (*this.*m_state)(char32_t(ch));
}

inline void
Decoder::decode(char32_t ch) noexcept {
    (*this.*m_state)(ch);
}

}
}

#endif /* JSON_UNICODE_DECODER_HPP */
