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
 * @file test_string.cpp
 *
 * @brief Implementation
 */

#include "json/string.hpp"

#include "gtest/gtest.h"

#include <utility>
#include <algorithm>

using json::String;

TEST(TestString, Constructor1) {
    String string;

    EXPECT_EQ(0, string.size());
    EXPECT_EQ(0, string.length());
    EXPECT_EQ(nullptr, string.data());
}

TEST(TestString, Constructor2) {
    String string("test");

    EXPECT_EQ(4, string.size());
    EXPECT_EQ(4, string.length());
    EXPECT_NE(nullptr, string.data());
    EXPECT_TRUE(std::equal(
                string.cbegin(),
                string.cend(),
                "test"
            ));
}

TEST(TestString, Constructor3) {
    String string{"test"};

    EXPECT_EQ(4, string.size());
    EXPECT_EQ(4, string.length());
    EXPECT_NE(nullptr, string.data());
    EXPECT_TRUE(std::equal(
                string.cbegin(),
                string.cend(),
                "test"
            ));
}

TEST(TestString, Constructor4) {
    String string(5, 'c');

    EXPECT_EQ(5, string.size());
    EXPECT_EQ(5, string.length());
    EXPECT_NE(nullptr, string.data());
    EXPECT_TRUE(std::equal(
                string.cbegin(),
                string.cend(),
                "ccccc"
            ));
}

TEST(TestString, Constructor5) {
    String string("test", 2);

    EXPECT_EQ(2, string.size());
    EXPECT_EQ(2, string.length());
    EXPECT_NE(nullptr, string.data());
    EXPECT_TRUE(std::equal(
                string.cbegin(),
                string.cend(),
                "te"
            ));
}

TEST(TestString, Constructor6) {
    String string{'a', 'b', 'c'};

    EXPECT_EQ(3, string.size());
    EXPECT_EQ(3, string.length());
    EXPECT_NE(nullptr, string.data());
    EXPECT_TRUE(std::equal(
                string.cbegin(),
                string.cend(),
                "abc"
            ));
}

TEST(TestString, Constructor7) {
    String string1{'a', 'b', 'c'};
    String string2(string1);

    EXPECT_EQ(3, string2.size());
    EXPECT_EQ(3, string2.length());
    EXPECT_NE(nullptr, string2.data());
    EXPECT_TRUE(std::equal(
                string2.cbegin(),
                string2.cend(),
                "abc"
            ));
}

TEST(TestString, Constructor8) {
    String string1{'a', 'b', 'c'};
    String string2(std::move(string1));

    EXPECT_EQ(3, string2.size());
    EXPECT_EQ(3, string2.length());
    EXPECT_NE(nullptr, string2.data());
    EXPECT_TRUE(std::equal(
                string2.cbegin(),
                string2.cend(),
                "abc"
            ));

    EXPECT_EQ(0, string1.size());
    EXPECT_EQ(nullptr, string1.data());
}

TEST(TestString, Constructor9) {
    String string1{'a', 'b', 'c'};
    String string2(string1, 1, 1);

    EXPECT_EQ(1, string2.size());
    EXPECT_EQ(1, string2.length());
    EXPECT_NE(nullptr, string2.data());
    EXPECT_TRUE(std::equal(
                string2.cbegin(),
                string2.cend(),
                "b"
            ));
}
