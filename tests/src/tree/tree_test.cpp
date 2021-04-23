/*
 * Copyright (c) 2020, Marin Peko
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

#include <booleval/tree/tree.hpp>

TEST( TreeTest, RelationalOperation )
{
    EXPECT_EQ( booleval::tree::build( "field_a" ), nullptr );

    EXPECT_NE( booleval::tree::build( "field_a foo" ), nullptr );
}

TEST( TreeTest, AndOperation )
{
    EXPECT_EQ( booleval::tree::build( "and"             ), nullptr );
    EXPECT_EQ( booleval::tree::build( " and "           ), nullptr );
    EXPECT_EQ( booleval::tree::build( "field_a foo and" ), nullptr );

    EXPECT_NE( booleval::tree::build( "field_a foo and field_b bar" ), nullptr );
}

TEST( TreeTest, OrOperation )
{
    EXPECT_EQ( booleval::tree::build( "or"             ), nullptr );
    EXPECT_EQ( booleval::tree::build( " or "           ), nullptr );
    EXPECT_EQ( booleval::tree::build( "field_a foo or" ), nullptr );
    EXPECT_EQ( booleval::tree::build( "or field_b bar" ), nullptr );

    EXPECT_NE( booleval::tree::build( "field_a foo or field_b bar" ), nullptr );
}

TEST( TreeTest, Parentheses )
{
    EXPECT_EQ( booleval::tree::build( "(field_a foo or field_b bar"  ), nullptr );
    EXPECT_EQ( booleval::tree::build( "( field_a foo or field_b bar" ), nullptr );
    // EXPECT_EQ( booleval::tree::build( "field_a foo or field_b bar)"  ), nullptr );
    // EXPECT_EQ( booleval::tree::build( "field_a foo or field_b bar )" ), nullptr );

    EXPECT_NE( booleval::tree::build( "(field_a foo or field_b bar)"   ), nullptr );
    EXPECT_NE( booleval::tree::build( "( field_a foo or field_b bar )" ), nullptr );
}
