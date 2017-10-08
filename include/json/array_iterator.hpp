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
 * @file json/array_iterator.hpp
 *
 * @brief JSON array iterator interface
 */

#ifndef JSON_ARRAY_ITERATOR_HPP
#define JSON_ARRAY_ITERATOR_HPP

#include "list_iterator.hpp"

#include <type_traits>

namespace json {

class Value;

template<bool is_const>
class ArrayIterator {
public:
    using value_type = typename std::conditional<is_const,
          const Value, Value>::type;

    using pointer = value_type*;
    using reference = value_type&;
    using difference_type = Difference;
    using iterator_category = std::bidirectional_iterator_tag;

    template<bool T>
    using enable_const = typename std::enable_if<T, int>::type;

    ArrayIterator() noexcept = default;

    ArrayIterator(const ArrayIterator& other) noexcept = default;

    ArrayIterator(const ListIterator<is_const>& other) noexcept;

    template<bool T = is_const, typename = enable_const<T>>
    ArrayIterator(const ArrayIterator<false>& other) noexcept;

    ArrayIterator(ArrayIterator&& other) noexcept = default;

    ArrayIterator& operator=(const ArrayIterator& other) noexcept = default;

    ArrayIterator& operator=(ArrayIterator&& other) noexcept = default;

    ArrayIterator& operator++() noexcept;

    ArrayIterator operator++(int) noexcept;

    ArrayIterator& operator--() noexcept;

    ArrayIterator operator--(int) noexcept;

    ArrayIterator operator+(difference_type n) const noexcept;

    ArrayIterator& operator+=(difference_type n) noexcept;

    ArrayIterator operator-(difference_type n) const noexcept;

    ArrayIterator& operator-=(difference_type n) noexcept;

    reference operator[](difference_type n) noexcept;

    reference operator[](difference_type n) const noexcept;

    reference operator*() noexcept;

    reference operator*() const noexcept;

    pointer operator->() noexcept;

    pointer operator->() const noexcept;

    operator bool() const noexcept;

    template<bool other_is_const>
    bool operator==(const ArrayIterator<other_is_const>& other) const noexcept;

    template<bool other_is_const>
    bool operator!=(const ArrayIterator<other_is_const>& other) const noexcept;

    const ListIterator<is_const>& base() const noexcept;
private:
    ListIterator<is_const> m_iterator{};
};

template<bool is_const> inline
ArrayIterator<is_const>::ArrayIterator(
        const ListIterator<is_const>& other) noexcept :
    m_iterator{other}
{ }

template<bool is_const> template<bool T, typename> inline
ArrayIterator<is_const>::ArrayIterator(
        const ArrayIterator<false>& other) noexcept :
    m_iterator{other.base()}
{ }

template<bool is_const> inline auto
ArrayIterator<is_const>::operator++() noexcept -> ArrayIterator& {
    ++m_iterator;
    return *this;
}

template<bool is_const> inline auto
ArrayIterator<is_const>::operator++(int) noexcept -> ArrayIterator {
    return m_iterator++;
}

template<bool is_const> inline auto
ArrayIterator<is_const>::operator--() noexcept -> ArrayIterator& {
    --m_iterator;
    return *this;
}

template<bool is_const> inline auto
ArrayIterator<is_const>::operator--(int) noexcept -> ArrayIterator {
    return m_iterator--;
}

template<bool is_const> inline auto
ArrayIterator<is_const>::operator+(difference_type n) const noexcept ->
        ArrayIterator {
    return (m_iterator + n);
}

template<bool is_const> inline auto
ArrayIterator<is_const>::operator+=(difference_type n) noexcept ->
        ArrayIterator& {
    return *this = (m_iterator + n);
}

template<bool is_const> inline auto
ArrayIterator<is_const>::operator-(difference_type n) const noexcept ->
        ArrayIterator {
    return *this = (m_iterator - n);
}

template<bool is_const> inline auto
ArrayIterator<is_const>::operator[](difference_type n) noexcept -> reference {
    return *(*this + n);
}

template<bool is_const> inline auto
ArrayIterator<is_const>::operator[](
        difference_type n) const noexcept -> reference {
    return *(*this + n);
}

template<bool is_const> inline auto
ArrayIterator<is_const>::operator*() noexcept -> reference {
    return *operator->();
}

template<bool is_const> inline auto
ArrayIterator<is_const>::operator*() const noexcept -> reference {
    return *operator->();
}

template<> auto
ArrayIterator<true>::operator->() noexcept -> pointer;

template<> inline auto
ArrayIterator<false>::operator->() noexcept -> pointer {
    return const_cast<pointer>(ArrayIterator<true>{*this}.operator->());
}

template<> inline auto
ArrayIterator<true>::operator->() const noexcept -> pointer {
    return const_cast<ArrayIterator<true>*>(this)->operator->();
}

template<> inline auto
ArrayIterator<false>::operator->() const noexcept -> pointer {
    return const_cast<pointer>(ArrayIterator<true>{*this}.operator->());
}

template<bool is_const> inline
ArrayIterator<is_const>::operator bool() const noexcept {
    return bool(m_iterator);
}

template<bool is_const>
template<bool other_is_const> inline bool
ArrayIterator<is_const>::operator==(
        const ArrayIterator<other_is_const>& other) const noexcept {
    return m_iterator == other.m_iterator;
}

template<bool is_const>
template<bool other_is_const> inline bool
ArrayIterator<is_const>::operator!=(
        const ArrayIterator<other_is_const>& other) const noexcept {
    return m_iterator != other.m_iterator;
}

template<bool is_const> inline auto
ArrayIterator<is_const>::base() const noexcept ->
        const ListIterator<is_const>& {
    return m_iterator;
}

}

#endif /* JSON_ARRAY_ITERATOR_HPP */
