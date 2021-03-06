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
 * @file json/object.hpp
 *
 * @brief JSON object interface
 */

#ifndef JSON_OBJECT_HPP
#define JSON_OBJECT_HPP

#include "list.hpp"
#include "types.hpp"
#include "allocator.hpp"
#include "object_iterator.hpp"

#include <initializer_list>

namespace json {

class Object {
public:
    using value_type = Pair;
    using size_type = Size;
    using reference = value_type&;
    using allocator_type = Allocator;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = ObjectIterator<false>;
    using const_iterator = ObjectIterator<true>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    Object() noexcept = default;

    Object(allocator_type& alloc) noexcept;

    Object(const Object& other) noexcept;

    Object(const Object& other, allocator_type& alloc) noexcept;

    Object(Object&& other) noexcept = default;

    Object(Object&& other, allocator_type& alloc) noexcept;

    Object(std::initializer_list<value_type> ilist,
            allocator_type& alloc = Allocator::get_instance()) noexcept;

    Object(size_type count, const value_type& value,
            allocator_type& alloc = Allocator::get_instance()) noexcept;

    Object(size_type count,
            allocator_type& alloc = Allocator::get_instance()) noexcept;

    Object& operator=(const Object& other) noexcept;

    Object& operator=(Object&& other) noexcept;

    Object& operator=(std::initializer_list<value_type> ilist) noexcept;

    void assign(size_type count, const value_type& pair) noexcept;

    template<typename InputIt>
    void assign(InputIt first, InputIt last) noexcept;

    void assign(std::initializer_list<value_type> ilist) noexcept;

    void push_back(const value_type& pair) noexcept;

    void push_back(value_type&& pair) noexcept;

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

    ~Object() noexcept;
private:
    Allocator* m_allocator{&Allocator::get_instance()};
    List m_list{};
};

inline
Object::Object(size_type count, const value_type& pair,
        allocator_type& alloc) noexcept :
    m_allocator{&alloc}
{
    while (count--) {
        push_back(pair);
    }
}

template<> void
Object::assign<Object::const_iterator>(const_iterator first,
        const_iterator last) noexcept;

inline
Object::Object(allocator_type& alloc) noexcept :
    m_allocator{&alloc}
{ }

inline
Object::Object(const Object& other) noexcept :
    Object{other, const_cast<Object&>(other).allocator()}
{ }

inline
Object::Object(Object&& other, allocator_type& alloc) noexcept :
    m_allocator{&alloc}
{
    *this = std::move(other);
}

inline
Object::Object(const Object& other, allocator_type& alloc) noexcept :
    m_allocator{&alloc}
{
    assign(other.cbegin(), other.cend());
}

inline
Object::Object(std::initializer_list<value_type> ilist,
        allocator_type& alloc) noexcept :
    m_allocator{&alloc}
{
    assign(ilist);
}

inline auto
Object::Object::operator=(const Object& other) noexcept -> Object& {
    assign(other.cbegin(), other.cend());
    return *this;
}

inline auto
Object::Object::operator=(
        std::initializer_list<value_type> ilist) noexcept -> Object& {
    assign(ilist);
    return *this;
}

template<typename InputIt> void
Object::assign(InputIt first, InputIt last) noexcept {
    clear();

    while (first != last) {
        push_back(*first++);
    }
}

inline auto
Object::back() const noexcept -> const_reference {
    return const_cast<Object*>(this)->back();
}

inline auto
Object::front() const noexcept -> const_reference {
    return const_cast<Object*>(this)->front();
}

inline auto
Object::allocator() noexcept -> Allocator& {
    return *m_allocator;
}

inline auto
Object::size() const noexcept -> size_type {
    return m_list.size();
}

inline auto
Object::empty() const noexcept -> bool {
    return m_list.empty();
}

inline auto
Object::begin() noexcept -> iterator {
    return m_list.begin();
}

inline auto
Object::begin() const noexcept -> const_iterator {
    return m_list.begin();
}

inline auto
Object::cbegin() const noexcept -> const_iterator {
    return m_list.cbegin();
}

inline auto
Object::end() noexcept -> iterator {
    return m_list.end();
}

inline auto
Object::end() const noexcept -> const_iterator {
    return m_list.end();
}

inline auto
Object::cend() const noexcept -> const_iterator {
    return m_list.cend();
}

inline auto
Object::rbegin() noexcept -> reverse_iterator {
    return m_list.rbegin();
}

inline auto
Object::rbegin() const noexcept -> const_reverse_iterator {
    return m_list.rbegin();
}

inline auto
Object::crbegin() const noexcept -> const_reverse_iterator {
    return m_list.crbegin();
}

inline auto
Object::rend() noexcept -> reverse_iterator {
    return m_list.rend();
}

inline auto
Object::rend() const noexcept -> const_reverse_iterator {
    return m_list.rend();
}

inline auto
Object::crend() const noexcept -> const_reverse_iterator {
    return m_list.crend();
}

template<typename... Args> void
Object::emplace_back(Args&&... args) noexcept {
    push_back({std::forward<Args>(args)...});
}

}

#endif /* JSON_OBJECT_HPP */
