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
 * @file json/list.hpp
 *
 * @brief JSON list interface
 */

#ifndef JSON_LIST_HPP
#define JSON_LIST_HPP

#include "types.hpp"
#include "list_item.hpp"
#include "list_iterator.hpp"

#include <cstddef>

namespace json {

class List {
public:
    using value_type = ListItem;
    using size_type = Size;
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
    return reverse_iterator(m_last);
}

inline auto
List::rbegin() const noexcept -> const_reverse_iterator {
    return const_reverse_iterator(m_last);
}

inline auto
List::crbegin() const noexcept -> const_reverse_iterator {
    return const_reverse_iterator(m_last);
}

inline auto
List::rend() noexcept -> reverse_iterator {
    return reverse_iterator(nullptr);
}

inline auto
List::rend() const noexcept -> const_reverse_iterator {
    return const_reverse_iterator(nullptr);
}

inline auto
List::crend() const noexcept -> const_reverse_iterator {
    return const_reverse_iterator(nullptr);
}

}

#endif /* JSON_LIST_HPP */
