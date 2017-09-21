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
 * @file json/string.cpp
 *
 * @brief Implementation
 */

#include "json/string.hpp"

#include <algorithm>

using json::String;

static constexpr String::value_type EMPTY_STRING[]{""};

String::String(String&& other, allocator_type* alloc) noexcept :
    m_allocator{alloc}
{
    if (other.allocator() == allocator()) {
        m_data = other.data();
        m_size = other.size();
        other.m_allocator = nullptr;
    }
    else if (allocator()) {
        m_data = allocator()->allocate<value_type>(other.size());
        if (m_data) {
            m_size = other.size();
            std::copy_n(other.data(), other.size(), m_data);
        }
    }
}

String::String(size_type count, value_type ch, allocator_type* alloc) noexcept :
    m_allocator{alloc}
{
    if (allocator()) {
        m_data = allocator()->allocate<value_type>(count);
        if (m_data) {
            m_size = count;
            std::fill_n(m_data, m_size, ch);
        }
    }
}

String::String(const_pointer s, size_type count, allocator_type* alloc) noexcept :
    m_allocator{alloc}
{
    if (allocator()) {
        m_data = allocator()->allocate<value_type>(count);
        if (m_data) {
            m_size = count;
            std::copy_n(s, count,  m_data);
        }
    }
}

String::String(const String& other, size_type pos, size_type count,
        allocator_type* alloc) noexcept :
    m_allocator{alloc}
{
    auto total_length = other.length();

    if (total_length > pos) {
        total_length -= pos;
    }
    else {
        total_length = 0;
    }

    if (count < total_length) {
        total_length = count;
    }

    if (allocator()) {
        m_data = allocator()->allocate<value_type>(total_length);
        if (m_data) {
            m_size = total_length;
            std::copy_n(other.data(), total_length, m_data);
        }
    }
}

String::size_type String::length(const_pointer s) noexcept {
    size_type count = 0;

    if (s) {
        while (*s && (count < npos)) {
            ++s;
            ++count;
        }
    }

    return count;
}

void String::shrink_to_fit() noexcept {
    if (m_allocator && (m_allocator->size(m_data) != m_size)) {
        auto ptr = m_allocator->reallocate(m_data, m_size);
        if (ptr) {
            m_data = ptr;
        }
    }
}

void String::reserve(size_type new_capacity) noexcept {
    if (m_allocator && (m_allocator->size(m_data) < new_capacity)) {
        auto ptr = m_allocator->reallocate(m_data, m_size);
        if (ptr) {
            m_data = ptr;
        }
    }
}

void String::resize(size_type count) noexcept {
    if (count <= m_size) {
        m_size = count;
    }
    else if (m_allocator) {
        auto ptr = m_allocator->reallocate(m_data, count);
        if (ptr) {
            m_data = ptr;
            m_size = count;
        }
    }
}

void String::resize(size_type count, value_type ch) noexcept {
    if (count <= m_size) {
        m_size = count;
    }
    else if (m_allocator) {
        auto ptr = m_allocator->reallocate(m_data, count);
        if (ptr) {
            std::fill_n(ptr + m_size, count - m_size, ch);
            m_data = ptr;
            m_size = count;
        }
    }
}

String::size_type String::copy(pointer dest, size_type count,
        size_type pos) const noexcept {
    if (dest) {
        if (pos > m_size) {
            pos = m_size;
        }

        auto diff = m_size - pos;

        if (diff < count) {
            count = diff;
        }

        std::copy_n(m_data + pos, count, dest);
    }
    else {
        count = 0;
    }

    return count;
}

void String::push_back(value_type ch) noexcept {
    if (m_allocator) {
        if (m_size < m_allocator->size(m_data)) {
            m_data[m_size++] = ch;
        }
        else {
            auto ptr = m_allocator->allocate<value_type>(m_size + 1);
            if (ptr) {
                m_data = ptr;
                m_data[m_size++] = ch;
            }
        }
    }
}

String::const_pointer String::c_str() noexcept {
    String::const_pointer str = EMPTY_STRING;

    if (m_data[m_size] != '\0') {
        if (m_allocator) {
            if (m_size < m_allocator->size(m_data)) {
                m_data[m_size] = '\0';
            }
            else {
                auto ptr = m_allocator->allocate<value_type>(m_size + 1);
                if (ptr) {
                    m_data = ptr;
                    m_data[m_size] = '\0';
                }
            }
        }
    }
    else {
        str = m_data;
    }

    return str;
}
