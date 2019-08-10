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

#include <string>
#include <gtest/gtest.h>
#include <booleval/utils/regex_pattern.h>
#include <booleval/utils/string_utils.h>

class StringUtilsTest : public testing::Test {};

TEST_F(StringUtilsTest, LeftTrim) {
    using namespace booleval::utils;

    EXPECT_STREQ(ltrim_copy("abc").c_str(), "abc");
    EXPECT_STREQ(ltrim_copy(" abc").c_str(), "abc");
    EXPECT_STREQ(ltrim_copy("  abc").c_str(), "abc");
    EXPECT_STREQ(ltrim_copy("  abc ").c_str(), "abc ");
}

TEST_F(StringUtilsTest, RightTrim) {
    using namespace booleval::utils;

    EXPECT_STREQ(rtrim_copy("abc").c_str(), "abc");
    EXPECT_STREQ(rtrim_copy("abc ").c_str(), "abc");
    EXPECT_STREQ(rtrim_copy("abc  ").c_str(), "abc");
    EXPECT_STREQ(rtrim_copy(" abc  ").c_str(), " abc");
}

TEST_F(StringUtilsTest, Trim) {
    using namespace booleval::utils;

    EXPECT_STREQ(trim_copy("abc").c_str(), "abc");
    EXPECT_STREQ(trim_copy(" abc ").c_str(), "abc");
    EXPECT_STREQ(trim_copy("  abc  ").c_str(), "abc");
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
}

TEST_F(StringUtilsTest, Split) {
    using namespace booleval::utils;

    RegexPattern pattern;
    pattern.match_whitespaces();

    std::string expression = "foo and bar or baz";
    auto tokens = split(expression, pattern.to_string());

    EXPECT_STREQ(tokens[0].c_str(), "foo");
    EXPECT_STREQ(tokens[1].c_str(), "and");
    EXPECT_STREQ(tokens[2].c_str(), "bar");
    EXPECT_STREQ(tokens[3].c_str(), "or");
    EXPECT_STREQ(tokens[4].c_str(), "baz");
}