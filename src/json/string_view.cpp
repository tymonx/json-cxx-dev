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
 * @file json/string_view.cpp
 *
 * @brief Implementation
 */

#include "json/string_view.hpp"

#include <type_traits>

using json::StringView;

static_assert(std::is_standard_layout<StringView>::value,
        "json::StringView is not a standard layout");

StringView StringView::subspan(size_type pos, size_type count) noexcept {
    return Span::subspan(pos, count);
}
