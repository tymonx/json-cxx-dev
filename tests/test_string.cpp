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
