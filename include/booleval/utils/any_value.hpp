/*
 * Copyright (c) 2020, Marin Peko
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

#ifndef BOOLEVAL_ANY_VALUE_HPP
#define BOOLEVAL_ANY_VALUE_HPP

#include <string>
#include <type_traits>
#include <booleval/utils/string_utils.hpp>

namespace booleval::utils
{

/**
 * @class any_value
 *
 * Represents the class that accepts any type of value through its constructor
 * or assignment operator and internally stores its string version.
 */
class any_value
{
public:
    any_value() noexcept = default;

    any_value( any_value       && rhs ) noexcept = default;
    any_value( any_value const  & rhs ) noexcept = default;

    template< typename T >
    any_value( T && rhs ) noexcept
    {
        if constexpr ( std::is_arithmetic_v< std::remove_reference_t< T > > )
        {
            value_ = utils::to_chars< std::remove_reference_t< T > >( std::forward< T >( rhs ) );
            use_string_comparison_ = false;
        }
        else if constexpr ( std::is_constructible_v< std::string, std::remove_reference_t< T > > )
        {
            value_ = std::forward< T >( rhs );
            use_string_comparison_ = true;
        }
    }

    any_value& operator=( any_value       && rhs ) noexcept = default;
    any_value& operator=( any_value const  & rhs ) noexcept = default;

    template< typename T >
    any_value& operator=( T && rhs ) noexcept
    {
        if constexpr ( std::is_arithmetic_v< std::remove_reference_t< T > > )
        {
            value_ = utils::to_chars< std::remove_reference_t< T > >( std::forward< T >( rhs ) );
            use_string_comparison_ = false;
        }
        else if constexpr ( std::is_constructible_v< std::string, std::remove_reference_t< T > > )
        {
            value_ = std::forward< T >( rhs );
            use_string_comparison_ = true;
        }
        return *this;
    }

    template< typename T >
    [[ nodiscard ]] bool operator==( T && rhs ) noexcept
    {
        if constexpr ( std::is_arithmetic_v< std::remove_reference_t< T > > )
        {
            return value_ == utils::to_chars< std::remove_reference_t< T > >( std::forward< T >( rhs ) );
        }
        else if constexpr ( std::is_constructible_v< std::string, std::remove_reference_t< T > > )
        {
            return value_ == rhs;
        }
    }

    template< typename T >
    [[ nodiscard ]] bool operator!=( T && rhs ) noexcept
    {
        if constexpr ( std::is_arithmetic_v< std::remove_reference_t< T > > )
        {
            return value_ != utils::to_chars< std::remove_reference_t< T > >( std::forward< T >( rhs ) );
        }
        else if constexpr ( std::is_constructible_v< std::string, std::remove_reference_t< T > > )
        {
            return value_ != rhs;
        }
    }

    template <typename T,
              typename std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
    [[ nodiscard ]] bool operator>(T const rhs);

    template <typename T,
              typename std::enable_if_t<std::is_constructible_v<std::string, T>>* = nullptr>
    [[ nodiscard ]] bool operator>(T const rhs);

    template <typename T,
              typename std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
    [[ nodiscard ]] bool operator<(T const rhs);

    template <typename T,
              typename std::enable_if_t<std::is_constructible_v<std::string, T>>* = nullptr>
    [[ nodiscard ]] bool operator<(T const rhs);

    template <typename T,
              typename std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
    [[ nodiscard ]] bool operator>=(T const rhs);

    template <typename T,
              typename std::enable_if_t<std::is_constructible_v<std::string, T>>* = nullptr>
    [[ nodiscard ]] bool operator>=(T const rhs);

    template <typename T,
              typename std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
    [[ nodiscard ]] bool operator<=(T const rhs);

    template <typename T,
              typename std::enable_if_t<std::is_constructible_v<std::string, T>>* = nullptr>
    [[ nodiscard ]] bool operator<=(T const rhs);

    ~any_value() noexcept = default;

