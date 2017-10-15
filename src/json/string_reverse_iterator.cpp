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
 * @file json/string_reverse_iterator.cpp
 *
 * @brief Implementation
 */

#include "json/string_reverse_iterator.hpp"

#include <utility>
#include <type_traits>

using json::StringReverseIterator;

static_assert(std::is_standard_layout<StringReverseIterator>::value,
        "json::StringReverseIterator is not a standard layout");

StringReverseIterator::StringReverseIterator(
        const StringIterator& other) noexcept :
    m_iterator{other}
{ }

StringReverseIterator::StringReverseIterator(
        StringReverseIterator&& other) noexcept :
    m_iterator{std::move(other.m_iterator)}
{ }

StringReverseIterator::StringReverseIterator(
        const StringReverseIterator& other) noexcept :
    m_iterator{other.m_iterator}
{ }

auto StringReverseIterator::operator=(
        StringReverseIterator&& other) noexcept -> StringReverseIterator& {
    m_iterator = std::move(other.m_iterator);
    return *this;
}

auto StringReverseIterator::operator=(
        const StringReverseIterator& other) noexcept -> StringReverseIterator& {
    m_iterator = other.m_iterator;
    return *this;
}

auto StringReverseIterator::operator++() noexcept -> StringReverseIterator& {
    --m_iterator;
    return *this;
}

auto StringReverseIterator::operator++(int) noexcept -> StringReverseIterator {
    return m_iterator--;
}

auto StringReverseIterator::operator--() noexcept -> StringReverseIterator& {
    ++m_iterator;
    return *this;
}

auto StringReverseIterator::operator--(int) noexcept -> StringReverseIterator {
    return m_iterator++;
}

auto StringReverseIterator::operator+(
        difference_type n) const noexcept -> StringReverseIterator {
    return m_iterator - n;
}

auto StringReverseIterator::operator+=(
        difference_type n) noexcept -> StringReverseIterator& {
    return (*this = (*this - n));
}

auto StringReverseIterator::operator-(
        difference_type n) const noexcept -> StringReverseIterator {
    return (*this) + n;
}

auto StringReverseIterator::operator-=(
        difference_type n) noexcept -> StringReverseIterator& {
    return (*this) += n;
}

auto StringReverseIterator::operator[](
        difference_type n) const noexcept -> value_type {
    return *((*this) - n);
}

auto StringReverseIterator::operator*() const noexcept -> value_type {
    return *m_iterator;
}

auto StringReverseIterator::unicode() const noexcept -> Unicode {
    return m_iterator.unicode();
}

auto StringReverseIterator::base() const noexcept -> const void* {
    return m_iterator.base();
}

StringReverseIterator::operator bool() const noexcept {
    return bool(m_iterator);
}

bool StringReverseIterator::operator<(
        const StringReverseIterator& other) const noexcept {
    return base() < other.base();
}

bool StringReverseIterator::operator<=(
        const StringReverseIterator& other) const noexcept {
    return base() <= other.base();
}

bool StringReverseIterator::operator>(
        const StringReverseIterator& other) const noexcept {
    return base() > other.base();
}

bool StringReverseIterator::operator>=(
        const StringReverseIterator& other) const noexcept {
    return base() >= other.base();
}

bool StringReverseIterator::operator==(
        const StringReverseIterator& other) const noexcept {
    return base() == other.base();
}

bool StringReverseIterator::operator!=(
        const StringReverseIterator& other) const noexcept {
    return base() != other.base();
}

StringReverseIterator::~StringReverseIterator() noexcept { }
