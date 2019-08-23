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
#include <booleval/token/field_token.h>
#include <booleval/token/token_type.h>

class FieldTokenTest : public testing::Test {};

TEST_F(FieldTokenTest, DefaultConstructor) {
    using namespace booleval::token;

    FieldToken<> token;
    EXPECT_EQ(token.type(), TokenType::FIELD);
    EXPECT_STREQ(token.field().c_str(), "");
}

TEST_F(FieldTokenTest, ConstructorFromField) {
    using namespace booleval::token;

    FieldToken<> token("foo");
    EXPECT_EQ(token.type(), TokenType::FIELD);
    EXPECT_STREQ(token.field().c_str(), "foo");
}

TEST_F(FieldTokenTest, Type) {
    using namespace booleval::token;

    FieldToken<> token;
    token.type(TokenType::AND);
    EXPECT_EQ(token.type(), TokenType::FIELD);
    EXPECT_STREQ(token.field().c_str(), "");
}

TEST_F(FieldTokenTest, Field) {
    using namespace booleval::token;

    FieldToken<> token;
    token.field("foo");
    EXPECT_EQ(token.type(), TokenType::FIELD);
    EXPECT_STREQ(token.field().c_str(), "foo");
}
