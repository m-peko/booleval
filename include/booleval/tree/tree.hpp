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

#ifndef BOOLEVAL_TREE_HPP
#define BOOLEVAL_TREE_HPP

#include <memory>
#include <vector>
#include <string_view>

#include <booleval/tree/node.hpp>
#include <booleval/token/tokenizer.hpp>

namespace booleval::tree
{

namespace internal
{

    using tokens = std::vector< token::token >;

    // Forward declarations

    std::unique_ptr< node > parse_expression          ( tokens const & tokens, std::size_t & current );
    std::unique_ptr< node > parse_and_operation       ( tokens const & tokens, std::size_t & current );
    std::unique_ptr< node > parse_parentheses         ( tokens const & tokens, std::size_t & current );
    std::unique_ptr< node > parse_relational_operation( tokens const & tokens, std::size_t & current );
    std::unique_ptr< node > parse_terminal            ( tokens const & tokens, std::size_t & current );

    // Definitions

    inline bool has_unused( tokens const & tokens, std::size_t const current ) noexcept
    {
        return current < std::size( tokens );
    }

    inline std::unique_ptr< node > parse_expression( tokens const & tokens, std::size_t & current )
    {
        auto left{ parse_and_operation( tokens, current ) };

        auto const is_relational_operator
        {
            has_unused( tokens, current ) &&
            tokens[ current ].is_one_of
            (
                token::token_type::eq,
                token::token_type::neq,
                token::token_type::gt,
                token::token_type::lt,
                token::token_type::geq,
                token::token_type::leq
            )
        };

        if ( is_relational_operator )
        {
            return nullptr;
        }

        if ( has_unused( tokens, current ) && tokens[ current ].is_not( token::token_type::logical_or ) )
        {
            return left;
        }

        while ( has_unused( tokens, current ) && tokens[ current ].is( token::token_type::logical_or ) )
        {
            ++current;
            auto logical_or{ std::make_unique< node >( token::token_type::logical_or ) };

            auto right{ parse_and_operation( tokens, current ) };
            if ( right == nullptr ) { return nullptr; }

            logical_or->left  = std::move( left  );
            logical_or->right = std::move( right );
            left = std::move( logical_or );
        }

        return left;
    }

    inline std::unique_ptr< node > parse_and_operation( tokens const & tokens, std::size_t & current )
    {
        auto left{ parse_parentheses( tokens, current ) };
        if ( left == nullptr )
        {
            left = parse_relational_operation( tokens, current );
        }

        if ( left == nullptr ) { return nullptr; }

        while ( has_unused( tokens, current ) && tokens[ current ].is( token::token_type::logical_and ) )
        {
            ++current;

            auto logical_and{ std::make_unique< node >( token::token_type::logical_and ) };

            auto right{ parse_parentheses( tokens, current ) };
            if ( right == nullptr )
            {
                right = parse_relational_operation( tokens, current );
            }

            if ( right == nullptr ) { return nullptr; }

            logical_and->left  = std::move( left  );
            logical_and->right = std::move( right );
            left = std::move( logical_and );
        }

        return left;
    }

    inline std::unique_ptr< node > parse_parentheses( tokens const & tokens, std::size_t & current )
    {
        if ( !has_unused( tokens, current ) ) { return nullptr; }

        if ( tokens[ current ].is( token::token_type::lp ) )
        {
            ++current;
            auto expression{ parse_expression( tokens, current ) };
            if ( !has_unused( tokens, current ) ) { return nullptr; }

            if ( tokens[ current++ ].is( token::token_type::rp ) )
            {
                return expression;
            }
        }

        return nullptr;
    }

    inline std::unique_ptr< node > parse_relational_operation( tokens const & tokens, std::size_t & current )
    {
        auto left{ parse_terminal( tokens, current ) };
        if ( left == nullptr ) { return nullptr; }

        if ( !has_unused( tokens, current ) ) { return nullptr; }

        auto operation{ std::make_unique< node >( tokens[ current++ ] ) };

        auto right{ parse_terminal( tokens, current ) };
        if ( right == nullptr ) { return nullptr; }

        operation->left  = std::move( left  );
        operation->right = std::move( right );

        return operation;
    }

    inline std::unique_ptr< node > parse_terminal( tokens const & tokens, std::size_t & current )
    {
        if ( !has_unused( tokens, current ) ) { return nullptr; }

        auto const & token{ tokens[ current++ ] };
        if ( token.is( token::token_type::field ) )
        {
            return std::make_unique< node >( token );
        }
        else
        {
            return nullptr;
        }
    }

} // namespace internal

/**
 * Builds an expression tree by using a recursive descent parser method.
 */
std::unique_ptr< node > build( std::string_view expression )
{
    auto const tokens{ token::tokenize( expression ) };
    if ( tokens.empty() ) { return nullptr; }

    std::size_t current{ 0u };

    return internal::parse_expression( tokens, current );
}

} // namespace booleval::tree

#endif // BOOLEVAL_TREE_HPP