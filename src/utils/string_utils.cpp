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

#include <regex>
#include <algorithm>
#include <booleval/utils/string_utils.h>

namespace booleval {

namespace utils {

void ltrim(std::string& str) {
    str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](int character) {
        return !std::isspace(character);
    }));
}

void rtrim(std::string& str) {
    str.erase(std::find_if(str.rbegin(), str.rend(), [](int character) {
        return !std::isspace(character);
    }).base(), str.end());
}

void trim(std::string& str) {
    ltrim(str);
    rtrim(str);
}

std::string ltrim_copy(std::string str) {
    ltrim(str);
    return str;
}

std::string rtrim_copy(std::string str) {
    rtrim(str);
    return str;
}

std::string trim_copy(std::string str) {
    trim(str);
    return str;
}

bool is_empty(std::string const& str) {
    return str.empty() || str.find_first_not_of(' ') == std::string::npos;
}

std::vector<std::string> split(std::string const& str, std::string const& pattern) {
    std::regex regex(pattern);

    std::vector<std::string> result(
        std::sregex_token_iterator(str.begin(), str.end(), regex, {-1, 0}),
        std::sregex_token_iterator()
    );

    for (auto& token : result) {
        trim(token);
    }

    result.erase(
        std::remove_if(
            result.begin(),
            result.end(),
            [](std::string const& str) { return is_empty(str); }
        ),
        result.end()
    );

    return result;
}

} // utils

} // booleval