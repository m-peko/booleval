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
#include <booleval/token.h>

class TokenTest : public testing::Test {};

TEST_F(TokenTest, DefaultConstructor) {
    using namespace Booleval;

    Token<uint8_t> token;
    EXPECT_EQ(token.type(), TokenType::UNKNOWN);
    EXPECT_EQ(token.value(), 0U);
}

TEST_F(TokenTest, ConstructorFromType) {
    using namespace Booleval;

    Token<uint8_t> token(TokenType::AND);
    EXPECT_EQ(token.type(), TokenType::AND);
    EXPECT_EQ(token.value(), 0U);
}

TEST_F(TokenTest, ConstructorFromTypeAndValue) {
    using namespace Booleval;

    Token<uint8_t> token(TokenType::VALUE, 1);
    EXPECT_EQ(token.type(), TokenType::VALUE);
    EXPECT_EQ(token.value(), 1U);
}

TEST_F(TokenTest, CopyConstructor) {
    using namespace Booleval;

    Token<uint8_t> token1(TokenType::VALUE, 1);
    Token<uint8_t> token2(token1);
    EXPECT_EQ(token1.type(), token2.type());
    EXPECT_EQ(token1.value(), token2.value());
}

TEST_F(TokenTest, Type) {
    using namespace Booleval;

    Token<uint8_t> token;
    token.type(TokenType::AND);
    EXPECT_EQ(token.type(), TokenType::AND);
}

TEST_F(TokenTest, Value) {
    using namespace Booleval;

    Token<uint8_t> token;
    token.value(1);
    EXPECT_EQ(token.value(), 1U);
}

TEST_F(TokenTest, IsTokenType) {
    using namespace Booleval;

    BaseToken token(TokenType::AND);
    EXPECT_TRUE(token.is(TokenType::AND));
    EXPECT_FALSE(token.is(TokenType::OR));
}

TEST_F(TokenTest, IsNotTokenType) {
    using namespace Booleval;

    BaseToken token(TokenType::AND);
    EXPECT_FALSE(token.is_not(TokenType::AND));
    EXPECT_TRUE(token.is_not(TokenType::OR));
}

TEST_F(TokenTest, IsOneOfTwoTokenTypes) {
    using namespace Booleval;

    BaseToken token(TokenType::AND);
    EXPECT_TRUE(token.is_one_of(TokenType::AND, TokenType::OR));
    EXPECT_FALSE(token.is_one_of(TokenType::LP, TokenType::RP));
}

TEST_F(TokenTest, IsOneOfMultipleTokenTypes) {
    using namespace Booleval;

    BaseToken token(TokenType::AND);
    EXPECT_TRUE(token.is_one_of(TokenType::AND, TokenType::LP, TokenType::RP));
    EXPECT_FALSE(token.is_one_of(TokenType::OR, TokenType::LP, TokenType::RP));
}

TEST_F(TokenTest, IsFieldType) {
    using namespace Booleval;

    BaseToken token(TokenType::FIELD_A);
    EXPECT_TRUE(token.is_field_type());

    token.type(TokenType::AND);
    EXPECT_FALSE(token.is_field_type());
}