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
#include <booleval/token/base_token.h>

class BaseTokenTest : public testing::Test {};

TEST_F(BaseTokenTest, DefaultConstructor) {
    using namespace booleval::token;

    BaseToken token;
    EXPECT_EQ(token.type(), TokenType::UNKNOWN);
}

TEST_F(BaseTokenTest, ConstructorFromType) {
    using namespace booleval::token;

    BaseToken token(TokenType::AND);
    EXPECT_EQ(token.type(), TokenType::AND);
}

TEST_F(BaseTokenTest, Type) {
    using namespace booleval::token;

    BaseToken token;
    token.type(TokenType::AND);
    EXPECT_EQ(token.type(), TokenType::AND);
}

TEST_F(BaseTokenTest, IsType) {
    using namespace booleval::token;

    BaseToken token(TokenType::AND);
    EXPECT_TRUE(token.is(TokenType::AND));
    EXPECT_FALSE(token.is(TokenType::OR));
}

TEST_F(BaseTokenTest, IsNotType) {
    using namespace booleval::token;

    BaseToken token(TokenType::AND);
    EXPECT_TRUE(token.is_not(TokenType::OR));
    EXPECT_FALSE(token.is_not(TokenType::AND));
}

TEST_F(BaseTokenTest, IsOneOfTwoTypes) {
    using namespace booleval::token;

    BaseToken token(TokenType::AND);
    EXPECT_TRUE(token.is_one_of(TokenType::AND, TokenType::OR));
    EXPECT_FALSE(token.is_one_of(TokenType::LP, TokenType::RP));
}

TEST_F(BaseTokenTest, IsOneOfMoreThanTwoTypes) {
    using namespace booleval::token;

    BaseToken token(TokenType::AND);
    EXPECT_TRUE(token.is_one_of(TokenType::AND, TokenType::LP, TokenType::RP));
    EXPECT_FALSE(token.is_one_of(TokenType::OR, TokenType::LP, TokenType::RP));
}