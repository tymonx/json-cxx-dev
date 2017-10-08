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
 * @file json/array.cpp
 *
 * @brief Implementation
 */

#include "json/array.hpp"

#include "array_item.hpp"

#include <new>
#include <cstddef>
#include <utility>
#include <functional>
#include <type_traits>

using json::Array;

static_assert(std::is_standard_layout<Array>::value,
        "json::Array is not a standard layout");

static_assert(std::is_standard_layout<json::ArrayItem>::value,
        "json::ArrayItem is not a standard layout");

Array::Array(size_type count, allocator_type& alloc) noexcept :
    m_allocator{&alloc}
{
    assign(count, Value());
}

Array::~Array() noexcept {
    for (auto it = m_list.begin(); it != m_list.end(); ++it) {
        iterator{it}->~Value();
        allocator().deallocate(&*it);
    }
}

Array& Array::operator=(Array&& other) noexcept {
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

void Array::assign(size_type count, const value_type& value) noexcept {
    clear();

    while (count--) {
        push_back(value);
    }
}

template<> void
Array::assign<Array::const_iterator>(const_iterator first,
        const_iterator last) noexcept {
    clear();

    while (first != last) {
        push_back(*first++);
    }
}

void Array::assign(std::initializer_list<value_type> ilist) noexcept {
    clear();

    for (const auto& value : ilist) {
        push_back(value);
    }
}

void Array::push_back(const value_type& value) noexcept {
    auto ptr = allocator().allocate<ArrayItem>();
    if (ptr) {
        new (&ptr->value) Value(value);
        m_list.push_back(ptr->list);
    }
}

void Array::push_back(value_type&& value) noexcept {
    auto ptr = allocator().allocate<ArrayItem>();
    if (ptr) {
        new (&ptr->value) Value(std::move(value));
        m_list.push_back(ptr->list);
    }
}

void Array::pop_back() noexcept {
    if (!empty()) {
        auto ptr = &m_list.back();
        iterator{ptr}->~Value();
        m_list.pop_back();
        allocator().deallocate(ptr);
    }
}

void Array::clear() noexcept {
    for (auto it = m_list.begin(); it != m_list.end(); ++it) {
        iterator{it}->~Value();
        allocator().deallocate(&*it);
    }
    m_list.clear();
}

Array::reference Array::back() noexcept {
    return reinterpret_cast<ArrayItem*>(&m_list.back())->value;
}

Array::reference Array::front() noexcept {
    return reinterpret_cast<ArrayItem*>(&m_list.front())->value;
}
