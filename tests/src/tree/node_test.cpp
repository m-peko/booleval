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

#include <booleval/token/token_type.hpp>
#include <booleval/token/token.hpp>
#include <booleval/tree/node.hpp>

TEST( NodeTest, DefaultConstructor )
{
    booleval::tree::node node{};
    EXPECT_EQ( node.token.type(), booleval::token::token_type::unknown );
    EXPECT_EQ( node.left , nullptr );
    EXPECT_EQ( node.right, nullptr );
}

TEST( NodeTest, ConstructorFromTokenType )
{
    booleval::tree::node node{ booleval::token::token_type::logical_and };
    EXPECT_EQ( node.token.type(), booleval::token::token_type::logical_and );
    EXPECT_EQ( node.left , nullptr );
    EXPECT_EQ( node.right, nullptr );
}

TEST( NodeTest, ConstructorFromToken )
{
    {
        booleval::token::token and_token{ booleval::token::token_type::logical_and };
        booleval::tree::node node{ and_token };
        EXPECT_EQ( node.token.type(), booleval::token::token_type::logical_and );
        EXPECT_EQ( node.left , nullptr );
        EXPECT_EQ( node.right, nullptr );
    }
    {
        booleval::token::token field_token{ "foo" };
        booleval::tree::node node{ field_token };
        EXPECT_EQ( node.token.value(), "foo" );
        EXPECT_EQ( node.token.type(), booleval::token::token_type::field );
        EXPECT_EQ( node.left , nullptr );
        EXPECT_EQ( node.right, nullptr );
    }
}
