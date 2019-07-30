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
#include <memory>
#include <gtest/gtest.h>
#include <booleval/tokenizer.h>

class TokenizerTest : public testing::Test {};

TEST_F(TokenizerTest, DefaultConstructor) {
    using namespace Booleval;

    Tokenizer tokenizer;
    EXPECT_STREQ(tokenizer.expression().c_str(), "");
    EXPECT_FALSE(tokenizer.has_token());
}

TEST_F(TokenizerTest, ConstructorFromExpression) {
    using namespace Booleval;

    std::string expression = "field_a foo";

    Tokenizer tokenizer(expression);
    EXPECT_STREQ(tokenizer.expression().c_str(), expression.c_str());
    EXPECT_FALSE(tokenizer.has_token());
}

TEST_F(TokenizerTest, Expression) {
    using namespace Booleval;

    std::string expression = "field_a foo";

    Tokenizer tokenizer;
    tokenizer.expression(expression);
    EXPECT_STREQ(tokenizer.expression().c_str(), expression.c_str());
}

TEST_F(TokenizerTest, TokenizeEmptyExpression) {
    using namespace Booleval;

    std::string expression = "";

    Tokenizer tokenizer;
    tokenizer.expression(expression);
    tokenizer.tokenize();

    EXPECT_STREQ(tokenizer.expression().c_str(), expression.c_str());
    EXPECT_FALSE(tokenizer.has_token());
}

TEST_F(TokenizerTest, TokenizeExpression1) {
    using namespace Booleval;

    std::string expression = "(field_a foo and field_b bar) or field_c john";

    Tokenizer tokenizer;
    tokenizer.expression(expression);
    tokenizer.tokenize();

    EXPECT_STREQ(tokenizer.expression().c_str(), expression.c_str());
    EXPECT_EQ(tokenizer.token()->type(), TokenType::LP);

    tokenizer++;
    EXPECT_EQ(tokenizer.token()->type(), TokenType::FIELD_A);

    tokenizer++;
    EXPECT_EQ(tokenizer.token()->type(), TokenType::VALUE);
    auto imsi_token1 = std::dynamic_pointer_cast<Token<std::string>>(tokenizer.token());
    EXPECT_STREQ(imsi_token1->value().c_str(), "foo");

    tokenizer++;
    EXPECT_EQ(tokenizer.token()->type(), TokenType::AND);

    tokenizer++;
    EXPECT_EQ(tokenizer.token()->type(), TokenType::FIELD_B);

    tokenizer++;
    EXPECT_EQ(tokenizer.token()->type(), TokenType::VALUE);
    auto apn_token = std::dynamic_pointer_cast<Token<std::string>>(tokenizer.token());
    EXPECT_STREQ(apn_token->value().c_str(), "bar");

    tokenizer++;
    EXPECT_EQ(tokenizer.token()->type(), TokenType::RP);

    tokenizer++;
    EXPECT_EQ(tokenizer.token()->type(), TokenType::OR);

    tokenizer++;
    EXPECT_EQ(tokenizer.token()->type(), TokenType::FIELD_C);

    tokenizer++;
    EXPECT_EQ(tokenizer.token()->type(), TokenType::VALUE);
    auto imsi_token2 = std::dynamic_pointer_cast<Token<std::string>>(tokenizer.token());
    EXPECT_STREQ(imsi_token2->value().c_str(), "john");
}

TEST_F(TokenizerTest, TokenizeExpression2) {
    using namespace Booleval;

    std::string expression = "(field_a foo && field_b bar) || field_c john";

    Tokenizer tokenizer;
    tokenizer.expression(expression);
    tokenizer.tokenize();

    EXPECT_STREQ(tokenizer.expression().c_str(), expression.c_str());
    EXPECT_EQ(tokenizer.token()->type(), TokenType::LP);

    tokenizer++;
    EXPECT_EQ(tokenizer.token()->type(), TokenType::FIELD_A);

    tokenizer++;
    EXPECT_EQ(tokenizer.token()->type(), TokenType::VALUE);
    auto imsi_token1 = std::dynamic_pointer_cast<Token<std::string>>(tokenizer.token());
    EXPECT_STREQ(imsi_token1->value().c_str(), "foo");

    tokenizer++;
    EXPECT_EQ(tokenizer.token()->type(), TokenType::AND);

    tokenizer++;
    EXPECT_EQ(tokenizer.token()->type(), TokenType::FIELD_B);

    tokenizer++;
    EXPECT_EQ(tokenizer.token()->type(), TokenType::VALUE);
    auto apn_token = std::dynamic_pointer_cast<Token<std::string>>(tokenizer.token());
    EXPECT_STREQ(apn_token->value().c_str(), "bar");

    tokenizer++;
    EXPECT_EQ(tokenizer.token()->type(), TokenType::RP);

    tokenizer++;
    EXPECT_EQ(tokenizer.token()->type(), TokenType::OR);

    tokenizer++;
    EXPECT_EQ(tokenizer.token()->type(), TokenType::FIELD_C);

    tokenizer++;
    EXPECT_EQ(tokenizer.token()->type(), TokenType::VALUE);
    auto imsi_token2 = std::dynamic_pointer_cast<Token<std::string>>(tokenizer.token());
    EXPECT_STREQ(imsi_token2->value().c_str(), "john");
}
