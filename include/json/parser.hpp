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
 * @file json/parser.hpp
 *
 * @brief JSON parser interface
 */

#ifndef JSON_PARSER_HPP
#define JSON_PARSER_HPP

#include "types.hpp"
#include "value.hpp"
#include "unicode/decoder.hpp"
#include "unicode/encoder.hpp"

namespace json {

class Parser final : private unicode::Decoder::Observer,
   private unicode::Encoder::Observer {
public:
    Parser() noexcept;

    void put(char32_t ch) noexcept;

    virtual ~Parser() noexcept override;
private:
    using StateHandler = void (Parser::*)(char32_t);

    virtual void unicode_decoded(char32_t ch) noexcept override;

    virtual void unicode_decoded(char32_t ch,
            unicode::Error error) noexcept override;

    virtual void unicode_encoded(char32_t ch) noexcept override;

    virtual void unicode_encoded(char32_t ch,
            unicode::Error error) noexcept override;

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

    unicode::Decoder m_decoder{*this};
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
