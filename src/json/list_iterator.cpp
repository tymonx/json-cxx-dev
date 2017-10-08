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
 * @file json/list_iterator.cpp
 *
 * @brief Implementation
 */

#include "json/list_iterator.hpp"

using json::ListIterator;

static_assert(std::is_standard_layout<json::ListItem>::value,
        "json::ListItem is not a standard layout");

template<> auto
ListIterator<false>::operator+(difference_type n) const noexcept
        -> ListIterator {
    ListIterator<false> it{m_item};

    if (n > 0) {
        while (it && n--) {
            it = it->next;
        }
    }
    else if (n < 0) {
        while (it && n++) {
            it = it->prev;
        }
    }

    return it;
}

template<> auto
ListIterator<false>::operator-(difference_type n) const noexcept
        -> ListIterator {
    ListIterator<false> it{m_item};

    if (n > 0) {
        while (it && n--) {
            it = it->prev;
        }
    }
    else if (n < 0) {
        while (it && n++) {
            it = it->next;
        }
    }

    return it;
}
