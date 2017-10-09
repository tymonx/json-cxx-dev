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
#include "byte_order.hpp"

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

    void encoding(Encoding encoding_type,
            ByteOrder byte_order = ByteOrder::BIG_ENDIAN_ORDER) noexcept;

    void insert_byte_order_mark() noexcept;

    void encode(char ch) noexcept;

    void encode(char16_t ch) noexcept;

    void encode(char32_t ch) noexcept;
private:
    using StateHandler = void (Encoder::*)(char32_t ch);
    using ByteOrderHandler = char32_t (*)(char32_t ch);

    static char32_t unicode_big_endian(char32_t ch) noexcept;

    static char32_t utf16_little_endian(char32_t ch) noexcept;

    static char32_t utf32_little_endian(char32_t ch) noexcept;

    void encode_utf8_code(char32_t ch) noexcept;

    void encode_utf16_code(char32_t ch) noexcept;

    void encode_utf32_code(char32_t ch) noexcept;

    void write(char32_t ch) noexcept;

    void write(char32_t ch, Error error) noexcept;

    std::reference_wrapper<Observer> m_observer;
    ByteOrderHandler m_byte_order{unicode_big_endian};
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
