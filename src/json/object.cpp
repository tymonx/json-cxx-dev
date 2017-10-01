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
 *   list of conditions and the following disclaimer.
 *
 * @copright
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
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

