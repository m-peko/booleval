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

#include <booleval/utils/regex_pattern.h>

namespace booleval {

namespace utils {

RegexPattern::RegexPattern() noexcept
    : is_first_(true)
{}

void RegexPattern::match_whitespaces() noexcept {
    if (is_first_) {
        is_first_ = false;
    } else {
        output_ << '|';
    }

    output_ << "\\s+";
}

std::string RegexPattern::to_string() const noexcept {
    return output_.str();
}

void RegexPattern::escape() noexcept {
    output_ << "\\";
}

RegexPattern& operator<<(RegexPattern& pattern, std::string const& value) {
    if (pattern.is_first_) {
        pattern.is_first_ = false;
    } else {
        pattern.output_ << '|';
    }

    for (auto const character : value) {
        if (!isalnum(character)) {
            pattern.escape();
        }

        pattern.output_ << character;
    }

    return pattern;
}

} // utils

} // booleval