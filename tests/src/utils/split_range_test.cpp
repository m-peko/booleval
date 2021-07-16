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
#include <booleval/utils/split_range.hpp>

namespace
{

    template< typename It >
    void test_split_range_iterator
    (
        It               const it,
        bool             const expected_is_quoted,
        std::string_view const expected_value
    ) noexcept
    {
        ASSERT_EQ( it->is_quoted, expected_is_quoted );
        ASSERT_EQ( it->value    , expected_value     );
    }

} // namespace

TEST( SplitRangeTest, SplitEmpty )
{
    auto const range{ booleval::utils::split_range( "" ) };
    ASSERT_EQ( std::begin( range ), std::end( range ) );
}

TEST( SplitRangeTest, SplitByWhitespace )
{
    auto const range{ booleval::utils::split_range( "a b c d \"a b c\"" ) };
    auto       it { range.begin() };
    auto const end{ range.end()   };

    test_split_range_iterator( it++, false, "a"      );
    test_split_range_iterator( it++, false, "b"      );
    test_split_range_iterator( it++, false, "c"      );
    test_split_range_iterator( it++, false, "d"      );
    test_split_range_iterator( it++, true ,  "a b c" );
    ASSERT_EQ( it, end );
}

TEST( SplitRangeTest, SplitByWhitespaceWithSingleQuoteChar )
{
    auto const range
    {
        booleval::utils::split_range
        <
            booleval::utils::split_options::exclude_delimiters |
            booleval::utils::split_options::allow_quoted_strings,
            booleval::utils::single_quote_char
        >( "a b c d \'a b c\'" )
    };
    auto       it { range.begin() };
    auto const end{ range.end()   };

    test_split_range_iterator( it++, false, "a"     );
    test_split_range_iterator( it++, false, "b"     );
    test_split_range_iterator( it++, false, "c"     );
    test_split_range_iterator( it++, false, "d"     );
    test_split_range_iterator( it++, true , "a b c" );
    ASSERT_EQ( it, end );
}

TEST( SplitRangeTest, SplitByComma )
{
    auto const range{ booleval::utils::split_range( "\"a,b,c\",a,b,c,d", "," ) };
    auto       it { range.begin() };
    auto const end{ range.end()   };

    test_split_range_iterator( it++, true ,  "a,b,c" );
    test_split_range_iterator( it++, false, "a"      );
    test_split_range_iterator( it++, false, "b"      );
    test_split_range_iterator( it++, false, "c"      );
    test_split_range_iterator( it++, false, "d"      );
    ASSERT_EQ( it, end );
}

TEST( SplitRangeTest, SplitByMultipleDelimiters )
{
    auto const range{ booleval::utils::split_range( "a,b.\"a.b,c\".c,d.", ".," ) };
    auto       it { range.begin() };
    auto const end{ range.end()   };

    test_split_range_iterator( it++, false, "a"     );
    test_split_range_iterator( it++, false, "b"     );
    test_split_range_iterator( it++, true , "a.b,c" );
    test_split_range_iterator( it++, false, "c"     );
    test_split_range_iterator( it++, false, "d"     );
    ASSERT_EQ( it, end );
}

TEST( SplitRangeTest, SplitWithIncludeDelimitersOption )
{
    using namespace booleval::utils;

    auto const range
    {
        split_range
        <
            split_options::include_delimiters
        >( "a,b.", ".," )
    };
    auto       it { range.begin() };
    auto const end{ range.end()   };

    test_split_range_iterator( it++, false, "a" );
    test_split_range_iterator( it++, false, "," );
    test_split_range_iterator( it++, false, "b" );
    test_split_range_iterator( it++, false, "." );
    ASSERT_EQ( it, end );
}

TEST( SplitRangeTest, SplitWithMultipleOptions )
{
    auto const range
    {
        booleval::utils::split_range
        <
            booleval::utils::split_options::include_delimiters  |
            booleval::utils::split_options::split_by_whitespace |
            booleval::utils::split_options::allow_quoted_strings
        >( "a , b .\"c , \" \"d . e\"", ".," )
    };
    auto       it { range.begin() };
    auto const end{ range.end()   };

    test_split_range_iterator( it++, false, "a"     );
    test_split_range_iterator( it++, false, ","     );
    test_split_range_iterator( it++, false, "b"     );
    test_split_range_iterator( it++, false, "."     );
    test_split_range_iterator( it++, true , "c , "  );
    test_split_range_iterator( it++, true , "d . e" );
    ASSERT_EQ( it, end );
}

TEST( SplitRangeTest, SplitWithMultipleOptionsAndParentheses )
{
    auto const range
    {
        booleval::utils::split_range
        <
            booleval::utils::split_options::include_delimiters  |
            booleval::utils::split_options::split_by_whitespace |
            booleval::utils::split_options::allow_quoted_strings
        >( "(a b c d \"a b c\")", "()" )
    };
    auto       it { range.begin() };
    auto const end{ range.end()   };

    test_split_range_iterator( it++, false, "("     );
    test_split_range_iterator( it++, false, "a"     );
    test_split_range_iterator( it++, false, "b"     );
    test_split_range_iterator( it++, false, "c"     );
    test_split_range_iterator( it++, false, "d"     );
    test_split_range_iterator( it++, true , "a b c" );
    test_split_range_iterator( it++, false, ")"     );
    ASSERT_EQ( it, end );
}