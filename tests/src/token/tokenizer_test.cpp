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

#include <string_view>
#include <gtest/gtest.h>
#include <booleval/token/tokenizer.h>
#include <booleval/token/token_type.h>

class TokenizerTest : public testing::Test {};

TEST_F(TokenizerTest, ConstructorFromExpression) {
    using namespace booleval;

    std::string_view expression{ "field foo" };
    token::tokenizer tokenizer(expression);
    EXPECT_EQ(tokenizer.expression(), expression);
    EXPECT_FALSE(tokenizer.has_tokens());
}

TEST_F(TokenizerTest, Expression) {
    using namespace booleval;

    std::string_view expression{ "field foo" };
    token::tokenizer tokenizer;
    tokenizer.expression(expression);
    EXPECT_EQ(tokenizer.expression(), expression);
}

TEST_F(TokenizerTest, TokenizeEmptyExpression) {
    using namespace booleval;

    std::string_view expression;

    token::tokenizer tokenizer;
    tokenizer.expression(expression);
    EXPECT_EQ(tokenizer.expression(), expression);

    tokenizer.tokenize();
    EXPECT_FALSE(tokenizer.has_tokens());
}

TEST_F(TokenizerTest, TokenizeKeywordBasedExpression) {
    using namespace booleval;

    std::string_view expression{ "(field_a foo and field_b neq bar) or field_c eq baz" };

    token::tokenizer tokenizer;
    tokenizer.expression(expression);
    EXPECT_EQ(tokenizer.expression(), expression);

    tokenizer.tokenize();
    EXPECT_TRUE(tokenizer.has_tokens());

    EXPECT_TRUE(tokenizer.next_token().is(token::token_type::lp));
    EXPECT_TRUE(tokenizer.next_token().is(token::token_type::field));
    EXPECT_TRUE(tokenizer.next_token().is(token::token_type::eq));

    EXPECT_TRUE(tokenizer.weak_next_token().is(token::token_type::field));
    EXPECT_EQ(tokenizer.next_token().value(), "foo");

    EXPECT_TRUE(tokenizer.next_token().is(token::token_type::logical_and));
    EXPECT_TRUE(tokenizer.next_token().is(token::token_type::field));
    EXPECT_TRUE(tokenizer.next_token().is(token::token_type::neq));

    EXPECT_TRUE(tokenizer.weak_next_token().is(token::token_type::field));
    EXPECT_EQ(tokenizer.next_token().value(), "bar");

    EXPECT_TRUE(tokenizer.next_token().is(token::token_type::rp));
    EXPECT_TRUE(tokenizer.next_token().is(token::token_type::logical_or));
    EXPECT_TRUE(tokenizer.next_token().is(token::token_type::field));
    EXPECT_TRUE(tokenizer.next_token().is(token::token_type::eq));

    EXPECT_TRUE(tokenizer.weak_next_token().is(token::token_type::field));
    EXPECT_EQ(tokenizer.next_token().value(), "baz");
}

TEST_F(TokenizerTest, TokenizeSymbolBasedExpression) {
    using namespace booleval;

    std::string_view expression{ "(field_a foo && field_b != bar) || field_c > baz" };

    token::tokenizer tokenizer;
    tokenizer.expression(expression);
    EXPECT_EQ(tokenizer.expression(), expression);

    tokenizer.tokenize();
    EXPECT_TRUE(tokenizer.has_tokens());

    EXPECT_TRUE(tokenizer.next_token().is(token::token_type::lp));
    EXPECT_TRUE(tokenizer.next_token().is(token::token_type::field));
    EXPECT_TRUE(tokenizer.next_token().is(token::token_type::eq));

    EXPECT_TRUE(tokenizer.weak_next_token().is(token::token_type::field));
    EXPECT_EQ(tokenizer.next_token().value(), "foo");

    EXPECT_TRUE(tokenizer.next_token().is(token::token_type::logical_and));
    EXPECT_TRUE(tokenizer.next_token().is(token::token_type::field));
    EXPECT_TRUE(tokenizer.next_token().is(token::token_type::neq));

    EXPECT_TRUE(tokenizer.weak_next_token().is(token::token_type::field));
    EXPECT_EQ(tokenizer.next_token().value(), "bar");

    EXPECT_TRUE(tokenizer.next_token().is(token::token_type::rp));
    EXPECT_TRUE(tokenizer.next_token().is(token::token_type::logical_or));
    EXPECT_TRUE(tokenizer.next_token().is(token::token_type::field));
    EXPECT_TRUE(tokenizer.next_token().is(token::token_type::gt));

    EXPECT_TRUE(tokenizer.weak_next_token().is(token::token_type::field));
    EXPECT_EQ(tokenizer.next_token().value(), "baz");
}
