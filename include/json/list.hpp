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
 *   span of conditions and the following disclaimer.
 *
 * @copright
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this span of conditions and the following disclaimer in the documentation
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
 * @file json/list.hpp
 *
 * @brief JSON list interface
 */

#ifndef JSON_LIST_HPP
#define JSON_LIST_HPP

#include "list_item.hpp"
#include "list_iterator.hpp"

#include <cstddef>

namespace json {

class List {
public:
    using value_type = ListItem;
    using size_type = std::size_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = ListIterator<false>;
    using const_iterator = ListIterator<true>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    List() noexcept = default;

    bool empty() const noexcept;

    void clear() noexcept;

    size_type size() const noexcept;

    void push_back(ListItem& item) noexcept;

    void pop_back() noexcept;

    void push_front(ListItem& item) noexcept;

    void pop_front() noexcept;

    reference front() noexcept;

    const_reference front() const noexcept;

    reference back() noexcept;

    const_reference back() const noexcept;

    iterator begin() noexcept;

    const_iterator begin() const noexcept;

    const_iterator cbegin() const noexcept;

    iterator end() noexcept;

    const_iterator end() const noexcept;

    const_iterator cend() const noexcept;

    reverse_iterator rbegin() noexcept;

    const_reverse_iterator rbegin() const noexcept;

    const_reverse_iterator crbegin() const noexcept;

    reverse_iterator rend() noexcept;

    const_reverse_iterator rend() const noexcept;

    const_reverse_iterator crend() const noexcept;
private:
    ListItem* m_first{nullptr};
    ListItem* m_last{nullptr};
};

inline bool
List::empty() const noexcept {
    return (nullptr == m_first);
}

inline void
List::clear() noexcept {
    m_first = nullptr;
    m_last = nullptr;
}

inline auto
List::front() noexcept -> reference {
    return *m_first;
}

inline auto
List::front() const noexcept -> const_reference {
    return *m_first;
}

inline auto
List::back() noexcept -> reference {
    return *m_last;
}

inline auto
List::back() const noexcept -> const_reference {
    return *m_last;
}

inline auto
List::begin() noexcept -> iterator {
    return m_first;
}

inline auto
List::begin() const noexcept -> const_iterator {
    return m_first;
}

inline auto
List::cbegin() const noexcept -> const_iterator {
    return m_first;
}

inline auto
List::end() noexcept -> iterator {
    return nullptr;
}

inline auto
List::end() const noexcept -> const_iterator {
    return nullptr;
}

inline auto
List::cend() const noexcept -> const_iterator {
    return nullptr;
}

inline auto
List::rbegin() noexcept -> reverse_iterator {
    return reverse_iterator(nullptr);
}

inline auto
List::rbegin() const noexcept -> const_reverse_iterator {
    return const_reverse_iterator(nullptr);
}

inline auto
List::crbegin() const noexcept -> const_reverse_iterator {
    return const_reverse_iterator(nullptr);
}

inline auto
List::rend() noexcept -> reverse_iterator {
    return reverse_iterator(m_last);
}

inline auto
List::rend() const noexcept -> const_reverse_iterator {
    return const_reverse_iterator(m_last);
}

inline auto
List::crend() const noexcept -> const_reverse_iterator {
    return const_reverse_iterator(m_last);
}

}

#endif /* JSON_LIST_HPP */
