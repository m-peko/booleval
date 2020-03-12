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
#include <algorithm>
#include <booleval/utils/string_utils.h>

namespace booleval {

namespace utils {

void ltrim(std::string_view& strv) {
    strv.remove_prefix(
        std::min(
            strv.find_first_not_of(' '),
            strv.size()
        )
    );
}

void rtrim(std::string_view& strv) {
    strv.remove_suffix(
        std::min(
            strv.size() - strv.find_last_not_of(' ') - 1,
            strv.size()
        )
    );
}

void trim(std::string_view& strv) {
    ltrim(strv);
    rtrim(strv);
}

std::string_view ltrim_copy(std::string_view strv) {
    ltrim(strv);
    return strv;
}

std::string_view rtrim_copy(std::string_view strv) {
    rtrim(strv);
    return strv;
}

std::string_view trim_copy(std::string_view strv) {
    trim(strv);
    return strv;
}

bool is_empty(std::string_view strv) {
    return strv.empty() || std::string_view::npos == strv.find_first_not_of(' ');
}

std::vector<std::string_view> split(std::string_view strv, std::string_view delims, split_options const options) {
    std::string delims_impl{ delims };
    if (is_set(options, split_options::include_whitespace)) {
        delims_impl.append(1, ' ');
    }

    std::vector<std::string_view> tokens;

    auto first = std::begin(strv);
    while (first != std::end(strv)) {
        auto const second = std::find_first_of(
            first, std::cend(strv),
			std::cbegin(delims_impl), std::cend(delims_impl)
        );

		if (first != second) {
			tokens.emplace_back(
                strv.substr(
                    std::distance(std::begin(strv), first),
                    std::distance(first, second)
                )
            );
		}

		if (std::end(strv) == second) {
			break;
        }

        if (is_set(options, split_options::include_delimiters)) {
            std::string_view delim{ second, 1 };
            if (!is_empty(delim)) {
                tokens.emplace_back(delim);
            }
        }

		first = std::next(second);
	}

	return tokens;
}

} // utils

} // booleval