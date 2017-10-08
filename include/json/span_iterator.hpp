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
 * @file json/span_iterator.hpp
 *
 * @brief JSON span iterator interface
 */

#ifndef JSON_SPAN_ITERATOR_HPP
#define JSON_SPAN_ITERATOR_HPP

#include <cstddef>
#include <iterator>
#include <type_traits>

namespace json {

template<typename T>
class SpanIterator {
public:
    using value_type = T;
    using pointer = value_type*;
    using reference = value_type&;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::random_access_iterator_tag;

    template<typename U>
    using is_const_t = typename std::enable_if<std::is_const<U>::value>::type;

    template<typename U>
    using remove_const_t = typename std::remove_const<U>::type;

    SpanIterator() noexcept = default;

    SpanIterator(pointer ptr) noexcept;

    SpanIterator(const SpanIterator& other) noexcept = default;

    template<typename U = T, typename = is_const_t<U>>
    SpanIterator(const SpanIterator<remove_const_t<T>>& other) noexcept;

    SpanIterator(SpanIterator&& other) noexcept = default;

    SpanIterator& operator=(const SpanIterator& other) noexcept = default;

    SpanIterator& operator=(SpanIterator&& other) noexcept = default;

    SpanIterator& operator++() noexcept;

    SpanIterator operator++(int) noexcept;

    SpanIterator& operator--() noexcept;

    SpanIterator operator--(int) noexcept;

    SpanIterator operator+(difference_type n) const noexcept;

    SpanIterator& operator+=(difference_type n) noexcept;

    SpanIterator operator-(difference_type n) const noexcept;

    SpanIterator& operator-=(difference_type n) noexcept;

    reference operator[](difference_type n) noexcept;

    reference operator[](difference_type n) const noexcept;

    reference operator*() noexcept;

    reference operator*() const noexcept;

    pointer operator->() noexcept;

    pointer operator->() const noexcept;
private:
    pointer m_ptr{nullptr};
};

template<typename T>
template<typename U, typename> inline
SpanIterator<T>::SpanIterator(
        const SpanIterator<remove_const_t<T>>& other) noexcept :
    m_ptr{&*other}
{ }

template<typename T> inline
SpanIterator<T>::SpanIterator(pointer ptr) noexcept :
    m_ptr{ptr}
{ }

template<typename T> inline auto
SpanIterator<T>::operator++() noexcept -> SpanIterator& {
    ++m_ptr;
    return *this;
}

template<typename T> inline auto
SpanIterator<T>::operator++(int) noexcept -> SpanIterator {
    return SpanIterator<T>{m_ptr++};
}

template<typename T> inline auto
SpanIterator<T>::operator--() noexcept -> SpanIterator& {
    --m_ptr;
    return *this;
}

template<typename T> inline auto
SpanIterator<T>::operator--(int) noexcept -> SpanIterator {
    return SpanIterator<T>{m_ptr--};
}

template<typename T> inline auto
SpanIterator<T>::operator+(difference_type n) const noexcept ->
        SpanIterator {
    return m_ptr + n;
}

template<typename T> inline auto
SpanIterator<T>::operator+=(difference_type n) noexcept ->
        SpanIterator& {
    m_ptr += n;
    return *this;
}

template<typename T> inline auto
SpanIterator<T>::operator-(difference_type n) const noexcept ->
        SpanIterator {
    return m_ptr - n;
}

template<typename T> inline auto
SpanIterator<T>::operator-=(difference_type n) noexcept ->
        SpanIterator& {
    m_ptr -= n;
    return *this;
}

template<typename T> inline auto
SpanIterator<T>::operator[](difference_type n) noexcept -> reference {
    return *(m_ptr + n);
}

template<typename T> inline auto
SpanIterator<T>::operator[](difference_type n) const noexcept -> reference {
    return *(m_ptr + n);
}

template<typename T> inline auto
SpanIterator<T>::operator*() noexcept -> reference {
    return *m_ptr;
}

template<typename T> inline auto
SpanIterator<T>::operator*() const noexcept -> reference {
    return *m_ptr;
}

template<typename T> inline auto
SpanIterator<T>::operator->() noexcept -> pointer {
    return m_ptr;
}

template<typename T> inline auto
SpanIterator<T>::operator->() const noexcept -> pointer {
    return m_ptr;
}

template<typename T, typename U> static inline auto
operator-(const SpanIterator<T>& lhs, const SpanIterator<U>& rhs)
        noexcept -> decltype(&(*lhs) - &(*rhs)) {
    return &(*lhs) - &(*rhs);
}

template<typename T> static inline auto
operator+(typename SpanIterator<T>::difference_type n,
        const SpanIterator<T>& rhs) noexcept -> SpanIterator<T> {
    return n + &(*rhs);
}

template<typename T, typename U> static inline auto
operator==(const SpanIterator<T>& lhs,
        const SpanIterator<U>& rhs) noexcept -> bool {
    return &(*lhs) == &(*rhs);
}

template<typename T, typename U> static inline auto
operator!=(const SpanIterator<T>& lhs,
        const SpanIterator<U>& rhs) noexcept -> bool {
    return &(*lhs) != &(*rhs);
}

template<typename T, typename U> static inline auto
operator<(const SpanIterator<T>& lhs,
        const SpanIterator<U>& rhs) noexcept -> bool {
    return &(*lhs) < &(*rhs);
}

template<typename T, typename U> static inline auto
operator>(const SpanIterator<T>& lhs,
        const SpanIterator<U>& rhs) noexcept -> bool {
    return &(*lhs) > &(*rhs);
}

template<typename T, typename U> static inline auto
operator>=(const SpanIterator<T>& lhs,
        const SpanIterator<U>& rhs) noexcept -> bool {
    return &(*lhs) >= &(*rhs);
}

template<typename T, typename U> static inline auto
operator<=(const SpanIterator<T>& lhs,
        const SpanIterator<U>& rhs) noexcept -> bool {
    return &(*lhs) <= &(*rhs);
}

}

#endif /* JSON_SPAN_ITERATOR_HPP */
