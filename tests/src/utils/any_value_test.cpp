/*
 * Copyright (c) 2020, Marin Peko
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above
 *   copyright notice, this list of conditions and the following disclaimer
 *   in the documentation and/or other materials provided with the
 *   distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <string>
#include <string_view>
#include <gtest/gtest.h>
#include <booleval/utils/any_value.hpp>

class AnyValueTest : public testing::Test {};

TEST_F(AnyValueTest, IntValue) {
    using namespace booleval::utils;

    any_value value{ 1 };
    EXPECT_EQ(value, "1");
    EXPECT_EQ(value, 1U);
}

TEST_F(AnyValueTest, FloatingPointValue) {
    using namespace booleval::utils;

    any_value value{ 1.234567 };
    EXPECT_EQ(value, "1.234567");
    EXPECT_EQ(value, 1.234567F);
}

TEST_F(AnyValueTest, ConstCharPointerValue) {
    using namespace booleval::utils;

    char const* ptr{ "abc" };
    any_value value{ ptr };
    EXPECT_EQ(value, ptr);
}

TEST_F(AnyValueTest, StringValue) {
    using namespace booleval::utils;

    std::string str{ "abc" };
    any_value value{ str };
    EXPECT_EQ(value, str);
}

TEST_F(AnyValueTest, StringViewValue) {
    using namespace booleval::utils;

    std::string_view strv{ "abc" };
    any_value value{ strv };
    EXPECT_EQ(value, strv);
}

TEST_F(AnyValueTest, IntComparisons) {
    using namespace booleval::utils;

    any_value value{ 1 };

    EXPECT_TRUE(value == 1U);
    EXPECT_TRUE(value != 2U);

    EXPECT_TRUE(value > 0U);
    EXPECT_TRUE(value < 2U);

    EXPECT_TRUE(value >= 0U);
    EXPECT_TRUE(value >= 1U);

    EXPECT_TRUE(value <= 1U);
    EXPECT_TRUE(value <= 2U);
}

TEST_F(AnyValueTest, FloatingPointComparisons) {
    using namespace booleval::utils;

    any_value value{ 1.234567 };

    EXPECT_TRUE(value == 1.234567F);
    EXPECT_TRUE(value != 7.654321F);

    EXPECT_TRUE(value > 0.123456F);
    EXPECT_TRUE(value < 2.345678F);

    EXPECT_TRUE(value >= 0.123456F);
    EXPECT_TRUE(value >= 1.234567F);

    EXPECT_TRUE(value <= 1.234567F);
    EXPECT_TRUE(value <= 2.345678F);
}
