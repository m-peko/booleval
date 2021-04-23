/*
 * Copyright (c) 2021, Marin Peko
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

#ifndef BOOLEVAL_SPLIT_OPTIONS_HPP
#define BOOLEVAL_SPLIT_OPTIONS_HPP

#include <cstdint>

namespace booleval::utils
{

/**
 * enum class split_options
 *
 * Represents an options used when splitting a string.
 */
enum class [[ nodiscard ]] split_options : std::uint8_t
{
    off                  = 0x00,
    include_delimiters   = 0x01,
    exclude_delimiters   = 0x02,
    split_by_whitespace  = 0x04,
    allow_quoted_strings = 0x08
};

template< typename EnumT >
constexpr std::enable_if_t< std::is_enum_v< EnumT >, EnumT >
operator|( EnumT lhs, EnumT rhs )
{
    return static_cast< EnumT >
    (
        static_cast< std::underlying_type_t< EnumT > >( lhs) |
        static_cast< std::underlying_type_t< EnumT > >( rhs)
    );
}

template< typename EnumT >
constexpr std::enable_if_t< std::is_enum_v< EnumT >, EnumT >
operator&( EnumT lhs, EnumT rhs )
{
    return static_cast< EnumT >
    (
        static_cast< std::underlying_type_t< EnumT > >( lhs) &
        static_cast< std::underlying_type_t< EnumT > >( rhs)
    );
}

template< typename EnumT >
constexpr std::enable_if_t< std::is_enum_v< EnumT >, EnumT >
operator^( EnumT lhs, EnumT rhs )
{
    return static_cast< EnumT >
    (
        static_cast< std::underlying_type_t< EnumT > >( lhs) ^
        static_cast< std::underlying_type_t< EnumT > >( rhs)
    );
}

template< typename EnumT >
constexpr std::enable_if_t< std::is_enum_v< EnumT >, EnumT >
operator~( EnumT rhs )
{
    return static_cast< EnumT >
    (
        ~static_cast< std::underlying_type_t< EnumT > >( rhs)
    );
}

template< typename EnumT >
[[ nodiscard ]] constexpr std::enable_if_t< std::is_enum_v< EnumT >, EnumT > &
operator|=( EnumT & lhs, EnumT rhs )
{
    return static_cast< EnumT >
    (
        static_cast< std::underlying_type_t< EnumT > >( lhs ) |
        static_cast< std::underlying_type_t< EnumT > >( rhs )
    );
}

template< typename EnumT >
[[ nodiscard ]] constexpr std::enable_if_t< std::is_enum_v< EnumT >, EnumT > &
operator&=( EnumT & lhs, EnumT rhs )
{
    return static_cast< EnumT >
    (
        static_cast< std::underlying_type_t< EnumT > >( lhs ) &
        static_cast< std::underlying_type_t< EnumT > >( rhs )
    );
}

template< typename EnumT >
[[ nodiscard ]] constexpr std::enable_if_t< std::is_enum_v< EnumT >, EnumT > &
operator^=( EnumT & lhs, EnumT rhs )
{
    return static_cast< EnumT >
    (
        static_cast< std::underlying_type_t< EnumT > >( lhs ) ^
        static_cast< std::underlying_type_t< EnumT > >( rhs )
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
template
<
    typename EnumT,
    typename = std::enable_if_t< std::is_enum_v< EnumT > >
>
[[ nodiscard ]] constexpr bool is_set( EnumT const lhs, EnumT const rhs )
{
    return (lhs & rhs) == lhs ||
           (lhs & rhs) == rhs;
}

} // namespace booleval::utils

#endif // BOOLEVAL_SPLIT_OPTIONS_HPP
