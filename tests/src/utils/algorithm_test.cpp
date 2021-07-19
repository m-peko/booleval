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

#include <array>
#include <gtest/gtest.h>
#include <booleval/utils/algo_utils.hpp>

TEST( AlgoUtilsTest, FindIf )
{
    using namespace booleval;

    std::array arr{ 1, 2, 3, 4, 5 };

    {
        auto const it{ utils::find_if( std::begin( arr ), std::end( arr ), []( auto && i ) { return i == 1; } ) };
        ASSERT_NE(  it, std::end( arr ) );
        ASSERT_EQ( *it, 1               );
    }
    {
        auto const it{ utils::find_if( std::begin( arr ), std::end( arr ), []( auto && i ) { return i == 3; } ) };
        ASSERT_NE(  it, std::end( arr ) );
        ASSERT_EQ( *it, 3               );
    }
    {
        auto const it{ utils::find_if( std::begin( arr ), std::end( arr ), []( auto && i ) { return i == 5; } ) };
        ASSERT_NE(  it, std::end( arr ) );
        ASSERT_EQ( *it, 5               );
    }
    {
        auto const it{ utils::find_if( std::begin( arr ), std::end( arr ), []( auto && i ) { return i == 9; } ) };
        ASSERT_EQ(  it, std::end( arr ) );
    }
}

TEST( AlgoUtilsTest, CountIf )
{
    using namespace booleval;

    std::array arr{ 1, 2, 3, 4, 5 };

    {
        auto const count{ utils::count_if( std::begin( arr ), std::end( arr ), []( auto && i ) { return i > 6; } ) };
        ASSERT_EQ( count, 0 );
    }
    {
        auto const count{ utils::count_if( std::begin( arr ), std::end( arr ), []( auto && i ) { return i < 6; } ) };
        ASSERT_EQ( count, 5 );
    }
    {
        arr = { 1, 2, 2, 2, 5 };
        auto const count{ utils::count_if( std::begin( arr ), std::end( arr ), []( auto && i ) { return i == 2; } ) };
        ASSERT_EQ( count, 3 );
    }
}
