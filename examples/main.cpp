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
 * */

#include "json/string.hpp"
#include "json/pair.hpp"
#include "json/value.hpp"

#include <iostream>

int main() {
    json::Value value;

    value.emplace_back("a", 1);
    value.emplace_back("b", 2);
    value.emplace_back("c", 3);
    value.emplace_back("d", 4);

    std::cout << "Size: " << value.size() << std::endl;

    for (const auto& item : value) {
        std::cout << json::Int(item) << std::endl;
    }
}
