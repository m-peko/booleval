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

#include <array>
#include <limits>
#include <string>
#include <vector>
#include <numeric>
#include <sstream>
#include <charconv>
#include <optional>
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
constexpr std::enable_if_t<std::is_enum_v<Enum>, Enum>
operator|(Enum lhs, Enum rhs) {
    return static_cast<Enum>(
        static_cast<std::underlying_type_t<Enum>>(lhs) |
        static_cast<std::underlying_type_t<Enum>>(rhs)
    );
}

template <typename Enum>
constexpr std::enable_if_t<std::is_enum_v<Enum>, Enum>
operator&(Enum lhs, Enum rhs) {
    return static_cast<Enum>(
        static_cast<std::underlying_type_t<Enum>>(lhs) &
        static_cast<std::underlying_type_t<Enum>>(rhs)
    );
}

template <typename Enum>
constexpr std::enable_if_t<std::is_enum_v<Enum>, Enum>
operator^(Enum lhs, Enum rhs) {
    return static_cast<Enum>(
        static_cast<std::underlying_type_t<Enum>>(lhs) ^
        static_cast<std::underlying_type_t<Enum>>(rhs)
    );
}

template <typename Enum>
constexpr std::enable_if_t<std::is_enum_v<Enum>, Enum>
operator~(Enum rhs) {
    return static_cast<Enum>(
        ~static_cast<std::underlying_type_t<Enum>>(rhs)
    );
}

template <typename Enum>
constexpr std::enable_if_t<std::is_enum_v<Enum>, Enum>&
operator|=(Enum& lhs, Enum rhs) {
    return static_cast<Enum>(
        static_cast<std::underlying_type_t<Enum>>(lhs) |
        static_cast<std::underlying_type_t<Enum>>(rhs)
    );
}

template <typename Enum>
constexpr std::enable_if_t<std::is_enum_v<Enum>, Enum>&
operator&=(Enum& lhs, Enum rhs) {
    return static_cast<Enum>(
        static_cast<std::underlying_type_t<Enum>>(lhs) &
        static_cast<std::underlying_type_t<Enum>>(rhs)
    );
}

template <typename Enum>
constexpr std::enable_if_t<std::is_enum_v<Enum>, Enum>&
operator^=(Enum& lhs, Enum rhs) {
    return static_cast<Enum>(
        static_cast<std::underlying_type_t<Enum>>(lhs) ^
        static_cast<std::underlying_type_t<Enum>>(rhs)
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
          typename = std::enable_if_t<std::is_enum_v<Enum>>>
constexpr bool is_set(Enum lhs, Enum rhs) {
    return (lhs & rhs) == lhs ||
           (lhs & rhs) == rhs;
}

/**
 * Removes whitespace characters from the left side of the string view.
 *
 * @param strv String view to remove whitespace characters from
 * @param c    Character to trim from the left side of the string view
 */
void ltrim(std::string_view& strv, char const c = ' ');

/**
 * Removes whitespace characters from the right side of the string view.
 *
 * @param strv String view to remove whitespace characters from
 * @param c    Character to trim from the right side of the string view
 */
void rtrim(std::string_view& strv, char const c = ' ');

/**
 * Removes whitespace characters from the both sides of the string view.
 *
 * @param strv String view to remove whitespace characters from
 * @param c    Character to trim from the both sides of the string view
 */
void trim(std::string_view& strv, char const c = ' ');

/**
 * Removes whitespace characters from the left side of the string view.
 *
 * @param strv String view to remove whitespace characters from
 * @param c    Character to trim from the left side of the string view
 *
 * @return Modified string view
 */
std::string_view ltrim_copy(std::string_view strv, char const c = ' ');

/**
 * Removes whitespace characters from the right side of the string view.
 *
 * @param strv String view to remove whitespace characters from
 * @param c    Character to trim from the right side of the string view
 *
 * @return Modified string view
 */
std::string_view rtrim_copy(std::string_view strv, char const c = ' ');

/**
 * Removes whitespace characters from the both sides of the string view.
 *
 * @param strv String view to remove whitespace characters from
 * @param c    Character to trim from the both sides of the string view
 *
 * @return Modified string view
 */
std::string_view trim_copy(std::string_view strv, char const c = ' ');

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

/**
 * Joins the elements from specified range into a string.
 *
 * @param first     Start of the range
 * @param last      End of the range
 * @param separator Separator between the elements in the produced string
 *
 * @return String computed by joining the elements from range
 */
template <typename InputIt>
std::string join(InputIt const& first, InputIt const& last, std::string const& separator = "") {
    if (first == last) {
        return {};
    }

    std::string result{ *first };
    return std::accumulate(
        std::next(first), last, result,
        [&separator](auto result, auto const value) {
            return result + separator + value;
        }
    );
}

/**
 * Converts from string view to integral value.
 * If value cannot be parsed, std::nullopt is returned.
 *
 * @param strv String view to convert to integral value
 *
 * @return Optional value
 */
template <typename T,
          typename std::enable_if_t<std::is_integral_v<T>>* = nullptr>
std::optional<T> from_chars(std::string_view strv) {
    T value{};

    auto const result = std::from_chars(
        strv.data(),
        strv.data() + strv.size(),
        value
    );

    if (std::errc() == result.ec) {
        return value;
    }

    return std::nullopt;
}

/**
 * Converts from string view to floating point value.
 * If value cannot be parsed, std::nullopt is returned.
 *
 * @param strv String view to convert to floating point value
 *
 * @return Optional value
 */
template <typename T,
          typename std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
std::optional<T> from_chars(std::string_view strv) {
    T value{};

    std::stringstream ss;
    ss << strv;
    ss >> value;

    if (ss.fail()) {
        return std::nullopt;
    }

    return value;
}

/**
 * Converts from integral value to string.
 *
 * @param value Integral value to convert to string
 *
 * @return String
 */
template <typename T,
          typename std::enable_if_t<std::is_integral_v<T>>* = nullptr>
std::string to_chars(T const value) {
    constexpr std::size_t buffer_size = std::numeric_limits<T>::digits10 + 2;  // +1 for minus, +1 for digits10
    std::array<char, buffer_size> buffer;

    auto const result = std::to_chars(
        buffer.data(),
        buffer.data() + buffer.size(),
        value
    );

    if (std::errc() == result.ec) {
        return std::string(buffer.data(), result.ptr - buffer.data());
    }

    return {};
}

/**
 * Converts from floating point value to string.
 *
 * @param value Floating point value to convert to string
 *
 * @return String
 */
template <typename T,
          typename std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
std::string to_chars(T const value) {
    auto str = std::to_string(value);
    std::string_view strv{ str.c_str(), str.size() };
    rtrim(strv, '0');
    str = strv;
    return str;
}

} // utils

} // booleval

#endif // BOOLEVAL_STRING_UTILS_H