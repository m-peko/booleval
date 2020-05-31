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
#include <booleval/utils/any_mem_fn.hpp>

class AnyMemFnTest : public testing::Test {
public:
    template <typename T>
    class obj {
    public:
        obj() : value_{} {}
        obj(T value) : value_{ value } {}
        T value() const noexcept { return value_; }

    private:
        T value_;
    };
};

TEST_F(AnyMemFnTest, IntValue) {
    using namespace booleval::utils;

    obj<uint8_t> foo{ 1 };
    any_mem_fn fn{ &obj<uint8_t>::value };

    EXPECT_EQ(fn.invoke(foo), "1");
    EXPECT_EQ(fn.invoke(foo), 1U);
}

TEST_F(AnyMemFnTest, FloatingPointValue) {
    using namespace booleval::utils;

    obj<double> foo{ 1.234567 };
    any_mem_fn fn{ &obj<double>::value };

    EXPECT_EQ(fn.invoke(foo), "1.234567");
    EXPECT_EQ(fn.invoke(foo), 1.234567F);
}

TEST_F(AnyMemFnTest, StringValue) {
    using namespace booleval::utils;

    obj<std::string> foo{ "abc" };
    any_mem_fn fn{ &obj<std::string>::value };

    EXPECT_EQ(fn.invoke(foo), "abc");
}

TEST_F(AnyMemFnTest, StringViewValue) {
    using namespace booleval::utils;

    obj<std::string_view> foo{ "abc" };
    any_mem_fn fn{ &obj<std::string_view>::value };

    EXPECT_EQ(fn.invoke(foo), "abc");
}

TEST_F(AnyMemFnTest, IntComparisons) {
    using namespace booleval::utils;

    obj<uint8_t> foo{ 1 };
    any_mem_fn fn{ &obj<uint8_t>::value };

    EXPECT_TRUE(fn.invoke(foo) == 1U);
    EXPECT_TRUE(fn.invoke(foo) != 2U);

    EXPECT_TRUE(fn.invoke(foo) > 0U);
    EXPECT_TRUE(fn.invoke(foo) < 2U);

    EXPECT_TRUE(fn.invoke(foo) >= 0U);
    EXPECT_TRUE(fn.invoke(foo) >= 1U);

    EXPECT_TRUE(fn.invoke(foo) <= 1U);
    EXPECT_TRUE(fn.invoke(foo) <= 2U);
}

TEST_F(AnyMemFnTest, FloatingPointComparisons) {
    using namespace booleval::utils;

    obj<double> foo{ 1.234567 };
    any_mem_fn fn{ &obj<double>::value };

    EXPECT_TRUE(fn.invoke(foo) == 1.234567F);
    EXPECT_TRUE(fn.invoke(foo) != 7.654321F);

    EXPECT_TRUE(fn.invoke(foo) > 0.123456F);
    EXPECT_TRUE(fn.invoke(foo) < 2.345678F);

    EXPECT_TRUE(fn.invoke(foo) >= 0.123456F);
    EXPECT_TRUE(fn.invoke(foo) >= 1.234567F);

    EXPECT_TRUE(fn.invoke(foo) <= 1.234567F);
    EXPECT_TRUE(fn.invoke(foo) <= 2.345678F);
}
