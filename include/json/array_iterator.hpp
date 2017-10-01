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

    ArrayIterator(const ListIterator<is_const>& other) noexcept;

    ArrayIterator(const ArrayIterator& other) noexcept = default;

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
private:
    friend class ArrayIterator<true>;
    friend class ArrayIterator<false>;

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
    m_iterator{other.m_iterator}
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

}

#endif /* JSON_ARRAY_ITERATOR_HPP */
