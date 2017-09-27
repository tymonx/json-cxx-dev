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
 * @file json/array.cpp
 *
 * @brief Implementation
 */

#include "json/array.hpp"

#include "array_item.hpp"

#include <new>
#include <cstddef>
#include <utility>
#include <type_traits>

using json::Array;

static_assert(std::is_standard_layout<Array>(),
        "json::Array is not a standard layout");

static_assert(std::is_standard_layout<json::ArrayItem>(),
        "json::ArrayItem is not a standard layout");

Array::~Array() noexcept {
    for (auto it = m_list.begin(); it != m_list.end(); ++it) {
        iterator{it}->~Value();
        allocator().deallocate(&*it);
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
