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

#ifndef BOOLEVAL_SPLIT_RANGE_HPP
#define BOOLEVAL_SPLIT_RANGE_HPP

#include <iterator>
#include <algorithm>
#include <string_view>

#include <booleval/utils/split_options.hpp>
#include <booleval/utils/string_utils.hpp>

namespace booleval::utils
{

constexpr auto whitespace_char  { ' '  };
constexpr auto single_quote_char{ '\'' };
constexpr auto double_quote_char{ '\"' };

/**
 * @class split_range
 *
 * Represents the range of tokens computed by splitting the given
 * string view by the specified delimiters.
 */
template
<
    split_options options    = split_options::exclude_delimiters | split_options::allow_quoted_strings,
    char          quote_char = double_quote_char
>
class split_range
{
public:
    template
    <
        split_options iterator_options,
        char          iterator_quote_char
    >
    class iterator
    {
        friend split_range;

    public:
        struct element
        {
            bool             is_quoted{ false };
            std::string_view value    {};
        };

        using iterator_category = std::forward_iterator_tag;
        using value_type        = element;
        using difference_type   = std::ptrdiff_t;
        using pointer           = value_type*;
        using reference         = value_type&;

    public:
        explicit constexpr iterator( std::string_view const strv, std::string_view const delims ) noexcept
            : strv_  { strv                }
            , delims_{ delims              }
            , prev_  { std::begin( strv_ ) }
        {
            next();
        }

        constexpr iterator( iterator       && rhs ) noexcept = default;
        constexpr iterator( iterator const  & rhs ) noexcept = default;

        iterator& operator=( iterator       && rhs ) noexcept = default;
        iterator& operator=( iterator const  & rhs ) noexcept = default;

        [[ nodiscard ]] constexpr auto operator* () const noexcept { return  curr_value_; }
        [[ nodiscard ]] constexpr auto operator->() const noexcept { return &curr_value_; }

        constexpr iterator& operator++() noexcept
        {
            if ( curr_value_.is_quoted )
            {
                curr_ = skip_char( curr_, whitespace_char );
                if ( curr_ != std::end( strv_ ) && curr_ != std::prev( std::end( strv_ ) ) )
                {
                    curr_ = std::next( curr_ );
                }
                if ( curr_ != std::end( strv_ ) )
                {
                    prev_ = std::next( curr_ );
                    if constexpr ( is_set( iterator_options, split_options::include_delimiters ) )
                    {
                        prev_ = skip_char( curr_, whitespace_char );
                    }
                }
            }
            else
            {
                if constexpr ( is_set( iterator_options, split_options::include_delimiters ) )
                {
                    curr_ = skip_char( curr_, whitespace_char );
                    prev_ = curr_;
                }
                else
                {
                    if ( curr_ != std::end( strv_ ) ) { prev_ = std::next( curr_ ); }
                    else                              { prev_ = std::end ( strv_ ); }
                }
            }

            curr_value_.is_quoted = false;

            next();
            return *this;
        }

        constexpr iterator operator++( int ) noexcept
        {
            auto tmp{ *this };
            ++(*this);
            return tmp;
        }

        [[ nodiscard ]] constexpr bool operator!=( iterator const & rhs ) const noexcept { return prev_ != rhs.prev_; }
        [[ nodiscard ]] constexpr bool operator==( iterator const & rhs ) const noexcept { return prev_ == rhs.prev_; }

        ~iterator() noexcept = default;

    private:
        explicit constexpr iterator( std::string_view::const_iterator end ) noexcept : prev_{ end } {}

        /**
         * Finds the iterator pointing to the beginning of the next token in the specified range.
         *
         * @param first Iterator to the first element of the range to examine
         * @param last  Iterator to the last element of the range to examine
         *
         * @return Iterator to the beginning of the next token
         */
        [[ nodiscard ]] constexpr std::string_view::iterator find_next
        (
            std::string_view::iterator const first,
            std::string_view::iterator const last
        ) const noexcept
        {
            if constexpr ( is_set( iterator_options, split_options::allow_quoted_strings ) )
            {
                if ( curr_value_.is_quoted )
                {
                    return find_next_quote( first, last );
                }
                else
                {
                    return std::min
                    (
                        find_next_quote( first, last ),
                        find_next_delim( first, last )
                    );
                }
            }
            else
            {
                return find_next_delim( first, last );
            }
        }

