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
    using namespace booleval;

    Token token;
    EXPECT_EQ(token.type(), Token::Type::UNKNOWN);
}

TEST_F(TokenTest, ConstructorFromType) {
    using namespace booleval;

    Token token(Token::Type::AND);
    EXPECT_EQ(token.type(), Token::Type::AND);
}

TEST_F(TokenTest, Type) {
    using namespace booleval;

    Token token;
    token.type(Token::Type::AND);
    EXPECT_EQ(token.type(), Token::Type::AND);
}

TEST_F(TokenTest, IsType) {
    using namespace booleval;

    Token token(Token::Type::AND);
    EXPECT_TRUE(token.is(Token::Type::AND));
    EXPECT_FALSE(token.is(Token::Type::OR));
}

TEST_F(TokenTest, IsNotType) {
    using namespace booleval;

    Token token(Token::Type::AND);
    EXPECT_TRUE(token.is_not(Token::Type::OR));
    EXPECT_FALSE(token.is_not(Token::Type::AND));
}

TEST_F(TokenTest, IsOneOfTwoTypes) {
    using namespace booleval;

    Token token(Token::Type::AND);
    EXPECT_TRUE(token.is_one_of(Token::Type::AND, Token::Type::OR));
    EXPECT_FALSE(token.is_one_of(Token::Type::LP, Token::Type::RP));
}

TEST_F(TokenTest, IsOneOfMoreThanTwoTypes) {
    using namespace booleval;

    Token token(Token::Type::AND);
    EXPECT_TRUE(token.is_one_of(Token::Type::AND, Token::Type::LP, Token::Type::RP));
    EXPECT_FALSE(token.is_one_of(Token::Type::OR, Token::Type::LP, Token::Type::RP));
}