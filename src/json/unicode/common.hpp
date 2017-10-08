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
 * @file json/unicode/common.hpp
 *
 * @brief JSON Unicode common interface
 */

#ifndef JSON_UNICODE_COMMON_HPP
#define JSON_UNICODE_COMMON_HPP

namespace json {
namespace unicode {

static constexpr char32_t UTF8_1_BYTES_CODE{0x00};
static constexpr char32_t UTF8_2_BYTES_CODE{0xC0};
static constexpr char32_t UTF8_3_BYTES_CODE{0xE0};
static constexpr char32_t UTF8_4_BYTES_CODE{0xF0};

static constexpr char32_t UTF8_1_BYTES_MASK{0x7F};
static constexpr char32_t UTF8_2_BYTES_MASK{0x1F};
static constexpr char32_t UTF8_3_BYTES_MASK{0x0F};
static constexpr char32_t UTF8_4_BYTES_MASK{0x07};

static constexpr char32_t UTF8_1_BYTES_MIN{0x0000};
static constexpr char32_t UTF8_1_BYTES_MAX{0x007F};

static constexpr char32_t UTF8_2_BYTES_MIN{0x0080};
static constexpr char32_t UTF8_2_BYTES_MAX{0x07FF};

static constexpr char32_t UTF8_3_BYTES_MIN{0x0800};
static constexpr char32_t UTF8_3_BYTES_MAX{0xFFFF};

static constexpr char32_t UTF8_4_BYTES_MIN{0x010000};
static constexpr char32_t UTF8_4_BYTES_MAX{0x10FFFF};

static constexpr char32_t UTF8_NEXT_BYTE_CODE{0x80};
static constexpr char32_t UTF8_NEXT_BYTE_MASK{0x3F};

static constexpr char32_t UTF16_LOW_SURROGATE_MIN{0xDC00};
static constexpr char32_t UTF16_LOW_SURROGATE_MAX{0xDFFF};
static constexpr char32_t UTF16_LOW_SURROGATE_MASK{0x3FF};

static constexpr char32_t UTF16_HIGH_SURROGATE_MIN{0xD800};
static constexpr char32_t UTF16_HIGH_SURROGATE_MAX{0xDBFF};
static constexpr char32_t UTF16_HIGH_SURROGATE_MASK{0x3FF};

static constexpr char32_t SUPPLEMENTARY_MULTILINGUAL_PLANE{0x10000};

static constexpr char32_t UNICODE_MAX{0x10FFFF};

}
}

#endif /* JSON_UNICODE_COMMON_HPP */
