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
 * @file json/list_iterator.hpp
 *
 * @brief JSON list iterator interface
 */

#ifndef JSON_LIST_ITERATOR_HPP
#define JSON_LIST_ITERATOR_HPP

#include "types.hpp"
#include "list_item.hpp"

#include <cstddef>
#include <iterator>
#include <type_traits>

namespace json {

template<bool is_const>
class ListIterator {
public:
    using value_type = typename std::conditional<
        is_const, const ListItem, ListItem>::type;

    using pointer = value_type*;
    using reference = value_type&;
    using difference_type = Difference;
    using iterator_category = std::bidirectional_iterator_tag;

    template<bool T>
    using enable_const = typename std::enable_if<T, int>::type;

    ListIterator() noexcept = default;

    ListIterator(pointer ptr) noexcept;

    ListIterator(const ListIterator& other) noexcept = default;

    template<bool T = is_const, typename = enable_const<T>>
    ListIterator(const ListIterator<false>& other) noexcept;

    ListIterator(ListIterator&& other) noexcept = default;

    ListIterator& operator=(const ListIterator& other) noexcept = default;

    ListIterator& operator=(ListIterator&& other) noexcept = default;

    ListIterator& operator++() noexcept;

    ListIterator operator++(int) noexcept;

    ListIterator& operator--() noexcept;

    ListIterator operator--(int) noexcept;

    ListIterator operator+(difference_type n) const noexcept;

    ListIterator& operator+=(difference_type n) noexcept;

    ListIterator operator-(difference_type n) const noexcept;

    ListIterator& operator-=(difference_type n) noexcept;

    reference operator[](difference_type n) noexcept;

    reference operator[](difference_type n) const noexcept;

    reference operator*() noexcept;

    reference operator*() const noexcept;

    pointer operator->() noexcept;

    pointer operator->() const noexcept;

    operator bool() const noexcept;

    template<bool other_is_const>
    bool operator==(const ListIterator<other_is_const>& other) const noexcept;

    template<bool other_is_const>
    bool operator!=(const ListIterator<other_is_const>& other) const noexcept;

    pointer base() const noexcept;
private:
    pointer m_item{nullptr};
};

template<bool is_const> template<bool T, typename> inline
ListIterator<is_const>::ListIterator(
        const ListIterator<false>& other) noexcept :
    m_item{other.ListIterator::operator->()}
{ }

template<bool is_const> inline
ListIterator<is_const>::ListIterator(pointer ptr) noexcept :
    m_item{ptr}
{ }

template<bool is_const> inline auto
ListIterator<is_const>::operator++() noexcept -> ListIterator& {
    m_item = m_item->next;
    return *this;
}

template<bool is_const> inline auto
ListIterator<is_const>::operator++(int) noexcept -> ListIterator {
    ListIterator<is_const> tmp{m_item};
    m_item = m_item->next;
    return tmp;
}

template<bool is_const> inline auto
ListIterator<is_const>::operator--() noexcept -> ListIterator& {
    m_item = m_item->prev;
    return *this;
}

template<bool is_const> inline auto
ListIterator<is_const>::operator--(int) noexcept -> ListIterator {
    ListIterator<is_const> tmp{m_item};
    m_item = m_item->prev;
    return tmp;
}

template<> auto
ListIterator<false>::operator+(difference_type n) const noexcept ->
        ListIterator;

template<> inline auto
ListIterator<true>::operator+(difference_type n) const noexcept ->
        ListIterator {
    return ListIterator<false>{const_cast<std::remove_const<
        value_type>::type*>(m_item)} + n;
}

template<> inline auto
ListIterator<false>::operator+=(difference_type n) noexcept ->
        ListIterator& {
    return *this = (ListIterator<false>{const_cast<std::remove_const<
        value_type>::type*>(m_item)} + n);
}

template<> inline auto
ListIterator<true>::operator+=(difference_type n) noexcept ->
        ListIterator& {
    return *this = (ListIterator<false>{const_cast<std::remove_const<
        value_type>::type*>(m_item)} + n);
}

template<> auto
ListIterator<false>::operator-(difference_type n) const noexcept ->
        ListIterator;

template<> inline auto
ListIterator<true>::operator-(difference_type n) const noexcept ->
        ListIterator {
    return ListIterator<false>{const_cast<std::remove_const<
        value_type>::type*>(m_item)} - n;
}

template<> inline auto
ListIterator<false>::operator-=(difference_type n) noexcept ->
        ListIterator& {
     return *this = (ListIterator<false>{const_cast<std::remove_const<
        value_type>::type*>(m_item)} - n);
}

template<> inline auto
ListIterator<true>::operator-=(difference_type n) noexcept ->
        ListIterator& {
    return *this = (ListIterator<false>{const_cast<std::remove_const<
        value_type>::type*>(m_item)} - n);
}

template<bool is_const> inline auto
ListIterator<is_const>::operator[](difference_type n) noexcept -> reference {
    return *(*this + n);
}

template<bool is_const> inline auto
ListIterator<is_const>::operator[](
        difference_type n) const noexcept -> reference {
    return *(*this + n);
}

template<bool is_const> inline auto
ListIterator<is_const>::operator*() noexcept -> reference {
    return *m_item;
}

template<bool is_const> inline auto
ListIterator<is_const>::operator*() const noexcept -> reference {
    return *m_item;
}

template<bool is_const> inline auto
ListIterator<is_const>::operator->() noexcept -> pointer {
    return m_item;
}

template<bool is_const> inline auto
ListIterator<is_const>::operator->() const noexcept -> pointer {
    return m_item;
}

template<bool is_const> inline
ListIterator<is_const>::operator bool() const noexcept {
    return (nullptr != m_item);
}

template<bool is_const>
template<bool other_is_const> inline auto
ListIterator<is_const>::operator==(
        const ListIterator<other_is_const>& other) const noexcept -> bool {
    return m_item == &(*other);
}

template<bool is_const>
template<bool other_is_const> inline auto
ListIterator<is_const>::operator!=(
        const ListIterator<other_is_const>& other) const noexcept -> bool {
    return m_item != &(*other);
}

template<bool is_const> inline auto
ListIterator<is_const>::base() const noexcept -> pointer {
    return m_item;
}

}

#endif /* JSON_LIST_ITERATOR_HPP */
