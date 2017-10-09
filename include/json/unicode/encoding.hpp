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
 * @file json/unicode/type.hpp
 *
 * @brief JSON Unicode type interface
 */

#ifndef JSON_UNICODE_ENCODING_HPP
#define JSON_UNICODE_ENCODING_HPP

namespace json {
namespace unicode {

enum class Encoding {
    UTF8,
    UTF16,
    UTF16_BE,
    UTF16_LE,
    UTF32,
    UTF32_BE,
    UTF32_LE
};

}
}

#endif /* JSON_UNICODE_ENCODING_HPP */
