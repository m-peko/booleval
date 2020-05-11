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
#include <algorithm>
#include <string_view>

namespace booleval {

namespace utils {

/**
 * enum class split_options
 *
 * Represents an option used when splitting a string.
 */
enum class [[nodiscard]] split_options : uint8_t {
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
[[nodiscard]] constexpr std::enable_if_t<std::is_enum_v<Enum>, Enum>&
operator|=(Enum& lhs, Enum rhs) {
    return static_cast<Enum>(
        static_cast<std::underlying_type_t<Enum>>(lhs) |
        static_cast<std::underlying_type_t<Enum>>(rhs)
    );
}

template <typename Enum>
[[nodiscard]] constexpr std::enable_if_t<std::is_enum_v<Enum>, Enum>&
operator&=(Enum& lhs, Enum rhs) {
    return static_cast<Enum>(
        static_cast<std::underlying_type_t<Enum>>(lhs) &
        static_cast<std::underlying_type_t<Enum>>(rhs)
    );
}

template <typename Enum>
[[nodiscard]] constexpr std::enable_if_t<std::is_enum_v<Enum>, Enum>&
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
[[nodiscard]] constexpr bool is_set(Enum lhs, Enum rhs) {
    return (lhs & rhs) == lhs ||
           (lhs & rhs) == rhs;
}

/**
 * Removes whitespace characters from the left side of the string view.
 *
 * @param strv String view to remove whitespace characters from
 * @param c    Character to trim from the left side of the string view
 *
 * @return Modified string view
 */
[[nodiscard]] constexpr std::string_view ltrim(std::string_view strv, char const c = ' ') {
    strv.remove_prefix(
        std::min(
            strv.find_first_not_of(c),
            strv.size()
        )
    );
    return strv;
}

/**
 * Removes whitespace characters from the right side of the string view.
 *
 * @param strv String view to remove whitespace characters from
 * @param c    Character to trim from the right side of the string view
 *
 * @return Modified string view
 */
[[nodiscard]] constexpr std::string_view rtrim(std::string_view strv, char const c = ' ') {
    strv.remove_suffix(
        std::min(
            strv.size() - strv.find_last_not_of(c) - 1,
            strv.size()
        )
    );
    return strv;
}

/**
 * Removes whitespace characters from the both sides of the string view.
 *
 * @param strv String view to remove whitespace characters from
 * @param c    Character to trim from the both sides of the string view
 *
 * @return Modified string view
 */
[[nodiscard]] constexpr std::string_view trim(std::string_view strv, char const c = ' ') {
    strv = ltrim(strv, c);
    return rtrim(strv, c);
}

/**
 * Checks whether string view is empty or contains only whitespace characters.
 *
 * @param strv String view to check
 *
 * @return True if string view is empty, otherwise false
 */
[[nodiscard]] constexpr bool is_empty(std::string_view strv) {
    return strv.empty() || std::string_view::npos == strv.find_first_not_of(' ');
}

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
template <split_options options = split_options::exclude_delimiters>
[[nodiscard]] std::vector<std::string_view> split(std::string_view strv,
                                                  std::string_view delims = " ") {
    std::string delims_impl{ delims };
    if constexpr (is_set(options, split_options::include_whitespace)) {
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

        if constexpr (is_set(options, split_options::include_delimiters)) {
            std::string_view delim{ &*second, 1 };
            if (!is_empty(delim)) {
                tokens.emplace_back(delim);
            }
        }

        first = std::next(second);
    }

    return tokens;
}

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
[[nodiscard]] std::string join(InputIt const& first, InputIt const& last, std::string const& separator = "") {
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
 * Converts from string view to arithmetic value.
 * If value cannot be parsed, std::nullopt is returned.
 *
 * NOTE: Floating point version of std::from_chars is
 * implemented only on MSVC and not on GCC/CLANG.
 *
 * @param strv String view to convert to arithmetic value
 *
 * @return Optional value
 */
#if defined(__GNUC__) || defined(__clang__)
template <typename T,
          typename std::enable_if_t<std::is_integral_v<T>>* = nullptr>
#elif defined(_MSC_VER)
template <typename T,
          typename std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
#endif
[[nodiscard]] std::optional<T> from_chars(std::string_view strv) {
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
 * Converts from string view to arithmetic value.
 * If value cannot be parsed, std::nullopt is returned.
 *
 * NOTE: Floating point version of std::from_chars is
 * implemented only on MSVC and not on GCC/CLANG.
 *
 * @param strv String view to convert to arithmetic value
 *
 * @return Optional value
 */
#if defined(__GNUC__) || defined(__clang__)
template <typename T,
          typename std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
[[nodiscard]] std::optional<T> from_chars(std::string_view strv) {
    T value{};

    std::stringstream ss;
    ss << strv;
    ss >> value;

    if (ss.fail()) {
        return std::nullopt;
    }

    return value;
}
#endif

/**
 * Converts from arithmetic value to string.
 *
 * NOTE: Floating point version of std::to_chars is
 * implemented only on MSVC and not on GCC/CLANG.
 *
 * @param value Arithmetic value to convert to string
 *
 * @return String representation of arithmetic value
 */
#if defined(__GNUC__) || defined(__clang__)
template <typename T,
          typename std::enable_if_t<std::is_integral_v<T>>* = nullptr>
#elif defined(_MSC_VER)
template <typename T,
          typename std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
#endif
[[nodiscard]] std::string to_chars(T const value) {
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
 * Converts from arithmetic value to string.
 *
 * NOTE: Floating point version of std::to_chars is
 * implemented only on MSVC and not on GCC/CLANG.
 *
 * @param value Arithmetic value to convert to string
 *
 * @return String representation of arithmetic value
 */
#if defined(__GNUC__) || defined(__clang__)
template <typename T,
          typename std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
[[nodiscard]] std::string to_chars(T const value) {
    auto str = std::to_string(value);
    std::string_view strv{ str.c_str(), str.size() };
    str = rtrim(strv, '0');
    return str;
}
#endif

} // utils

} // booleval

#endif // BOOLEVAL_STRING_UTILS_H