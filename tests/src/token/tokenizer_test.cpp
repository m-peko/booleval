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

#include <string>
#include <gtest/gtest.h>
#include <booleval/token/tokenizer.h>
#include <booleval/token/base_token.h>
#include <booleval/token/field_token.h>

class TokenizerTest : public testing::Test {};

TEST_F(TokenizerTest, DefaultConstructor) {
    using namespace booleval::token;

    Tokenizer tokenizer;
    EXPECT_STREQ(tokenizer.expression().c_str(), "");
    EXPECT_FALSE(tokenizer.has_token());
}

TEST_F(TokenizerTest, ConstructorFromExpression) {
    using namespace booleval::token;

    std::string expression = "field foo";

    Tokenizer tokenizer(expression);
    EXPECT_STREQ(tokenizer.expression().c_str(), expression.c_str());
    EXPECT_FALSE(tokenizer.has_token());
}

TEST_F(TokenizerTest, Expression) {
    using namespace booleval::token;

    std::string expression = "field foo";

    Tokenizer tokenizer;
    tokenizer.expression(expression);
    EXPECT_STREQ(tokenizer.expression().c_str(), expression.c_str());
}

TEST_F(TokenizerTest, TokenizeEmptyExpression) {
    using namespace booleval::token;

    std::string expression = "";

    Tokenizer tokenizer;
    tokenizer.expression(expression);
    EXPECT_STREQ(tokenizer.expression().c_str(), expression.c_str());

    tokenizer.tokenize();
    EXPECT_FALSE(tokenizer.has_token());
}

TEST_F(TokenizerTest, TokenizeExpression1) {
    using namespace booleval::token;

    std::string expression = "(field_a foo and field_b neq bar) or field_c greater baz";

    Tokenizer tokenizer;
    tokenizer.expression(expression);
    EXPECT_STREQ(tokenizer.expression().c_str(), expression.c_str());

    tokenizer.tokenize();
    EXPECT_TRUE(tokenizer.token()->is(TokenType::LP));

    tokenizer++;
    EXPECT_TRUE(tokenizer.token()->is(TokenType::FIELD));

    tokenizer++;
    EXPECT_TRUE(tokenizer.token()->is(TokenType::EQ));

    tokenizer++;
    EXPECT_TRUE(tokenizer.token()->is(TokenType::FIELD));
    auto foo_token = std::dynamic_pointer_cast<FieldToken<>>(tokenizer.token());
    EXPECT_STREQ(foo_token->field().c_str(), "foo");

    tokenizer++;
    EXPECT_TRUE(tokenizer.token()->is(TokenType::AND));

    tokenizer++;
    EXPECT_TRUE(tokenizer.token()->is(TokenType::FIELD));

    tokenizer++;
    EXPECT_TRUE(tokenizer.token()->is(TokenType::NEQ));

    tokenizer++;
    EXPECT_TRUE(tokenizer.token()->is(TokenType::FIELD));
    auto bar_token = std::dynamic_pointer_cast<FieldToken<>>(tokenizer.token());
    EXPECT_STREQ(bar_token->field().c_str(), "bar");

    tokenizer++;
    EXPECT_TRUE(tokenizer.token()->is(TokenType::RP));

    tokenizer++;
    EXPECT_TRUE(tokenizer.token()->is(TokenType::OR));

    tokenizer++;
    EXPECT_TRUE(tokenizer.token()->is(TokenType::FIELD));

    tokenizer++;
    EXPECT_TRUE(tokenizer.token()->is(TokenType::GT));

    tokenizer++;
    EXPECT_TRUE(tokenizer.token()->is(TokenType::FIELD));
    auto baz_token = std::dynamic_pointer_cast<FieldToken<>>(tokenizer.token());
    EXPECT_STREQ(baz_token->field().c_str(), "baz");
}

TEST_F(TokenizerTest, TokenizeExpression2) {
    using namespace booleval::token;

    std::string expression = "(field_a foo && field_b != bar) || field_c > baz";

    Tokenizer tokenizer;
    tokenizer.expression(expression);
    EXPECT_STREQ(tokenizer.expression().c_str(), expression.c_str());

    tokenizer.tokenize();
    EXPECT_TRUE(tokenizer.token()->is(TokenType::LP));

    tokenizer++;
    EXPECT_TRUE(tokenizer.token()->is(TokenType::FIELD));

    tokenizer++;
    EXPECT_TRUE(tokenizer.token()->is(TokenType::EQ));

    tokenizer++;
    EXPECT_TRUE(tokenizer.token()->is(TokenType::FIELD));
    auto foo_token = std::dynamic_pointer_cast<FieldToken<>>(tokenizer.token());
    EXPECT_STREQ(foo_token->field().c_str(), "foo");

    tokenizer++;
    EXPECT_TRUE(tokenizer.token()->is(TokenType::AND));

    tokenizer++;
    EXPECT_TRUE(tokenizer.token()->is(TokenType::FIELD));

    tokenizer++;
    EXPECT_TRUE(tokenizer.token()->is(TokenType::NEQ));

    tokenizer++;
    EXPECT_TRUE(tokenizer.token()->is(TokenType::FIELD));
    auto bar_token = std::dynamic_pointer_cast<FieldToken<>>(tokenizer.token());
    EXPECT_STREQ(bar_token->field().c_str(), "bar");

    tokenizer++;
    EXPECT_TRUE(tokenizer.token()->is(TokenType::RP));

    tokenizer++;
    EXPECT_TRUE(tokenizer.token()->is(TokenType::OR));

    tokenizer++;
    EXPECT_TRUE(tokenizer.token()->is(TokenType::FIELD));

    tokenizer++;
    EXPECT_TRUE(tokenizer.token()->is(TokenType::GT));

    tokenizer++;
    EXPECT_TRUE(tokenizer.token()->is(TokenType::FIELD));
    auto baz_token = std::dynamic_pointer_cast<FieldToken<>>(tokenizer.token());
    EXPECT_STREQ(baz_token->field().c_str(), "baz");
}
