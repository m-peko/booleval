/*
 * Copyright (c) 2019, Marin Peko
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

#include <gtest/gtest.h>
#include <booleval/token/token.hpp>

class TokenTest : public testing::Test {};

TEST_F(TokenTest, DefaultConstructor) {
    using namespace booleval;

    token::token token;
    EXPECT_EQ(token.type(), token::token_type::unknown);
    EXPECT_EQ(token.value(), "");
}

TEST_F(TokenTest, ConstructorFromType) {
    using namespace booleval;

    token::token token(token::token_type::logical_and);
    EXPECT_EQ(token.type(), token::token_type::logical_and);
    EXPECT_TRUE(token.value() == "and" ||
                token.value() == "AND");
}

TEST_F(TokenTest, ConstructorFromValue) {
    using namespace booleval;

    token::token token("and");
    EXPECT_EQ(token.type(), token::token_type::logical_and);
    EXPECT_TRUE(token.value() == "and" ||
                token.value() == "AND");
}

TEST_F(TokenTest, ConstructorFromTypeAndValue) {
    using namespace booleval;

    token::token token(token::token_type::field, "field");
    EXPECT_EQ(token.type(), token::token_type::field);
    EXPECT_EQ(token.value(), "field");
}

TEST_F(TokenTest, Type) {
    using namespace booleval;

    token::token token;
    token.type(token::token_type::logical_and);
    EXPECT_EQ(token.type(), token::token_type::logical_and);
}

TEST_F(TokenTest, Value) {
    using namespace booleval;

    token::token token;
    token.value("field");
    EXPECT_EQ(token.value(), "field");
}

TEST_F(TokenTest, ValueAsInt) {
    using namespace booleval;

    token::token token;
    token.value("1");
    EXPECT_EQ(token.value<uint8_t>(), 1U);

    token.value("a");
    EXPECT_EQ(token.value<uint8_t>(), std::nullopt);
}

TEST_F(TokenTest, ValueAsDouble) {
    using namespace booleval;

    token::token token;
    token.value("1.23456789");
    EXPECT_DOUBLE_EQ(token.value<double>().value(), 1.23456789);

    token.value("a");
    EXPECT_EQ(token.value<double>(), std::nullopt);
}

TEST_F(TokenTest, ValueAsFloat) {
    using namespace booleval;

    token::token token;
    token.value("1.23456789");
    EXPECT_FLOAT_EQ(token.value<float>().value(), 1.23456789F);

    token.value("a");
    EXPECT_EQ(token.value<float>(), std::nullopt);
}

TEST_F(TokenTest, IsType) {
    using namespace booleval;

    token::token token(token::token_type::logical_and);
    EXPECT_TRUE(token.is(token::token_type::logical_and));
    EXPECT_FALSE(token.is(token::token_type::logical_or));
}

TEST_F(TokenTest, IsNotType) {
    using namespace booleval;

    token::token token(token::token_type::logical_and);
    EXPECT_TRUE(token.is_not(token::token_type::logical_or));
    EXPECT_FALSE(token.is_not(token::token_type::logical_and));
}

TEST_F(TokenTest, IsOneOfTwoTypes) {
    using namespace booleval;

    token::token token(token::token_type::logical_and);
    EXPECT_TRUE(token.is_one_of(token::token_type::logical_and, token::token_type::logical_or));
    EXPECT_FALSE(token.is_one_of(token::token_type::lp, token::token_type::rp));
}

TEST_F(TokenTest, IsOneOfMoreThanTwoTypes) {
    using namespace booleval;

    token::token token(token::token_type::logical_and);
    EXPECT_TRUE(token.is_one_of(token::token_type::logical_and, token::token_type::lp, token::token_type::rp));
    EXPECT_FALSE(token.is_one_of(token::token_type::logical_or, token::token_type::lp, token::token_type::rp));
}