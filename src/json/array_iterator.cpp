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
 * @file json/array_iterator.cpp
 *
 * @brief Implementation
 */

#include "json/array_iterator.hpp"

#include "array_item.hpp"

using json::ArrayIterator;

template<> auto
ArrayIterator<true>::operator->() noexcept -> pointer {
    return &reinterpret_cast<const ArrayItem*>(&*m_iterator)->value;
}
