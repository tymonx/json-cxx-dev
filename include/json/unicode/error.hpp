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
 * @file json/unicode/error.hpp
 *
 * @brief JSON Unicode error interface
 */

#ifndef JSON_UNICODE_ERROR_HPP
#define JSON_UNICODE_ERROR_HPP

namespace json {
namespace unicode {

enum class Error {
    INVALID_UTF8_FIRST_CODE,
    INVALID_UTF8_NEXT_CODE,
    INVALID_UTF16_SURROGATE,
    INVALID_CODE
};

}
}

#endif /* JSON_UNICODE_ERROR_HPP */
