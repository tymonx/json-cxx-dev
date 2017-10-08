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
 * @file json/value_iterator.hpp
 *
 * @brief JSON value iterator interface
 */

#ifndef JSON_VALUE_ITERATOR_HPP
#define JSON_VALUE_ITERATOR_HPP

#include "list_iterator.hpp"
#include "array_iterator.hpp"
#include "object_iterator.hpp"

#include <type_traits>

namespace json {

class Value;

template<bool is_const>
class ValueIterator {
public:
    using value_type = typename std::conditional<is_const,
          const Value, Value>::type;

    using pointer = value_type*;
    using reference = value_type&;
    using difference_type = Difference;
    using iterator_category = std::bidirectional_iterator_tag;

    template<bool T>
    using enable_const = typename std::enable_if<T, int>::type;

    template<bool T>
    using enable_iterator = typename std::enable_if<
        (T || (!T && !is_const)), int>::type;

    ValueIterator() noexcept = default;

    ValueIterator(const ArrayIterator<is_const>& other) noexcept;

    ValueIterator(const ObjectIterator<is_const>& other) noexcept;

    ValueIterator(const ValueIterator& other) noexcept = default;

    template<bool T = is_const, typename = enable_const<T>>
    ValueIterator(const ValueIterator<false>& other) noexcept;

    ValueIterator(ValueIterator&& other) noexcept = default;

    ValueIterator& operator=(const ValueIterator& other) noexcept = default;

    ValueIterator& operator=(ValueIterator&& other) noexcept = default;

    ValueIterator& operator++() noexcept;

    ValueIterator operator++(int) noexcept;

    ValueIterator& operator--() noexcept;

    ValueIterator operator--(int) noexcept;

    ValueIterator operator+(difference_type n) const noexcept;

    ValueIterator& operator+=(difference_type n) noexcept;

    ValueIterator operator-(difference_type n) const noexcept;

    ValueIterator& operator-=(difference_type n) noexcept;

    reference operator[](difference_type n) noexcept;

    reference operator[](difference_type n) const noexcept;

    reference operator*() noexcept;

    reference operator*() const noexcept;

    pointer operator->() noexcept;

    pointer operator->() const noexcept;

    operator bool() const noexcept;

    template<bool other_is_const, typename = enable_iterator<other_is_const>>
    operator ObjectIterator<other_is_const>() const noexcept;

    template<bool other_is_const, typename = enable_iterator<other_is_const>>
    operator ArrayIterator<other_is_const>() const noexcept;

    template<bool other_is_const>
    bool operator==(const ValueIterator<other_is_const>& other) const noexcept;

    template<bool other_is_const>
    bool operator!=(const ValueIterator<other_is_const>& other) const noexcept;

    const ListIterator<is_const>& base() const noexcept;
private:
    ListIterator<is_const> m_iterator{};
};

template<bool is_const> inline
ValueIterator<is_const>::ValueIterator(
        const ArrayIterator<is_const>& other) noexcept :
    m_iterator{other.base()}
{ }

template<bool is_const> inline
ValueIterator<is_const>::ValueIterator(
        const ObjectIterator<is_const>& other) noexcept :
    m_iterator{other.base()}
{ }

template<bool is_const> template<bool T, typename> inline
ValueIterator<is_const>::ValueIterator(
        const ValueIterator<false>& other) noexcept :
    m_iterator{other.base()}
{ }

template<bool is_const> inline auto
ValueIterator<is_const>::operator++() noexcept -> ValueIterator& {
    ++m_iterator;
    return *this;
}

template<bool is_const> inline auto
ValueIterator<is_const>::operator++(int) noexcept -> ValueIterator {
    return m_iterator++;
}

template<bool is_const> inline auto
ValueIterator<is_const>::operator--() noexcept -> ValueIterator& {
    --m_iterator;
    return *this;
}

template<bool is_const> inline auto
ValueIterator<is_const>::operator--(int) noexcept -> ValueIterator {
    return m_iterator--;
}

template<bool is_const> inline auto
ValueIterator<is_const>::operator+(difference_type n) const noexcept ->
        ValueIterator {
    return (m_iterator + n);
}

template<bool is_const> inline auto
ValueIterator<is_const>::operator+=(difference_type n) noexcept ->
        ValueIterator& {
    return *this = (m_iterator + n);
}

template<bool is_const> inline auto
ValueIterator<is_const>::operator-(difference_type n) const noexcept ->
        ValueIterator {
    return *this = (m_iterator - n);
}

template<bool is_const> inline auto
ValueIterator<is_const>::operator[](difference_type n) noexcept -> reference {
    return *(*this + n);
}

template<bool is_const> inline auto
ValueIterator<is_const>::operator[](
        difference_type n) const noexcept -> reference {
    return *(*this + n);
}

template<bool is_const> inline auto
ValueIterator<is_const>::operator*() noexcept -> reference {
    return *operator->();
}

template<bool is_const> inline auto
ValueIterator<is_const>::operator*() const noexcept -> reference {
    return *operator->();
}

template<> auto
ValueIterator<true>::operator->() noexcept -> pointer;

template<> inline auto
ValueIterator<false>::operator->() noexcept -> pointer {
    return const_cast<pointer>(ValueIterator<true>{*this}.operator->());
}

template<> inline auto
ValueIterator<true>::operator->() const noexcept -> pointer {
    return const_cast<ValueIterator<true>*>(this)->operator->();
}

template<> inline auto
ValueIterator<false>::operator->() const noexcept -> pointer {
    return const_cast<pointer>(ValueIterator<true>{*this}.operator->());
}

template<bool is_const> inline
ValueIterator<is_const>::operator bool() const noexcept {
    return bool(m_iterator);
}

template<bool is_const>
template<bool other_is_const> inline bool
ValueIterator<is_const>::operator==(
        const ValueIterator<other_is_const>& other) const noexcept {
    return m_iterator == other.m_iterator;
}

template<bool is_const>
template<bool other_is_const> inline bool
ValueIterator<is_const>::operator!=(
        const ValueIterator<other_is_const>& other) const noexcept {
    return m_iterator != other.m_iterator;
}

template<bool is_const>
template<bool other_is_const, typename> inline
ValueIterator<is_const>::operator
        ArrayIterator<other_is_const>() const noexcept {
    return ArrayIterator<other_is_const>{m_iterator};
}

template<bool is_const>
template<bool other_is_const, typename> inline
ValueIterator<is_const>::operator
        ObjectIterator<other_is_const>() const noexcept {
    return ObjectIterator<other_is_const>{m_iterator};
}

template<bool is_const> inline auto
ValueIterator<is_const>::base() const noexcept ->
        const ListIterator<is_const>& {
    return m_iterator;
}

}

#endif /* JSON_VALUE_ITERATOR_HPP */
