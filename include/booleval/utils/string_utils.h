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

#include <vector>
#include <string_view>

namespace booleval {

namespace utils {

/**
 * enum class split_options
 *
 * Represents an option used when splitting a string.
 */
enum class split_options : uint8_t {
    off                = 0x00,
    include_whitespace = 0x01,
    include_delimiters = 0x02,
    exclude_delimiters = 0x04
};

template <typename Enum>
constexpr std::enable_if_t<std::is_enum<Enum>::value, Enum>
operator|(Enum lhs, Enum rhs) {
    using underlying_type = typename std::underlying_type<Enum>::type;
    return static_cast<Enum>(
        static_cast<underlying_type>(lhs) |
        static_cast<underlying_type>(rhs)
    );
}

template <typename Enum>
constexpr std::enable_if_t<std::is_enum<Enum>::value, Enum>
operator&(Enum lhs, Enum rhs) {
    using underlying_type = typename std::underlying_type<Enum>::type;
    return static_cast<Enum>(
        static_cast<underlying_type>(lhs) &
        static_cast<underlying_type>(rhs)
    );
}

template <typename Enum>
constexpr std::enable_if_t<std::is_enum<Enum>::value, Enum>
operator^(Enum lhs, Enum rhs) {
    using underlying_type = typename std::underlying_type<Enum>::type;
    return static_cast<Enum>(
        static_cast<underlying_type>(lhs) ^
        static_cast<underlying_type>(rhs)
    );
}

template <typename Enum>
constexpr std::enable_if_t<std::is_enum<Enum>::value, Enum>
operator~(Enum rhs) {
    using underlying_type = typename std::underlying_type<Enum>::type;
    return static_cast<Enum>(
        ~static_cast<underlying_type>(rhs)
    );
}

template <typename Enum>
constexpr std::enable_if_t<std::is_enum<Enum>::value, Enum>&
operator|=(Enum& lhs, Enum rhs) {
    using underlying_type = typename std::underlying_type<Enum>::type;
    return static_cast<Enum>(
        static_cast<underlying_type>(lhs) |
        static_cast<underlying_type>(rhs)
    );
}

template <typename Enum>
constexpr std::enable_if_t<std::is_enum<Enum>::value, Enum>&
operator&=(Enum& lhs, Enum rhs) {
    using underlying_type = typename std::underlying_type<Enum>::type;
    return static_cast<Enum>(
        static_cast<underlying_type>(lhs) &
        static_cast<underlying_type>(rhs)
    );
}

template <typename Enum>
constexpr std::enable_if_t<std::is_enum<Enum>::value, Enum>&
operator^=(Enum& lhs, Enum rhs) {
    using underlying_type = typename std::underlying_type<Enum>::type;
    return static_cast<Enum>(
        static_cast<underlying_type>(lhs) ^
        static_cast<underlying_type>(rhs)
    );
}

/**
 * Checks whether bits of the first set are present in
 * another the second set and vice versa.
 *
 * @param lhs The first set of bits to check
 * @param rhs The second set of bits to check
 *
 * @return True if the bits are present, false otherwise
 */
template <typename Enum,
          typename = std::enable_if_t<std::is_enum<Enum>::value>>
constexpr bool is_set(Enum lhs, Enum rhs) {
    return (lhs & rhs) == lhs ||
           (lhs & rhs) == rhs;
}

/**
 * Removes whitespace characters from the left side of the string view.
 *
 * @param strv String view to remove whitespace characters from
 */
void ltrim(std::string_view& strv);

/**
 * Removes whitespace characters from the right side of the string view.
 *
 * @param strv String view to remove whitespace characters from
 */
void rtrim(std::string_view& strv);

/**
 * Removes whitespace characters from the both sides of the string view.
 *
 * @param strv String view to remove whitespace characters from
 */
void trim(std::string_view& strv);

/**
 * Removes whitespace characters from the left side of the string view.
 *
 * @param strv String view to remove whitespace characters from
 *
 * @return Modified string view
 */
std::string_view ltrim_copy(std::string_view strv);

/**
 * Removes whitespace characters from the right side of the string view.
 *
 * @param strv String view to remove whitespace characters from
 *
 * @return Modified string view
 */
std::string_view rtrim_copy(std::string_view strv);

/**
 * Removes whitespace characters from the both sides of the string view.
 *
 * @param strv String view to remove whitespace characters from
 *
 * @return Modified string view
 */
std::string_view trim_copy(std::string_view strv);

/**
 * Checks whether string view is empty or contains only whitespace characters.
 *
 * @param strv String view to check
 *
 * @return True if string view is empty, otherwise false
 */
bool is_empty(std::string_view strv);

/**
 * Splits string view by delimiters.
 *
 * @param strv    String view to split
 * @param delims  Delimiters to split the string view by
 * @param options Options used while splitting the string view
 *                (by default, whitespace delimiter is included and
 *                delimiters are excluded from the result)
 *
 * @return Tokens computed by splitting the given string view
 */
std::vector<std::string_view> split(std::string_view strv,
                                    std::string_view delims = " ",
                                    split_options const options = split_options::exclude_delimiters);

} // utils

} // booleval

#endif // BOOLEVAL_STRING_UTILS_H