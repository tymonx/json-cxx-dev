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
 * @file json/parser.hpp
 *
 * @brief JSON parser interface
 */

#ifndef JSON_PARSER_HPP
#define JSON_PARSER_HPP

#include "types.hpp"
#include "value.hpp"
#include "unicode/decoder.hpp"

namespace json {

class Parser {
public:
    Parser() noexcept;

    void put(char32_t ch) noexcept;
private:
    using StateHandler = void (Parser::*)(char32_t);

    static void unicode_decoded(void* ctx, char32_t ch) noexcept;

    static void unicode_error(void* ctx, char32_t ch,
            unicode::Decoder::Error error) noexcept;

    void state_error(char32_t ch) noexcept;

    void state_idle(char32_t ch) noexcept;

    void state_null_1(char32_t ch) noexcept;

    void state_null_2(char32_t ch) noexcept;

    void state_null_3(char32_t ch) noexcept;

    void state_true_1(char32_t ch) noexcept;

    void state_true_2(char32_t ch) noexcept;

    void state_true_3(char32_t ch) noexcept;

    void state_false_1(char32_t ch) noexcept;

    void state_false_2(char32_t ch) noexcept;

    void state_false_3(char32_t ch) noexcept;

    void state_false_4(char32_t ch) noexcept;

    void state_integral_first(char32_t ch) noexcept;

    void state_integral_second(char32_t ch) noexcept;

    void state_integral_next(char32_t ch) noexcept;

    void state_floating_integral_digit(char32_t ch) noexcept;

    void state_floating_dot(char32_t ch) noexcept;

    void state_floating_fractional_digit(char32_t ch) noexcept;

    void state_floating_exponent(char32_t ch) noexcept;

    void state_floating_exponent_sign(char32_t ch) noexcept;

    void state_floating_exponent_digit(char32_t ch) noexcept;

    void state_string_first(char32_t ch) noexcept;

    void state_string_next(char32_t ch) noexcept;

    unicode::Decoder m_decoder{this,
        unicode_decoded, unicode_error};
    StateHandler m_state{&Parser::state_idle};
    Value m_value{};
    bool m_is_negative{};
    union {
        Uint m_uint{};
        Double m_double;
    };
};

inline void
Parser::put(char32_t ch) noexcept {
    m_decoder.decode(ch);
}

}

#endif /* JSON_PARSER_HPP */
