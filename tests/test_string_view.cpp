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
 * @file test_string_view.cpp
 *
 * @brief Implementation
 */

#include "json/string_view.hpp"

#include "gtest/gtest.h"

#include <utility>
#include <algorithm>

using json::StringView;

TEST(TestStringView, Constructor1) {
    StringView string;

    EXPECT_EQ(0, string.size());
    EXPECT_EQ(0, string.length());
}

TEST(TestStringView, Constructor2) {
    StringView string("test");

    EXPECT_EQ(4, string.size());
    EXPECT_EQ(4, string.length());
    EXPECT_TRUE(std::equal(
                string.cbegin(),
                string.cend(),
                "test"
            ));
}

TEST(TestStringView, Constructor3) {
    StringView string{"test"};

    EXPECT_EQ(4, string.size());
    EXPECT_EQ(4, string.length());
    EXPECT_TRUE(std::equal(
                string.cbegin(),
                string.cend(),
                "test"
            ));
}

TEST(TestStringView, Constructor4) {
    StringView string("test", 2);

    EXPECT_EQ(2, string.size());
    EXPECT_EQ(2, string.length());
    EXPECT_TRUE(std::equal(
                string.cbegin(),
                string.cend(),
                "te"
            ));
}

TEST(TestStringView, Constructor5) {
    StringView string1{"abc"};
    StringView string2(string1);

    EXPECT_EQ(3, string2.size());
    EXPECT_EQ(3, string2.length());
    EXPECT_TRUE(std::equal(
                string2.cbegin(),
                string2.cend(),
                "abc"
            ));
}

TEST(TestStringView, Constructor6) {
    StringView string1{"abc"};
    StringView string2(std::move(string1));

    EXPECT_EQ(3, string2.size());
    EXPECT_EQ(3, string2.length());
    EXPECT_TRUE(std::equal(
                string2.cbegin(),
                string2.cend(),
                "abc"
            ));

    EXPECT_EQ(0, string1.size());
}

TEST(TestStringView, Find1) {
    StringView string{"abctestcba"};

    EXPECT_EQ(3, string.find("test"));
}

TEST(TestStringView, Find2) {
    StringView string{"testabc"};

    EXPECT_EQ(0, string.find("test"));
}

TEST(TestStringView, Find3) {
    StringView string{"abcdtest"};

    EXPECT_EQ(4, string.find("test"));
}

TEST(TestStringView, Find4) {
    StringView string{"abcdtesbvest"};

    EXPECT_EQ(StringView::npos, string.find("test"));
}

TEST(TestStringView, Find5) {
    StringView string{"abcdtesbvtes"};

    EXPECT_EQ(StringView::npos, string.find("test"));
}

TEST(TestStringView, Find6) {
    StringView string{"abc"};

    EXPECT_EQ(3, string.find("", 16));
}

TEST(TestStringView, Find7) {
    StringView string{""};

    EXPECT_EQ(StringView::npos, string.find("test"));
}

TEST(TestStringView, Find8) {
    StringView string{""};

    EXPECT_EQ(0, string.find(""));
}
