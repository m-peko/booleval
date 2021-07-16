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

TEST( TokenTest, DefaultConstructor )
{
    booleval::token::token token{};
    ASSERT_EQ( token.type(), booleval::token::token_type::unknown );
    ASSERT_EQ( token.value(), "" );
}

TEST( TokenTest, ConstructorFromType )
{
    booleval::token::token token{ booleval::token::token_type::logical_and };
    ASSERT_EQ( token.type(), booleval::token::token_type::logical_and );
    ASSERT_TRUE
    (
        token.value() == "and" ||
        token.value() == "AND"
    );
}

TEST( TokenTest, ConstructorFromValue )
{
    booleval::token::token token("and");
    ASSERT_EQ( token.type(), booleval::token::token_type::logical_and );
    ASSERT_TRUE
    (
        token.value() == "and" ||
        token.value() == "AND"
    );
}

TEST( TokenTest, ConstructorFromTypeAndValue )
{
    using namespace booleval;

    booleval::token::token token{ booleval::token::token_type::field, "field" };
    ASSERT_EQ( token.type(), booleval::token::token_type::field );
    ASSERT_EQ( token.value(), "field" );
}

TEST( TokenTest, Type )
{
    using namespace booleval;

    booleval::token::token token{};
    token.type( booleval::token::token_type::logical_and );
    ASSERT_EQ( token.type(), booleval::token::token_type::logical_and );
}

TEST( TokenTest, Value )
{
    booleval::token::token token{};
    token.value( "field" );
    ASSERT_EQ( token.value(), "field" );
}

TEST( TokenTest, ValueAsInt )
{
    {
        booleval::token::token token{};
        token.value( "1" );
        ASSERT_EQ( token.value< std::uint8_t >(), 1U );
    }
    {
        booleval::token::token token{};
        token.value( "a" );
        ASSERT_EQ( token.value< std::uint8_t >(), std::nullopt );
    }
}

TEST( TokenTest, ValueAsDouble )
{
    {
        booleval::token::token token{};
        token.value( "1.23456789" );
        ASSERT_DOUBLE_EQ( token.value< double >().value(), 1.23456789 );
    }
    {
        booleval::token::token token{};
        token.value( "1.2a" );
        ASSERT_EQ( token.value< double >(), std::nullopt );
    }
}

TEST( TokenTest, ValueAsFloat )
{
    {
        booleval::token::token token{};
        token.value( "1.23456789" );
        ASSERT_FLOAT_EQ( token.value< float >().value(), 1.23456789F );
    }
    {
        booleval::token::token token{};
        token.value( "1.2a" );
        ASSERT_EQ( token.value< float >(), std::nullopt );
    }
}

TEST( TokenTest, IsType )
{
    booleval::token::token token{ booleval::token::token_type::logical_and };
    ASSERT_TRUE ( token.is( booleval::token::token_type::logical_and ) );
    ASSERT_FALSE( token.is( booleval::token::token_type::logical_or  ) );
}

TEST( TokenTest, IsNotType )
{
    booleval::token::token token{ booleval::token::token_type::logical_and };
    ASSERT_TRUE ( token.is_not( booleval::token::token_type::logical_or  ) );
    ASSERT_FALSE( token.is_not( booleval::token::token_type::logical_and ) );
}

TEST( TokenTest, IsOneOfTwoTypes )
{
    booleval::token::token token{ booleval::token::token_type::logical_and };
    ASSERT_TRUE
    (
        token.is_one_of
        (
            booleval::token::token_type::logical_and,
            booleval::token::token_type::logical_or
        )
    );
    ASSERT_FALSE
    (
        token.is_one_of
        (
            booleval::token::token_type::lp,
            booleval::token::token_type::rp
        )
    );
}

TEST( TokenTest, IsOneOfMoreTypes )
{
    booleval::token::token token{ booleval::token::token_type::logical_and };
    ASSERT_TRUE
    (
        token.is_one_of
        (
            booleval::token::token_type::logical_and,
            booleval::token::token_type::lp,
            booleval::token::token_type::rp
        )
    );

    ASSERT_FALSE
    (
        token.is_one_of
        (
            booleval::token::token_type::logical_or,
            booleval::token::token_type::lp,
            booleval::token::token_type::rp
        )
    );
}