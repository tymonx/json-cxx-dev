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
 * @file json/types.hpp
 *
 * @brief JSON types interface
 */

#ifndef JSON_TYPES_HPP
#define JSON_TYPES_HPP

#include <cstddef>
#include <cstdint>

namespace json {

using Char = char;
using Null = std::nullptr_t;
using Bool = bool;
using Size = std::size_t;
using Int = std::intmax_t;
using Uint = std::uintmax_t;
using Double = double;
using Difference = std::ptrdiff_t;

}

#endif /* JSON_TYPES_HPP */
