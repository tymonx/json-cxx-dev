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
 * @file json/object.cpp
 *
 * @brief Implementation
 */

#include "json/object.hpp"

#include "object_item.hpp"

#include <new>
#include <cstddef>
#include <utility>
#include <functional>
#include <type_traits>

using json::Object;

static_assert(std::is_standard_layout<Object>::value,
        "json::Object is not a standard layout");

static_assert(std::is_standard_layout<json::ObjectItem>::value,
        "json::ObjectItem is not a standard layout");

Object::Object(size_type count, allocator_type& alloc) noexcept :
    m_allocator{&alloc}
{
    assign(count, Pair());
}

Object::~Object() noexcept {
    for (auto it = m_list.begin(); it != m_list.end(); ++it) {
        iterator{it}->~Pair();
        allocator().deallocate(&*it);
    }
}

Object& Object::operator=(Object&& other) noexcept {
    if (this != &other) {
        if (&allocator() == &other.allocator()) {
            clear();
            m_list = std::move(other.m_list);
        }
        else {
            *this = std::cref(other);
        }
    }
    return *this;
}

void Object::assign(size_type count, const value_type& pair) noexcept {
    clear();

    while (count--) {
        push_back(pair);
    }
}

template<> void
Object::assign<Object::const_iterator>(const_iterator first,
        const_iterator last) noexcept {
    clear();

    while (first != last) {
        push_back(*first++);
    }
}

void Object::assign(std::initializer_list<value_type> ilist) noexcept {
    clear();

    for (const auto& value : ilist) {
        push_back(value);
    }
}

void Object::push_back(const value_type& pair) noexcept {
    auto ptr = allocator().allocate<ObjectItem>();
    if (ptr) {
        new (&ptr->pair) Pair(pair);
        m_list.push_back(ptr->list);
    }
}

void Object::push_back(value_type&& pair) noexcept {
    auto ptr = allocator().allocate<ObjectItem>();
    if (ptr) {
        new (&ptr->pair) Pair(std::move(pair));
        m_list.push_back(ptr->list);
    }
}

void Object::pop_back() noexcept {
    if (!empty()) {
        auto ptr = &m_list.back();
        iterator{ptr}->~Pair();
        m_list.pop_back();
        allocator().deallocate(ptr);
    }
}

void Object::clear() noexcept {
    for (auto it = m_list.begin(); it != m_list.end(); ++it) {
        iterator{it}->~Pair();
        allocator().deallocate(&*it);
    }
    m_list.clear();
}

Object::reference Object::back() noexcept {
    return reinterpret_cast<ObjectItem*>(&m_list.back())->pair;
}

Object::reference Object::front() noexcept {
    return reinterpret_cast<ObjectItem*>(&m_list.front())->pair;
}