    friend bool operator==( any_value const & lhs, any_value const & rhs ) noexcept;
    friend bool operator!=( any_value const & lhs, any_value const & rhs ) noexcept;

private:
    std::string value_;
    bool use_string_comparison_{ false };
};

template <typename T,
          typename std::enable_if_t<std::is_arithmetic_v<T>>*>
bool any_value::operator>(T const rhs) {
    auto arithmetic_lhs = utils::from_chars<T>(value_);
    if (arithmetic_lhs) {
        return arithmetic_lhs.value() > rhs;
    }

    return false;
}

template <typename T,
          typename std::enable_if_t<std::is_constructible_v<std::string, T>>*>
bool any_value::operator>(T const rhs) {
    if (use_string_comparison_) {
        return value_ > rhs;
    }

    auto arithmetic_lhs = utils::from_chars<double>(value_);
    auto arithmetic_rhs = utils::from_chars<double>(rhs);
    if (arithmetic_lhs && arithmetic_rhs) {
        return arithmetic_lhs.value() > arithmetic_rhs.value();
    }

    return false;
}

template <typename T,
          typename std::enable_if_t<std::is_arithmetic_v<T>>*>
bool any_value::operator<(T const rhs) {
    auto arithmetic_lhs = utils::from_chars<T>(value_);
    if (arithmetic_lhs) {
        return arithmetic_lhs.value() < rhs;
    }

    return false;
}

template <typename T,
          typename std::enable_if_t<std::is_constructible_v<std::string, T>>*>
bool any_value::operator<(T const rhs) {
    if (use_string_comparison_) {
        return value_ < rhs;
    }

    auto arithmetic_lhs = utils::from_chars<double>(value_);
    auto arithmetic_rhs = utils::from_chars<double>(rhs);
    if (arithmetic_lhs && arithmetic_rhs) {
        return arithmetic_lhs.value() < arithmetic_rhs.value();
    }

    return false;
}

template <typename T,
          typename std::enable_if_t<std::is_arithmetic_v<T>>*>
bool any_value::operator>=(T const rhs) {
    auto arithmetic_lhs = utils::from_chars<T>(value_);
    if (arithmetic_lhs) {
        return arithmetic_lhs.value() >= rhs;
    }

    return false;
}

template <typename T,
          typename std::enable_if_t<std::is_constructible_v<std::string, T>>*>
bool any_value::operator>=(T const rhs) {
    if (use_string_comparison_) {
        return value_ >= rhs;
    }

    auto arithmetic_lhs = utils::from_chars<double>(value_);
    auto arithmetic_rhs = utils::from_chars<double>(rhs);
    if (arithmetic_lhs && arithmetic_rhs) {
        return arithmetic_lhs.value() >= arithmetic_rhs.value();
    }

    return false;
}

template <typename T,
          typename std::enable_if_t<std::is_arithmetic_v<T>>*>
bool any_value::operator<=(T const rhs) {
    auto arithmetic_lhs = utils::from_chars<T>(value_);
    if (arithmetic_lhs) {
        return arithmetic_lhs.value() <= rhs;
    }

    return false;
}

template <typename T,
          typename std::enable_if_t<std::is_constructible_v<std::string, T>>*>
bool any_value::operator<=(T const rhs) {
    if (use_string_comparison_) {
        return value_ <= rhs;
    }

    auto arithmetic_lhs = utils::from_chars<double>(value_);
    auto arithmetic_rhs = utils::from_chars<double>(rhs);
    if (arithmetic_lhs && arithmetic_rhs) {
        return arithmetic_lhs.value() <= arithmetic_rhs.value();
    }

    return false;
}

[[ nodiscard ]] inline bool operator==( any_value const & lhs, any_value const & rhs ) noexcept
{
    return lhs.value_ == rhs.value_;
}

[[ nodiscard ]] inline bool operator!=( any_value const & lhs, any_value const & rhs ) noexcept
{
    return lhs.value_ != rhs.value_;
}

} // namespace booleval::utils

#endif // BOOLEVAL_ANY_VALUE_HPP
