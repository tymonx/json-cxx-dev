/*!
 * @copright
 * Copyright (c) 2017, Tymoteusz Blazejczyk
 * All rights reserved.
 *
 * @copright
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * @copright
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * @copright
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * @copright
 * * Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * @copright
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * @file json/list.cpp
 *
 * @brief Implementation
 */

#include "json/list.hpp"

#include <type_traits>

using json::List;

static_assert(std::is_standard_layout<List>(),
        "json::List is not a standard layout");

static_assert(std::is_standard_layout<json::ListItem>(),
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
