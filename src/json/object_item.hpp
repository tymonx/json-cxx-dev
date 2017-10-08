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
 * @file json/object_item.hpp
 *
 * @brief JSON object item interface
 */

#ifndef JSON_OBJECT_ITEM_HPP
#define JSON_OBJECT_ITEM_HPP

#include "json/pair.hpp"
#include "json/list_item.hpp"

namespace json {

struct ObjectItem {
    ListItem list{};
    Pair pair{};
};

}

#endif /* JSON_OBJECT_ITEM_HPP */
