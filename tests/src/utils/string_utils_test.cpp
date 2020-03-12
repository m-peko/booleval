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
#include <booleval/utils/string_utils.h>

class StringUtilsTest : public testing::Test {};

TEST_F(StringUtilsTest, IsSetSplitOption) {
    using namespace booleval::utils;

    auto options = split_options::include_whitespace;
    EXPECT_TRUE(is_set(options, split_options::include_whitespace));
    EXPECT_FALSE(is_set(options, split_options::include_delimiters));
    EXPECT_FALSE(is_set(options, split_options::exclude_delimiters));

    options = split_options::include_whitespace |
              split_options::include_delimiters;
    EXPECT_TRUE(is_set(options, split_options::include_whitespace));
    EXPECT_TRUE(is_set(options, split_options::include_delimiters));
    EXPECT_FALSE(is_set(options, split_options::exclude_delimiters));

    options = split_options::include_whitespace |
              split_options::include_delimiters |
              split_options::exclude_delimiters;
    EXPECT_TRUE(is_set(options, split_options::include_whitespace));
    EXPECT_TRUE(is_set(options, split_options::include_delimiters));
    EXPECT_TRUE(is_set(options, split_options::exclude_delimiters));
}

TEST_F(StringUtilsTest, LeftTrim) {
    using namespace booleval::utils;

    EXPECT_EQ(ltrim_copy("abc"), "abc");
    EXPECT_EQ(ltrim_copy(" abc"), "abc");
    EXPECT_EQ(ltrim_copy("  abc"), "abc");
    EXPECT_EQ(ltrim_copy("  abc "), "abc ");
    EXPECT_EQ(ltrim_copy("  a b c "), "a b c ");
}

TEST_F(StringUtilsTest, RightTrim) {
    using namespace booleval::utils;

    EXPECT_EQ(rtrim_copy("abc"), "abc");
    EXPECT_EQ(rtrim_copy("abc "), "abc");
    EXPECT_EQ(rtrim_copy("abc  "), "abc");
    EXPECT_EQ(rtrim_copy(" abc  "), " abc");
    EXPECT_EQ(rtrim_copy(" a b c  "), " a b c");
}

TEST_F(StringUtilsTest, Trim) {
    using namespace booleval::utils;

    EXPECT_EQ(trim_copy("abc"), "abc");
    EXPECT_EQ(trim_copy(" abc "), "abc");
    EXPECT_EQ(trim_copy("  abc  "), "abc");
    EXPECT_EQ(trim_copy("  a b c  "), "a b c");
}

TEST_F(StringUtilsTest, IsEmpty) {
    using namespace booleval::utils;

    EXPECT_TRUE(is_empty(""));
    EXPECT_TRUE(is_empty(" "));
    EXPECT_TRUE(is_empty("  "));

    EXPECT_FALSE(is_empty("abc"));
    EXPECT_FALSE(is_empty(" abc"));
    EXPECT_FALSE(is_empty("abc "));
    EXPECT_FALSE(is_empty(" abc "));
    EXPECT_FALSE(is_empty(" a b c "));
}

TEST_F(StringUtilsTest, SplitByWhitespace) {
    using namespace booleval::utils;

    auto tokens = split("a b c d");

    EXPECT_EQ(tokens.size(), 4U);
    EXPECT_EQ(tokens[0], "a");
    EXPECT_EQ(tokens[1], "b");
    EXPECT_EQ(tokens[2], "c");
    EXPECT_EQ(tokens[3], "d");
}

TEST_F(StringUtilsTest, SplitByComma) {
    using namespace booleval::utils;

    auto tokens = split("a,b,c,d", ",");

    EXPECT_EQ(tokens.size(), 4U);
    EXPECT_EQ(tokens[0], "a");
    EXPECT_EQ(tokens[1], "b");
    EXPECT_EQ(tokens[2], "c");
    EXPECT_EQ(tokens[3], "d");
}

TEST_F(StringUtilsTest, SplitByMultipleDelimiters) {
    using namespace booleval::utils;

    auto tokens = split("a,b.c,d.", ".,");

    EXPECT_EQ(tokens.size(), 4U);
    EXPECT_EQ(tokens[0], "a");
    EXPECT_EQ(tokens[1], "b");
    EXPECT_EQ(tokens[2], "c");
    EXPECT_EQ(tokens[3], "d");
}

TEST_F(StringUtilsTest, SplitIncludeDelimitersOption) {
    using namespace booleval::utils;

    auto tokens = split("a,b.", ".,", split_options::include_delimiters);

    EXPECT_EQ(tokens.size(), 4U);
    EXPECT_EQ(tokens[0], "a");
    EXPECT_EQ(tokens[1], ",");
    EXPECT_EQ(tokens[2], "b");
    EXPECT_EQ(tokens[3], ".");
}

TEST_F(StringUtilsTest, SplitMultipleOptions) {
    using namespace booleval::utils;

    auto options = split_options::include_whitespace |
                   split_options::include_delimiters;
    auto tokens = split("a , b .", ".,", options);

    EXPECT_EQ(tokens.size(), 4U);
    EXPECT_EQ(tokens[0], "a");
    EXPECT_EQ(tokens[1], ",");
    EXPECT_EQ(tokens[2], "b");
    EXPECT_EQ(tokens[3], ".");
}

TEST_F(StringUtilsTest, JoinWithoutSeparator) {
    using namespace booleval::utils;

    auto tokens = { "a", "b", "c", "d" };
    auto result = join(std::begin(tokens), std::end(tokens));
    EXPECT_EQ(result, "abcd");
}

TEST_F(StringUtilsTest, JoinWithCommaSeparator) {
    using namespace booleval::utils;

    auto tokens = { "a", "b", "c", "d" };
    auto result = join(std::begin(tokens), std::end(tokens), ",");
    EXPECT_EQ(result, "a,b,c,d");
}
