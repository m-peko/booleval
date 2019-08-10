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

#ifndef BOOLEVAL_STRING_UTILS_H
#define BOOLEVAL_STRING_UTILS_H

#include <string>
#include <vector>

namespace booleval {

namespace utils {

/**
 * Removes whitespace characters from the left side of the string.
 *
 * @param str String to remove whitespace characters from
 */
void ltrim(std::string& str);

/**
 * Removes whitespace characters from the right side of the string.
 *
 * @param str String to remove whitespace characters from
 */
void rtrim(std::string& str);

/**
 * Removes whitespace characters from the both sides of the string.
 *
 * @param str String to remove whitespace characters from
 */
void trim(std::string& str);

/**
 * Removes whitespace characters from the left side of the string.
 *
 * @param str String to remove whitespace characters from
 *
 * @return Copy of the modified string
 */
std::string ltrim_copy(std::string str);

/**
 * Removes whitespace characters from the right side of the string.
 *
 * @param str String to remove whitespace characters from
 *
 * @return Copy of the modified string
 */
std::string rtrim_copy(std::string str);

/**
 * Removes whitespace characters from the both sides of the string.
 *
 * @param str String to remove whitespace characters from
 *
 * @return Copy of the modified string
 */
std::string trim_copy(std::string str);

/**
 * Checks whether string is empty or contains only whitespace characters.
 *
 * @param str String to check
 *
 * @return True if string is empty, otherwise false
 */
bool is_empty(std::string const& str);

/**
 * Splits string by regular expression.
 *
 * @param str     String to split
 * @param pattern Delimiting regular expression pattern
 *
 * @return Vector of strings computed by splitting the given string
 */
std::vector<std::string> split(std::string const& str, std::string const& pattern);

} // utils

} // booleval

#endif // BOOLEVAL_STRING_UTILS_H