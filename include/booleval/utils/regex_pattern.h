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

#ifndef BOOLEVAL_REGEX_PATTERN_H
#define BOOLEVAL_REGEX_PATTERN_H

#include <sstream>

namespace booleval {

namespace utils {

class RegexPattern {
public:
    RegexPattern() noexcept;
    RegexPattern(RegexPattern&& other) = default;
    RegexPattern(RegexPattern const& other) = default;

    RegexPattern& operator=(RegexPattern&& other) = default;
    RegexPattern& operator=(RegexPattern const& other) = default;

    ~RegexPattern() = default;

    void match_whitespaces() noexcept;
    std::string to_string() const noexcept;

    friend RegexPattern& operator<<(RegexPattern& pattern, std::string const& value);

private:
    void escape() noexcept;

private:
    std::ostringstream output_;
    bool is_first_;
};

} // utils

} // booleval

#endif // BOOLEVAL_REGEX_PATTERN_H