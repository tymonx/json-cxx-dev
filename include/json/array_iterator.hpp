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
class ArrayIterator : public ListIterator<is_const> {
public:
    using value_type = typename std::conditional<is_const,
          const Value, Value>::type;

    using pointer = value_type*;
    using reference = value_type&;

    using typename ListIterator<is_const>::difference_type;
    using typename ListIterator<is_const>::iterator_category;

    using ListIterator<is_const>::ListIterator;

    ArrayIterator(const ListIterator<is_const>& other) noexcept;

    reference operator[](difference_type n) noexcept;

    reference operator[](difference_type n) const noexcept;

    pointer operator->() noexcept;

    pointer operator->() const noexcept;

    reference operator*() noexcept;

    reference operator*() const noexcept;
private:
    friend class ArrayIterator<true>;

    pointer get() noexcept;

    pointer get(difference_type n) noexcept;
};

template<> auto
ArrayIterator<false>::get() noexcept -> pointer;

template<> inline auto
ArrayIterator<true>::get() noexcept -> pointer {
    return ArrayIterator<false>{const_cast<
        ListItem*>(ListIterator::operator->())}.get();
}

template<> auto
ArrayIterator<false>::get(difference_type n) noexcept -> pointer;

template<> inline auto
ArrayIterator<true>::get(difference_type n) noexcept -> pointer {
    return ArrayIterator<false>{const_cast<
        ListItem*>(ListIterator::operator->())}.get(n);
}

template<bool is_const> inline
ArrayIterator<is_const>::ArrayIterator(
        const ListIterator<is_const>& other) noexcept :
    ListIterator<is_const>{other}
{ }

template<bool is_const> inline auto
ArrayIterator<is_const>::operator[](difference_type n) noexcept -> reference {
    return *get(n);
}

template<bool is_const> inline auto
ArrayIterator<is_const>::operator[](difference_type n) const noexcept
        -> reference {
    return *get(n);
}

template<bool is_const> inline auto
ArrayIterator<is_const>::operator->() noexcept -> pointer {
    return get();
}

template<bool is_const> inline auto
ArrayIterator<is_const>::operator->() const noexcept -> pointer {
    return get();
}

template<bool is_const> inline auto
ArrayIterator<is_const>::operator*() noexcept -> reference {
    return *get();
}

template<bool is_const> inline auto
ArrayIterator<is_const>::operator*() const noexcept -> reference {
    return *get();
}

}

#endif /* JSON_ARRAY_ITERATOR_HPP */
