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
 * @file json/object_iterator.hpp
 *
 * @brief JSON object iterator interface
 */

#ifndef JSON_OBJECT_ITERATOR_HPP
#define JSON_OBJECT_ITERATOR_HPP

#include "list_iterator.hpp"

#include <type_traits>

namespace json {

class Pair;

template<bool is_const>
class ObjectIterator {
public:
    using value_type = typename std::conditional<is_const,
          const Pair, Pair>::type;

    using pointer = value_type*;
    using reference = value_type&;
    using difference_type = Difference;
    using iterator_category = std::bidirectional_iterator_tag;

    template<bool T>
    using enable_const = typename std::enable_if<T, int>::type;

    ObjectIterator() noexcept = default;

    ObjectIterator(const ListIterator<is_const>& other) noexcept;

    ObjectIterator(const ObjectIterator& other) noexcept = default;

    template<bool T = is_const, typename = enable_const<T>>
    ObjectIterator(const ObjectIterator<false>& other) noexcept;

    ObjectIterator(ObjectIterator&& other) noexcept = default;

    ObjectIterator& operator=(const ObjectIterator& other) noexcept = default;

    ObjectIterator& operator=(ObjectIterator&& other) noexcept = default;

    ObjectIterator& operator++() noexcept;

    ObjectIterator operator++(int) noexcept;

    ObjectIterator& operator--() noexcept;

    ObjectIterator operator--(int) noexcept;

    ObjectIterator operator+(difference_type n) const noexcept;

    ObjectIterator& operator+=(difference_type n) noexcept;

    ObjectIterator operator-(difference_type n) const noexcept;

    ObjectIterator& operator-=(difference_type n) noexcept;

    reference operator[](difference_type n) noexcept;

    reference operator[](difference_type n) const noexcept;

    reference operator*() noexcept;

    reference operator*() const noexcept;

    pointer operator->() noexcept;

    pointer operator->() const noexcept;

    operator bool() const noexcept;

    template<bool other_is_const>
    bool operator==(const ObjectIterator<other_is_const>& other) const noexcept;

    template<bool other_is_const>
    bool operator!=(const ObjectIterator<other_is_const>& other) const noexcept;
private:
    friend class ObjectIterator<true>;
    friend class ObjectIterator<false>;

    ListIterator<is_const> m_iterator{};
};

template<bool is_const> inline
ObjectIterator<is_const>::ObjectIterator(
        const ListIterator<is_const>& other) noexcept :
    m_iterator{other}
{ }

template<bool is_const> template<bool T, typename> inline
ObjectIterator<is_const>::ObjectIterator(
        const ObjectIterator<false>& other) noexcept :
    m_iterator{other.m_iterator}
{ }

template<bool is_const> inline auto
ObjectIterator<is_const>::operator++() noexcept -> ObjectIterator& {
    ++m_iterator;
    return *this;
}

template<bool is_const> inline auto
ObjectIterator<is_const>::operator++(int) noexcept -> ObjectIterator {
    return m_iterator++;
}

template<bool is_const> inline auto
ObjectIterator<is_const>::operator--() noexcept -> ObjectIterator& {
    --m_iterator;
    return *this;
}

template<bool is_const> inline auto
ObjectIterator<is_const>::operator--(int) noexcept -> ObjectIterator {
    return m_iterator--;
}

template<bool is_const> inline auto
ObjectIterator<is_const>::operator+(difference_type n) const noexcept ->
        ObjectIterator {
    return (m_iterator + n);
}

template<bool is_const> inline auto
ObjectIterator<is_const>::operator+=(difference_type n) noexcept ->
        ObjectIterator& {
    return *this = (m_iterator + n);
}

template<bool is_const> inline auto
ObjectIterator<is_const>::operator-(difference_type n) const noexcept ->
        ObjectIterator {
    return *this = (m_iterator - n);
}

template<bool is_const> inline auto
ObjectIterator<is_const>::operator[](difference_type n) noexcept -> reference {
    return *(*this + n);
}

template<bool is_const> inline auto
ObjectIterator<is_const>::operator[](
        difference_type n) const noexcept -> reference {
    return *(*this + n);
}

template<bool is_const> inline auto
ObjectIterator<is_const>::operator*() noexcept -> reference {
    return *operator->();
}

template<bool is_const> inline auto
ObjectIterator<is_const>::operator*() const noexcept -> reference {
    return *operator->();
}

template<> auto
ObjectIterator<true>::operator->() noexcept -> pointer;

template<> inline auto
ObjectIterator<false>::operator->() noexcept -> pointer {
    return const_cast<pointer>(ObjectIterator<true>{*this}.operator->());
}

template<> inline auto
ObjectIterator<true>::operator->() const noexcept -> pointer {
    return const_cast<ObjectIterator<true>*>(this)->operator->();
}

template<> inline auto
ObjectIterator<false>::operator->() const noexcept -> pointer {
    return const_cast<pointer>(ObjectIterator<true>{*this}.operator->());
}

template<bool is_const> inline
ObjectIterator<is_const>::operator bool() const noexcept {
    return bool(m_iterator);
}

template<bool is_const>
template<bool other_is_const> inline bool
ObjectIterator<is_const>::operator==(
        const ObjectIterator<other_is_const>& other) const noexcept {
    return m_iterator == other.m_iterator;
}

template<bool is_const>
template<bool other_is_const> inline bool
ObjectIterator<is_const>::operator!=(
        const ObjectIterator<other_is_const>& other) const noexcept {
    return m_iterator != other.m_iterator;
}

}

#endif /* JSON_OBJECT_ITERATOR_HPP */
