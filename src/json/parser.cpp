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
 *   list of conditions and the following disclaimer.
 *
 * @copright
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
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
 * @file json/parser.cpp
 *
 * @brief Implementation
 */

#include "json/parser.hpp"

using json::Parser;

Parser::Parser() noexcept { }

void Parser::unicode_decoded(void* ctx, char32_t ch) noexcept {
    auto parser = static_cast<Parser*>(ctx);

    (parser->*parser->m_state)(ch);
}

void Parser::unicode_error(void* ctx, char32_t ch,
        unicode::Decoder::Error /* error */) noexcept {
    auto parser = static_cast<Parser*>(ctx);

    parser->m_state = &Parser::state_error;
    (parser->*parser->m_state)(ch);
}

void Parser::state_error(char32_t /* ch */) noexcept {
}

void Parser::state_idle(char32_t ch) noexcept {
    switch (ch) {
    case ' ':
    case '\t':
    case '\r':
    case '\n':
        break;
    case '"':
        m_state = &Parser::state_string_first;
        break;
    case 'n':
        m_state = &Parser::state_null_1;
        break;
    case 't':
        m_state = &Parser::state_true_1;
        break;
    case 'f':
        m_state = &Parser::state_false_1;
        break;
    case '-':
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        state_integral_first(ch);
        break;
    case '[':
        break;
    case '{':
        break;
    default:
        m_state = &Parser::state_error;
        break;
    }
}

void Parser::state_null_1(char32_t ch) noexcept {
    if ('u' == ch) {
        m_state = &Parser::state_null_2;
    }
    else {
        m_state = &Parser::state_error;
    }
}

void Parser::state_null_2(char32_t ch) noexcept {
    if ('l' == ch) {
        m_state = &Parser::state_null_3;
    }
    else {
        m_state = &Parser::state_error;
    }
}

void Parser::state_null_3(char32_t ch) noexcept {
    if ('l' == ch) {
        m_state = &Parser::state_idle;
    }
    else {
        m_state = &Parser::state_error;
    }
}

void Parser::state_true_1(char32_t ch) noexcept {
    if ('r' == ch) {
        m_state = &Parser::state_true_2;
    }
    else {
        m_state = &Parser::state_error;
    }
}

void Parser::state_true_2(char32_t ch) noexcept {
    if ('u' == ch) {
        m_state = &Parser::state_true_3;
    }
    else {
        m_state = &Parser::state_error;
    }
}

void Parser::state_true_3(char32_t ch) noexcept {
    if ('e' == ch) {
        m_state = &Parser::state_idle;
    }
    else {
        m_state = &Parser::state_error;
    }
}

void Parser::state_false_1(char32_t ch) noexcept {
    if ('a' == ch) {
        m_state = &Parser::state_false_2;
    }
    else {
        m_state = &Parser::state_error;
    }
}

void Parser::state_false_2(char32_t ch) noexcept {
    if ('l' == ch) {
        m_state = &Parser::state_false_3;
    }
    else {
        m_state = &Parser::state_error;
    }
}

void Parser::state_false_3(char32_t ch) noexcept {
    if ('s' == ch) {
        m_state = &Parser::state_false_4;
    }
    else {
        m_state = &Parser::state_error;
    }
}

void Parser::state_false_4(char32_t ch) noexcept {
    if ('e' == ch) {
        m_state = &Parser::state_idle;
    }
    else {
        m_state = &Parser::state_error;
    }
}

void Parser::state_string_first(char32_t ch) noexcept {
    if ('\"' == ch) {
        m_state = &Parser::state_idle;
    }
    else {
        m_state = &Parser::state_string_next;
    }
}

void Parser::state_string_next(char32_t ch) noexcept {
    if ('\"' == ch) {
        m_state = &Parser::state_idle;
    }
    else {

    }
}

void Parser::state_integral_first(char32_t ch) noexcept {
    if ('-' == ch) {
        m_is_negative = true;
        m_state = &Parser::state_integral_second;
    }
    else if ('0' == ch) {
        m_is_negative = false;
        m_uint = 0;
        m_state = &Parser::state_floating_dot;
    }
    else {
        m_is_negative = false;
        m_uint = Uint(ch - '0');
        m_state = &Parser::state_integral_next;
    }
}

void Parser::state_integral_second(char32_t ch) noexcept {
    if ('0' == ch) {
        m_uint = 0;
        m_state = &Parser::state_floating_dot;
    }
    else if ((ch >= '1') && (ch <= '9')) {
        m_uint = Uint(ch - '0');
        m_state = &Parser::state_integral_next;
    }
    else {
        m_state = &Parser::state_error;
    }
}

void Parser::state_integral_next(char32_t ch) noexcept {
    if ((ch >= '0') && (ch <= '9')) {
        m_uint = (10 * m_uint) + Uint(ch - '0');
    }
    else if ('.' == ch) {
        m_double = Double(m_uint);
        m_state = &Parser::state_floating_fractional_digit;
    }
    else if (('e' == ch) || ('E' == ch)) {
        m_double = Double(m_uint);
    }
    else {

    }
}

void Parser::state_floating_integral_digit(char32_t ch) noexcept {
    if ((ch >= '0') && (ch <= '9')) {
        m_double = (10 * m_double) + Uint(ch - '0');
    }
    else if ('.' == ch) {
        m_state = &Parser::state_floating_fractional_digit;
    }
    else if (('e' == ch) || ('E' == ch)) {
        m_state = &Parser::state_floating_exponent_sign;
    }
    else {

    }
}

void Parser::state_floating_dot(char32_t ch) noexcept {
    if ('.' == ch) {
        m_double = Double(m_uint);
        m_state = &Parser::state_floating_fractional_digit;
    }
    else if (('e' == ch) || ('E' == ch)) {
        m_state = &Parser::state_floating_exponent_sign;
    }
    else {

    }
}

void Parser::state_floating_fractional_digit(char32_t ch) noexcept {
    if ((ch >= '0') && (ch <= '9')) {

    }
    else if (('e' == ch) || ('E' == ch)) {
        m_state = &Parser::state_floating_exponent_sign;
    }
    else {

    }
}

void Parser::state_floating_exponent(char32_t ch) noexcept {
    if (('e' == ch) || ('E' == ch)) {
        m_state = &Parser::state_floating_exponent_sign;
    }
    else {

    }
}

void Parser::state_floating_exponent_sign(char32_t ch) noexcept {
    if ('-' == ch) {
        m_state = &Parser::state_floating_exponent_digit;
    }
    else if ('+' == ch) {
        m_state = &Parser::state_floating_exponent_digit;
    }
    else if ((ch >= '0') && (ch <= '9')) {
        m_state = &Parser::state_floating_exponent_digit;
    }
    else {

    }
}

void Parser::state_floating_exponent_digit(char32_t ch) noexcept {
    if ((ch >= '0') && (ch <= '9')) {

    }
    else {

    }
}
