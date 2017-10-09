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
 * @file json/unicode/parser.hpp
 *
 * @brief JSON Unicode decoder interface
 */

#ifndef JSON_UNICODE_DECODER_HPP
#define JSON_UNICODE_DECODER_HPP

#include "error.hpp"
#include "encoding.hpp"

#include <functional>

namespace json {
namespace unicode {

class Decoder {
public:
    class Observer {
    public:
        virtual void unicode_decoded(char32_t ch) noexcept = 0;

        virtual void unicode_decoded(char32_t ch, Error error) noexcept = 0;

        virtual ~Observer() noexcept;
    };

    Decoder(Observer& observer) noexcept;

    void encoding(Encoding encoding_type) noexcept;

    void decode(char ch) noexcept;

    void decode(char16_t ch) noexcept;

    void decode(char32_t ch) noexcept;
private:
    using StateHandler = void (Decoder::*)(char32_t ch);

    void decode_utf8_code1(char32_t ch) noexcept;

    void decode_utf8_code2(char32_t ch) noexcept;

    void decode_utf8_code3(char32_t ch) noexcept;

    void decode_utf8_code4(char32_t ch) noexcept;

    void decode_utf16_bom(char32_t ch) noexcept;

    void decode_utf16_be_code1(char32_t ch) noexcept;

    void decode_utf16_be_code2(char32_t ch) noexcept;

    void decode_utf16_le_code1(char32_t ch) noexcept;

    void decode_utf16_le_code2(char32_t ch) noexcept;

    void decode_utf16_code2(char32_t ch) noexcept;

    void decode_utf32_bom(char32_t ch) noexcept;

    void decode_utf32_be_code(char32_t ch) noexcept;

    void decode_utf32_le_code(char32_t ch) noexcept;

    void decoded(char32_t ch) noexcept;

    std::reference_wrapper<Observer> m_observer;
    StateHandler m_state{&Decoder::decode_utf8_code1};
    char32_t m_unicode{};
};

inline
Decoder::Decoder(Observer& observer) noexcept :
    m_observer{observer},
    m_state{&Decoder::decode_utf8_code1}
{ }

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
