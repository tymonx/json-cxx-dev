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
 * @file json/string_view.hpp
 *
 * @brief JSON string view interface
 */

#ifndef JSON_STRING_VIEW_HPP
#define JSON_STRING_VIEW_HPP

#include "span.hpp"
#include "types.hpp"

namespace json {

class StringView : public Span<const Char> {
public:
    using Span::Span;
    using Span::value_type;
    using Span::difference_type;
    using Span::pointer;
    using Span::const_pointer;
    using Span::reference;
    using Span::const_reference;
    using Span::iterator;
    using Span::const_iterator;
    using Span::reverse_iterator;
    using Span::const_reverse_iterator;

    StringView(const Span<const Char>& other) noexcept;

    StringView subspan(size_type pos, size_type count) noexcept;
};

inline
StringView::StringView(const Span<const char>& other) noexcept :
    Span{other}
{ }

}

#endif /* JSON_STRING_VIEW_HPP */
