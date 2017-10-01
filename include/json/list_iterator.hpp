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

}

#endif /* JSON_LIST_ITERATOR_HPP */
