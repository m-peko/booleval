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

#include <booleval/token/tokenizer.hpp>
#include <booleval/token/token_type.hpp>

TEST( TokenizerTest, EmptyExpression )
{
    EXPECT_TRUE( booleval::token::tokenize( ""   ).empty() );
    EXPECT_TRUE( booleval::token::tokenize( " "  ).empty() );
    EXPECT_TRUE( booleval::token::tokenize( "  " ).empty() );
}

TEST( TokenizerTest, KeywordAsFieldExpression )
{
    auto const tokens{ booleval::token::tokenize( "field_a \"and\"" ) };
    EXPECT_FALSE( tokens.empty() );

    EXPECT_TRUE( tokens[ 0 ].is( booleval::token::token_type::field ) );
    EXPECT_TRUE( tokens[ 1 ].is( booleval::token::token_type::eq    ) );
    EXPECT_TRUE( tokens[ 2 ].is( booleval::token::token_type::field ) );
    EXPECT_EQ  ( tokens[ 2 ].value(), "and" );
}

TEST( TokenizerTest, LowercaseKeywordExpression )
{
    auto const tokens{ booleval::token::tokenize( "(field_a foo and field_b neq bar) or field_c eq baz" ) };
    EXPECT_FALSE( tokens.empty() );

    EXPECT_TRUE( tokens[ 0 ].is( booleval::token::token_type::lp    ) );
    EXPECT_TRUE( tokens[ 1 ].is( booleval::token::token_type::field ) );
    EXPECT_TRUE( tokens[ 2 ].is( booleval::token::token_type::eq    ) );

    EXPECT_TRUE( tokens[ 3 ].is( booleval::token::token_type::field ) );
    EXPECT_EQ  ( tokens[ 3 ].value(), "foo" );

    EXPECT_TRUE( tokens[ 4 ].is( booleval::token::token_type::logical_and ) );
    EXPECT_TRUE( tokens[ 5 ].is( booleval::token::token_type::field       ) );
    EXPECT_TRUE( tokens[ 6 ].is( booleval::token::token_type::neq         ) );

    EXPECT_TRUE( tokens[ 7 ].is( booleval::token::token_type::field ) );
    EXPECT_EQ  ( tokens[ 7 ].value(), "bar" );

    EXPECT_TRUE( tokens[  8 ].is( booleval::token::token_type::rp         ) );
    EXPECT_TRUE( tokens[  9 ].is( booleval::token::token_type::logical_or ) );
    EXPECT_TRUE( tokens[ 10 ].is( booleval::token::token_type::field      ) );
    EXPECT_TRUE( tokens[ 11 ].is( booleval::token::token_type::eq         ) );

    EXPECT_TRUE( tokens[ 12 ].is( booleval::token::token_type::field ) );
    EXPECT_EQ  ( tokens[ 12 ].value(), "baz" );
}

TEST( TokenizerTest, UppercaseKeywordExpression )
{
    auto const tokens{ booleval::token::tokenize( "(field_a foo AND field_b NEQ bar) OR field_c EQ baz" ) };
    EXPECT_FALSE( tokens.empty() );

    EXPECT_TRUE( tokens[ 0 ].is( booleval::token::token_type::lp    ) );
    EXPECT_TRUE( tokens[ 1 ].is( booleval::token::token_type::field ) );
    EXPECT_TRUE( tokens[ 2 ].is( booleval::token::token_type::eq    ) );

    EXPECT_TRUE( tokens[ 3 ].is( booleval::token::token_type::field ) );
    EXPECT_EQ  ( tokens[ 3 ].value(), "foo" );

    EXPECT_TRUE( tokens[ 4 ].is( booleval::token::token_type::logical_and ) );
    EXPECT_TRUE( tokens[ 5 ].is( booleval::token::token_type::field       ) );
    EXPECT_TRUE( tokens[ 6 ].is( booleval::token::token_type::neq         ) );

    EXPECT_TRUE( tokens[ 7 ].is( booleval::token::token_type::field ) );
    EXPECT_EQ  ( tokens[ 7 ].value(), "bar" );

    EXPECT_TRUE( tokens[  8 ].is( booleval::token::token_type::rp         ) );
    EXPECT_TRUE( tokens[  9 ].is( booleval::token::token_type::logical_or ) );
    EXPECT_TRUE( tokens[ 10 ].is( booleval::token::token_type::field      ) );
    EXPECT_TRUE( tokens[ 11 ].is( booleval::token::token_type::eq         ) );

    EXPECT_TRUE( tokens[ 12 ].is( booleval::token::token_type::field ) );
    EXPECT_EQ  ( tokens[ 12 ].value(), "baz" );
}

TEST( TokenizerTest, SymbolExpression )
{
    auto const tokens{ booleval::token::tokenize( "(field_a foo && field_b != bar) || field_c > baz" ) };
    EXPECT_FALSE( tokens.empty() );

    EXPECT_TRUE( tokens[ 0 ].is( booleval::token::token_type::lp    ) );
    EXPECT_TRUE( tokens[ 1 ].is( booleval::token::token_type::field ) );
    EXPECT_TRUE( tokens[ 2 ].is( booleval::token::token_type::eq    ) );

    EXPECT_TRUE( tokens[ 3 ].is( booleval::token::token_type::field ) );
    EXPECT_EQ  ( tokens[ 3 ].value(), "foo" );

    EXPECT_TRUE( tokens[ 4 ].is( booleval::token::token_type::logical_and ) );
    EXPECT_TRUE( tokens[ 5 ].is( booleval::token::token_type::field       ) );
    EXPECT_TRUE( tokens[ 6 ].is( booleval::token::token_type::neq         ) );

    EXPECT_TRUE( tokens[ 7 ].is( booleval::token::token_type::field ) );
    EXPECT_EQ  ( tokens[ 7 ].value(), "bar" );

    EXPECT_TRUE( tokens[  8 ].is( booleval::token::token_type::rp         ) );
    EXPECT_TRUE( tokens[  9 ].is( booleval::token::token_type::logical_or ) );
    EXPECT_TRUE( tokens[ 10 ].is( booleval::token::token_type::field      ) );
    EXPECT_TRUE( tokens[ 11 ].is( booleval::token::token_type::gt         ) );

    EXPECT_TRUE( tokens[ 12 ].is( booleval::token::token_type::field ) );
    EXPECT_EQ  ( tokens[ 12 ].value(), "baz" );
}
