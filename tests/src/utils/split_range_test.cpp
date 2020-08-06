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

class SplitRangeTest : public testing::Test {
public:
    template <typename SplitRangeIt>
    void test_split_range_iterator(SplitRangeIt it,
                                   bool const quoted,
                                   std::size_t const index,
                                   std::string_view const value) {
        EXPECT_EQ(it->quoted, quoted);
        EXPECT_EQ(it->index, index);
        EXPECT_EQ(it->value, value);
    }
};

TEST_F(SplitRangeTest, SplitEmpty) {
    using namespace booleval::utils;

    auto range = split_range("");
    EXPECT_EQ(range.begin(), range.end());
}

TEST_F(SplitRangeTest, SplitByWhitespace) {
    using namespace booleval::utils;

    auto range = split_range("a b c d \"a b c\"");
    auto it  = range.begin();
    auto end = range.end();

    test_split_range_iterator(it++, false, 0, "a");
    test_split_range_iterator(it++, false, 1, "b");
    test_split_range_iterator(it++, false, 2, "c");
    test_split_range_iterator(it++, false, 3, "d");
    test_split_range_iterator(it++, true,  4, "a b c");
    EXPECT_EQ(it, end);
}

TEST_F(SplitRangeTest, SplitByWhitespaceWithSingleQuoteChar) {
    using namespace booleval::utils;

    auto range = split_range<
        split_options::exclude_delimiters |
        split_options::allow_quoted_strings,
        single_quote_char
    >("a b c d \'a b c\'");
    auto it  = range.begin();
    auto end = range.end();

    test_split_range_iterator(it++, false, 0, "a");
    test_split_range_iterator(it++, false, 1, "b");
    test_split_range_iterator(it++, false, 2, "c");
    test_split_range_iterator(it++, false, 3, "d");
    test_split_range_iterator(it++, true,  4, "a b c");
    EXPECT_EQ(it, end);
}

TEST_F(SplitRangeTest, SplitByComma) {
    using namespace booleval::utils;

    auto range = split_range("\"a,b,c\",a,b,c,d", ",");
    auto it  = range.begin();
    auto end = range.end();

    test_split_range_iterator(it++, true,  0, "a,b,c");
    test_split_range_iterator(it++, false, 1, "a");
    test_split_range_iterator(it++, false, 2, "b");
    test_split_range_iterator(it++, false, 3, "c");
    test_split_range_iterator(it++, false, 4, "d");
    EXPECT_EQ(it, end);
}

TEST_F(SplitRangeTest, SplitByMultipleDelimiters) {
    using namespace booleval::utils;

    auto range = split_range("a,b.\"a.b,c\".c,d.", ".,");
    auto it  = range.begin();
    auto end = range.end();

    test_split_range_iterator(it++, false, 0, "a");
    test_split_range_iterator(it++, false, 1, "b");
    test_split_range_iterator(it++, true,  2, "a.b,c");
    test_split_range_iterator(it++, false, 3, "c");
    test_split_range_iterator(it++, false, 4, "d");
    EXPECT_EQ(it, end);
}

TEST_F(SplitRangeTest, SplitWithIncludeDelimitersOption) {
    using namespace booleval::utils;

    auto range = split_range<
        split_options::include_delimiters
    >("a,b.", ".,");
    auto it  = range.begin();
    auto end = range.end();

    test_split_range_iterator(it++, false, 0, "a");
    test_split_range_iterator(it++, false, 1, ",");
    test_split_range_iterator(it++, false, 2, "b");
    test_split_range_iterator(it++, false, 3, ".");
    EXPECT_EQ(it, end);
}

TEST_F(SplitRangeTest, SplitWithMultipleOptions) {
    using namespace booleval::utils;

    auto range = split_range<
        split_options::include_delimiters  |
        split_options::split_by_whitespace |
        split_options::allow_quoted_strings
    >("a , b .\"c , \" \"d . e\"", ".,");
    auto it  = range.begin();
    auto end = range.end();

    test_split_range_iterator(it++, false, 0, "a");
    test_split_range_iterator(it++, false, 1, ",");
    test_split_range_iterator(it++, false, 2, "b");
    test_split_range_iterator(it++, false, 3, ".");
    test_split_range_iterator(it++, true,  4, "c , ");
    test_split_range_iterator(it++, true,  5, "d . e");
    EXPECT_EQ(it, end);
}

TEST_F(SplitRangeTest, SplitWithMultipleOptionsAndParentheses) {
    using namespace booleval::utils;

    auto range = split_range<
        split_options::include_delimiters  |
        split_options::split_by_whitespace |
        split_options::allow_quoted_strings
    >("(a b c d \"a b c\")", "()");
    auto it  = range.begin();
    auto end = range.end();

    test_split_range_iterator(it++, false, 0, "(");
    test_split_range_iterator(it++, false, 1, "a");
    test_split_range_iterator(it++, false, 2, "b");
    test_split_range_iterator(it++, false, 3, "c");
    test_split_range_iterator(it++, false, 4, "d");
    test_split_range_iterator(it++, true,  5, "a b c");
    test_split_range_iterator(it++, false, 6, ")");
    EXPECT_EQ(it, end);
}