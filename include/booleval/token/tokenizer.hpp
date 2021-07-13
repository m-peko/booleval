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

#ifndef BOOLEVAL_TOKENIZER_HPP
#define BOOLEVAL_TOKENIZER_HPP

#include <vector>
#include <iostream>
#include <string_view>

#include <booleval/token/token.hpp>
#include <booleval/utils/string_utils.hpp>
#include <booleval/utils/split_options.hpp>
#include <booleval/utils/split_range.hpp>

namespace booleval::token
{

/**
 * Tokenizes given expression, i.e. transforms given expression
 * from string to the collection of token objects.
 */
inline std::vector< token > tokenize( std::string_view const expression ) noexcept
{
    std::vector< token > result;

    constexpr auto parentheses_symbols{ get_parentheses_symbols() };
    constexpr auto split_options
    {
        utils::split_options::include_delimiters  |
        utils::split_options::split_by_whitespace |
        utils::split_options::allow_quoted_strings
    };

    auto const delimiters
    {
        utils::join
        (
            std::cbegin( parentheses_symbols ),
            std::cend  ( parentheses_symbols )
        )
    };

    auto const tokens_range{ utils::split_range< split_options >( expression, delimiters ) };

    for ( auto const [ is_quoted, value ] : tokens_range )
    {
        if ( utils::is_whitespace( value ) ) { continue; }

        auto const type{ is_quoted ? token_type::field : to_token_type( value ) };

        if ( type == token_type::field )
        {
            if ( !result.empty() && result.back().is( token_type::field ) )
            {
                result.emplace_back( token_type::eq, to_token_keyword( token_type::eq ) );
            }
        }

        result.emplace_back( type, value );
    }

    return result;
}

} // namespace booleval::token

#endif // BOOLEVAL_TOKENIZER_HPP