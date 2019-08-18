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
#include <booleval/utils/regex_pattern.h>

class RegexPatternTest : public testing::Test {};

TEST_F(RegexPatternTest, EmptyPattern) {
    using namespace booleval::utils;

    RegexPattern pattern;
    EXPECT_STREQ(pattern.to_string().c_str(), "");
}

TEST_F(RegexPatternTest, MatchWhitespaces) {
    using namespace booleval::utils;

    RegexPattern pattern;
    pattern.match_whitespaces();

    EXPECT_STREQ(pattern.to_string().c_str(), "\\s+");
}

TEST_F(RegexPatternTest, MatchExpression) {
    using namespace booleval::utils;

    RegexPattern pattern;
    pattern.match_expression("\\s+");

    EXPECT_STREQ(pattern.to_string().c_str(), "\\s+");
}

TEST_F(RegexPatternTest, MatchWord) {
    using namespace booleval::utils;

    RegexPattern pattern;
    pattern.match_word("and");

    EXPECT_STREQ(pattern.to_string().c_str(), "and");
}

TEST_F(RegexPatternTest, BuildPattern) {
    using namespace booleval::utils;

    RegexPattern pattern;
    pattern.match_whitespaces()
           .logical_or()
           .match_word("and")
           .logical_or()
           .match_word("or");

    EXPECT_STREQ(pattern.to_string().c_str(), "\\s+|and|or");
}

TEST_F(RegexPatternTest, BuildPatternWithEscapeChars) {
    using namespace booleval::utils;

    RegexPattern pattern;
    pattern.match_whitespaces()
           .logical_or()
           .match_word("&&")
           .logical_or()
           .match_word("||");

    EXPECT_STREQ(pattern.to_string().c_str(), "\\s+|\\&\\&|\\|\\|");
}