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

#ifndef JSON_UNICODE_ENCODER_HPP
#define JSON_UNICODE_ENCODER_HPP

#include "error.hpp"
#include "encoding.hpp"

#include <functional>

namespace json {
namespace unicode {

class Encoder {
public:
    class Observer {
    public:
        virtual void unicode_encoded(char32_t ch) noexcept = 0;

        virtual void unicode_encoded(char32_t ch, Error error) noexcept = 0;

        virtual ~Observer() noexcept;
    };

    Encoder(Observer& observer) noexcept;

    void encoding(Encoding encoding_type) noexcept;

    void encode(char ch) noexcept;

    void encode(char16_t ch) noexcept;

    void encode(char32_t ch) noexcept;
private:
    using StateHandler = void (Encoder::*)(char32_t ch);

    void encode_utf8_code(char32_t ch) noexcept;

    void encode_utf16_code(char32_t ch) noexcept;

    void encode_utf32_bom(char32_t ch) noexcept;

    void encode_utf32_be_code(char32_t ch) noexcept;

    void encode_utf32_le_code(char32_t ch) noexcept;

    std::reference_wrapper<Observer> m_observer;
    StateHandler m_state{&Encoder::encode_utf8_code};
};

inline
Encoder::Encoder(Observer& observer) noexcept :
    m_observer{observer}
{ }

inline void
Encoder::encode(char ch) noexcept {
    (*this.*m_state)(char32_t(ch));
}

inline void
Encoder::encode(char16_t ch) noexcept {
    (*this.*m_state)(char32_t(ch));
}

inline void
Encoder::encode(char32_t ch) noexcept {
    (*this.*m_state)(ch);
}

}
}

#endif /* JSON_UNICODE_ENCODER_HPP */
