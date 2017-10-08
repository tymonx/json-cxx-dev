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
 * @file json/array.hpp
 *
 * @brief JSON array interface
 */

#ifndef JSON_ARRAY_HPP
#define JSON_ARRAY_HPP

#include "list.hpp"
#include "types.hpp"
#include "allocator.hpp"
#include "array_iterator.hpp"

#include <utility>
#include <initializer_list>

namespace json {

class Array {
public:
    using value_type = Value;
    using size_type = Size;
    using reference = value_type&;
    using allocator_type = Allocator;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = ArrayIterator<false>;
    using const_iterator = ArrayIterator<true>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    Array() noexcept = default;

    Array(allocator_type& alloc) noexcept;

    Array(const Array& other) noexcept;

    Array(const Array& other, allocator_type& alloc) noexcept;

    Array(Array&& other) noexcept = default;

    Array(Array&& other, allocator_type& alloc) noexcept;

    Array(std::initializer_list<value_type> ilist,
            allocator_type& alloc = Allocator::get_instance()) noexcept;

    Array(size_type count, const value_type& value,
            allocator_type& alloc = Allocator::get_instance()) noexcept;

    Array(size_type count,
            allocator_type& alloc = Allocator::get_instance()) noexcept;

    Array& operator=(const Array& other) noexcept;

    Array& operator=(Array&& other) noexcept;

    Array& operator=(std::initializer_list<value_type> ilist) noexcept;

    void assign(size_type count, const value_type& value) noexcept;

    template<typename InputIt>
    void assign(InputIt first, InputIt last) noexcept;

    void assign(std::initializer_list<value_type> ilist) noexcept;

    void push_back(const value_type& value) noexcept;

    void push_back(value_type&& value) noexcept;

    template<typename... Args>
    void emplace_back(Args&&... args) noexcept;

    void pop_back() noexcept;

    void clear() noexcept;

    size_type size() const noexcept;

    bool empty() const noexcept;

    reference back() noexcept;

    const_reference back() const noexcept;

    reference front() noexcept;

    const_reference front() const noexcept;

    iterator begin() noexcept;

    Allocator& allocator() noexcept;

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

    ~Array() noexcept;
private:
    Allocator* m_allocator{&Allocator::get_instance()};
    List m_list{};
};

inline
Array::Array(size_type count, const value_type& value,
        allocator_type& alloc) noexcept :
    m_allocator{&alloc}
{
    while (count--) {
        push_back(value);
    }
}

template<> void
Array::assign<Array::const_iterator>(const_iterator first,
        const_iterator last) noexcept;

inline
Array::Array(allocator_type& alloc) noexcept :
    m_allocator{&alloc}
{ }

inline
Array::Array(const Array& other) noexcept :
    Array{other, const_cast<Array&>(other).allocator()}
{ }

inline
Array::Array(Array&& other, allocator_type& alloc) noexcept :
    m_allocator{&alloc}
{
    *this = std::move(other);
}

inline
Array::Array(const Array& other, allocator_type& alloc) noexcept :
    m_allocator{&alloc}
{
    assign(other.cbegin(), other.cend());
}

inline
Array::Array(std::initializer_list<value_type> ilist,
        allocator_type& alloc) noexcept :
    m_allocator{&alloc}
{
    assign(ilist);
}

inline auto
Array::Array::operator=(const Array& other) noexcept -> Array& {
    assign(other.cbegin(), other.cend());
    return *this;
}

inline auto
Array::Array::operator=(
        std::initializer_list<value_type> ilist) noexcept -> Array& {
    assign(ilist);
    return *this;
}

template<typename InputIt> void
Array::assign(InputIt first, InputIt last) noexcept {
    clear();

    while (first != last) {
        push_back(*first++);
    }
}

inline auto
Array::back() const noexcept -> const_reference {
    return const_cast<Array*>(this)->back();
}

inline auto
Array::front() const noexcept -> const_reference {
    return const_cast<Array*>(this)->front();
}

inline auto
Array::allocator() noexcept -> Allocator& {
    return *m_allocator;
}

inline auto
Array::size() const noexcept -> size_type {
    return m_list.size();
}

inline auto
Array::empty() const noexcept -> bool {
    return m_list.empty();
}

inline auto
Array::begin() noexcept -> iterator {
    return m_list.begin();
}

inline auto
Array::begin() const noexcept -> const_iterator {
    return m_list.begin();
}

inline auto
Array::cbegin() const noexcept -> const_iterator {
    return m_list.cbegin();
}

inline auto
Array::end() noexcept -> iterator {
    return m_list.end();
}

inline auto
Array::end() const noexcept -> const_iterator {
    return m_list.end();
}

inline auto
Array::cend() const noexcept -> const_iterator {
    return m_list.cend();
}

inline auto
Array::rbegin() noexcept -> reverse_iterator {
    return m_list.rbegin();
}

inline auto
Array::rbegin() const noexcept -> const_reverse_iterator {
    return m_list.rbegin();
}

inline auto
Array::crbegin() const noexcept -> const_reverse_iterator {
    return m_list.crbegin();
}

inline auto
Array::rend() noexcept -> reverse_iterator {
    return m_list.rend();
}

inline auto
Array::rend() const noexcept -> const_reverse_iterator {
    return m_list.rend();
}

inline auto
Array::crend() const noexcept -> const_reverse_iterator {
    return m_list.crend();
}

template<typename... Args> void
Array::emplace_back(Args&&... args) noexcept {
    push_back({std::forward<Args>(args)...});
}

}

#endif /* JSON_ARRAY_HPP */
