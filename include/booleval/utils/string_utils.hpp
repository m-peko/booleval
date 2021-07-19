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

#ifndef BOOLEVAL_STRING_UTILS_HPP
#define BOOLEVAL_STRING_UTILS_HPP

#include <array>
#include <limits>
#include <string>
#include <numeric>
#include <sstream>
#include <charconv>
#include <optional>
#include <string_view>

namespace booleval::utils
{

/**
 * Checks if string view contains only whitespaces.
 *
 * @param strv String view to check
 *
 * @return True if string view contains only whitespaces, false otherwise
 */
[[ nodiscard ]] constexpr bool is_whitespace( std::string_view const strv ) noexcept
{
    return strv.find_first_not_of( ' ' ) == std::string_view::npos;
}

/**
 * Removes specified characters from the left side of the string view.
 *
 * @param strv String view to remove specified characters from
 * @param c    Character to trim from the left side of the string view
 *
 * @return Modified string view
 */
[[ nodiscard ]] constexpr std::string_view ltrim(std::string_view strv, char const c = ' ') noexcept
{
    strv.remove_prefix
    (
        std::min
        (
            strv.find_first_not_of( c ),
            std::size( strv )
        )
    );
    return strv;
}

/**
 * Removes specified characters from the right side of the string view.
 *
 * @param strv String view to remove specified characters from
 * @param c    Character to trim from the right side of the string view
 *
 * @return Modified string view
 */
[[ nodiscard ]] constexpr std::string_view rtrim( std::string_view strv, char const c = ' ') noexcept
{
    strv.remove_suffix
    (
        std::min
        (
            std::size( strv ) - strv.find_last_not_of( c ) - 1,
            std::size( strv )
        )
    );
    return strv;
}

/**
 * Removes specified characters from the both sides of the string view.
 *
 * @param strv String view to remove whitespace characters from
 * @param c    Character to trim from the both sides of the string view
 *
 * @return Modified string view
 */
[[ nodiscard ]] constexpr std::string_view trim( std::string_view strv, char const c = ' ' ) noexcept
{
    strv = ltrim( strv, c );
    return rtrim( strv, c );
}

/**
 * Checks whether string view is empty or contains only whitespace characters.
 *
 * @param strv String view to check
 *
 * @return True if string view is empty, otherwise false
 */
[[ nodiscard ]] constexpr bool is_empty( std::string_view const strv ) noexcept
{
    return strv.empty() || strv.find_first_not_of( ' ' ) == std::string_view::npos;
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
template< typename InputIt >
[[ nodiscard ]] std::string join( InputIt first, InputIt last, std::string_view const separator = "" ) noexcept
{
    std::string result;

    bool is_first{ true };

    for ( ; first != last; ++first )
    {
        if ( is_first ) { is_first = false; }
        else
        {
            result += separator;
        }

        result += *first;
    }

    return result;
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
#if defined( __GNUC__ ) || defined( __clang__ )
template
<
    typename T,
    typename std::enable_if_t< std::is_integral_v< T > >* = nullptr
>
#elif defined( _MSC_VER )
template
<
    typename T,
    typename std::enable_if_t< std::is_arithmetic_v< T > >* = nullptr
>
#endif
[[ nodiscard ]] std::optional< T > from_chars( std::string_view const strv ) noexcept
{
    T value{};

    auto const result
    {
        std::from_chars
        (
            std::data( strv ),
            std::data( strv ) + std::size( strv ),
            value
        )
    };

    if ( result.ec == std::errc() ) { return value; }

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
#if defined( __GNUC__ ) || defined( __clang__ )
template
<
    typename T,
    typename std::enable_if_t< std::is_floating_point_v< T > >* = nullptr
>
[[ nodiscard ]] std::optional< T > from_chars( std::string_view const strv ) noexcept
{
    T value{};

    std::stringstream ss;
    ss << strv;
    ss >> value;

    if ( ss.fail() ) { return std::nullopt; }

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
#if defined( __GNUC__ ) || defined( __clang__ )
template
<
    typename T,
    typename std::enable_if_t< std::is_integral_v< T > >* = nullptr
>
#elif defined( _MSC_VER )
template
<
    typename T,
    typename std::enable_if_t< std::is_arithmetic_v< T > >* = nullptr
>
#endif
[[ nodiscard ]] std::string to_chars( T const value ) noexcept
{
    constexpr std::size_t buffer_size{ std::numeric_limits< T >::digits10 + 2 };  // +1 for minus, +1 for digits10
    std::array< char, buffer_size > buffer;

    auto const result
    {
        std::to_chars
        (
            std::data( buffer ),
            std::data( buffer ) + std::size( buffer ),
            value
        )
    };

    if ( result.ec == std::errc() )
    {
        return std::string( buffer.data(), result.ptr - buffer.data() );
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
#if defined( __GNUC__ ) || defined( __clang__ )
template
<
    typename T,
    typename std::enable_if_t< std::is_floating_point_v< T > >* = nullptr
>
[[ nodiscard ]] std::string to_chars( T const value ) noexcept
{
    auto result{ std::to_string( value ) };
    result = rtrim( result, '0' );
    return result;
}
#endif

} // namespace booleval::utils

#endif // BOOLEVAL_STRING_UTILS_HPP