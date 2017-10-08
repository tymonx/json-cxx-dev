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
 * @file json/allocator/block.hpp
 *
 * @brief Interface
 */

#ifndef JSON_ALLOCATOR_BLOCK_HPP
#define JSON_ALLOCATOR_BLOCK_HPP

#include "json/allocator.hpp"

namespace json {
namespace allocator {

class Block final : public Allocator {
public:
    static constexpr auto DEFAULT_SIZE{32768};

    Block() noexcept = default;

    Block(Size block_size) noexcept;

    virtual void* allocate(Size size) noexcept override;

    virtual void* reallocate(void* ptr, Size size) noexcept override;

    virtual void deallocate(void* ptr) noexcept override;

    virtual Size size(const void* ptr) const noexcept override;

    virtual ~Block() noexcept override;
private:
    Block(const Block&) = delete;
    Block& operator=(const Block&) = delete;

    void* m_header_last{nullptr};
    Size m_block_size{DEFAULT_SIZE};
};

inline
Block::Block(Size block_size) noexcept :
    m_block_size{block_size}
{ }

}
}

#endif /* JSON_ALLOCATOR_BLOCK_HPP */