        /**
         * Finds the iterator pointing to the first delimiter in the specified range.
         *
         * @param first Iterator to the first element of the range to examine
         * @param last  Iterator to the last element of the range to examine
         *
         * @return Iterator to the first delimiter
         */
        [[ nodiscard ]] constexpr std::string_view::iterator find_next_delim
        (
            std::string_view::iterator const first,
            std::string_view::iterator const last
        ) const noexcept
        {
            if constexpr ( is_set( iterator_options, split_options::split_by_whitespace ) )
            {
                auto const whitespace { std::find( first, last, whitespace_char ) };
                auto const other_delim{ std::find_first_of( first, last, std::begin( delims_ ), std::end( delims_ ) ) };
                return std::min( whitespace, other_delim );
            }
            else
            {
                return std::find_first_of( first, last, std::begin( delims_ ), std::end( delims_ ) );
            }
        }

        /**
         * Finds the iterator pointing to the first quote character in the specified range.
         *
         * @param first Iterator to the first element of the range to examine
         * @param last  Iterator to the last element of the range to examine
         *
         * @return Iterator to the first quote character
         */
        [[ nodiscard ]] constexpr std::string_view::iterator find_next_quote
        (
            std::string_view::iterator const first,
            std::string_view::iterator const last
        ) const noexcept
        {
            return std::find( first, last, iterator_quote_char );
        }

        /**
         * Skips all the characters equal to the specified character.
         *
         * @param first Iterator to the first element to examine
         *
         * @return Iterator to the first element which is not equal to the specified character
         */
        [[ nodiscard ]] constexpr std::string_view::iterator skip_char
        (
            std::string_view::iterator       first,
            char                       const c
        ) const noexcept
        {
            while ( first != std::end( strv_ ) && c == *first )
            {
                first = std::next( first );
            }
            return first;
        }

        /**
         * Finds the next token and sets the current element properties.
         */
        constexpr void next() noexcept
        {
            if ( prev_ == std::end( strv_ ) ) { return; }

            curr_value_.is_quoted = false;
            curr_ = find_next( prev_, std::end( strv_ ) );

            if ( curr_ != std::end( strv_ ) )
            {
                if ( *curr_ == iterator_quote_char )
                {
                    curr_value_.is_quoted = true;
                    prev_ = skip_char( curr_, iterator_quote_char );
                    curr_ = find_next( prev_, std::end( strv_ ) );
                }
                else
                {
                    if ( prev_ == curr_) { curr_ = std::next( curr_ ); }
                }
            }

            if ( std::distance( std::begin( strv_ ), prev_ ) > std::distance( std::begin( strv_ ), std::end( strv_ ) ) )
            {
                prev_ = std::end( strv_ );
                return;
            }

            curr_value_.value = strv_.substr
            (
                std::distance( std::begin( strv_ ), prev_ ),
                std::distance( prev_, curr_ )
            );
        }

    private:
        std::string_view strv_  {};
        std::string_view delims_{};

        std::string_view::iterator prev_;
        std::string_view::iterator curr_;

        value_type curr_value_{};
    };

public:
    constexpr split_range() noexcept = default;

	constexpr split_range( std::string_view const strv, std::string_view const delims = " " ) noexcept
        : strv_  { strv   }
        , delims_{ delims }
    {}

    constexpr split_range( split_range       && rhs ) noexcept = default;
    constexpr split_range( split_range const  & rhs ) noexcept = default;

    split_range& operator=( split_range       && rhs ) noexcept = default;
    split_range& operator=( split_range const  & rhs ) noexcept = default;

    ~split_range() noexcept = default;

    /**
     * Returns an iterator to the first element of the range.
     *
     * @return Iterator to the first element
     */
    [[ nodiscard ]] constexpr auto begin() const noexcept
    {
        return iterator< options, quote_char >{ strv_, delims_ };
    }

    /**
     * Returns an iterator to the element following the last element of the range.
     *
     * @return Iterator to the element following the last element
     */
    [[ nodiscard ]] constexpr auto end() const noexcept
    {
        return iterator< options, quote_char >{ std::end( strv_ ) };
    }

private:
    std::string_view strv_  {};
    std::string_view delims_{};
};

} // namespace booleval::utils

#endif // BOOLEVAL_SPLIT_RANGE_HPP