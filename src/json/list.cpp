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
 * @file json/list.cpp
 *
 * @brief Implementation
 */

#include "json/list.hpp"

#include <type_traits>

using json::List;

static_assert(std::is_standard_layout<List>::value,
        "json::List is not a standard layout");

static_assert(std::is_standard_layout<json::ListItem>::value,
        "json::ListItem is not a standard layout");

void List::push_back(ListItem& item) noexcept {
    item.prev = m_last;
    item.next = nullptr;

    if (m_last) {
        m_last->next = &item;
    }

    if (!m_first) {
        m_first = &item;
    }

    m_last = &item;
}

void List::pop_back() noexcept {
    if (m_last) {
        if (m_last->prev) {
            m_last->prev->next = nullptr;
        }
        else {
            m_first = nullptr;
        }

        auto item = m_last->prev;
        m_last->prev = nullptr;
        m_last = item;
    }
}

void List::push_front(ListItem& item) noexcept {
    item.prev = nullptr;
    item.next = m_first;

    if (m_first) {
        m_first->prev = &item;
    }

    if (!m_last) {
        m_last = &item;
    }

    m_first = &item;
}

void List::pop_front() noexcept {
    if (m_first) {
        if (m_first->next) {
            m_first->next->prev = nullptr;
        }
        else {
            m_last = nullptr;
        }

        auto item = m_first->next;
        m_first->next = nullptr;
        m_first = item;
    }
}

List::size_type List::size() const noexcept {
    size_type count = 0;

    for (auto it = m_first; it; it = it->next) {
        ++count;
    }

    return count;
}
