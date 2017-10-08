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
 * @file json/allocator/dummy.hpp
 *
 * @brief Interface
 */

#ifndef JSON_ALLOCATOR_DUMMY_HPP
#define JSON_ALLOCATOR_DUMMY_HPP

#include "json/allocator.hpp"

namespace json {
namespace allocator {

class Dummy final : public Allocator {
public:
    static Allocator& get_instance() noexcept;

    Dummy() noexcept = default;

    virtual void* allocate(Size size) noexcept override;

    virtual void* reallocate(void* ptr, Size size) noexcept override;

    virtual void deallocate(void* ptr) noexcept override;

    virtual Size size(const void* ptr) const noexcept override;

    virtual ~Dummy() noexcept override;
};

}
}

#endif /* JSON_ALLOCATOR_DUMMY_HPP */
