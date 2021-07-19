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

#ifndef BOOLEVAL_RESULT_VISITOR_HPP
#define BOOLEVAL_RESULT_VISITOR_HPP

#include <memory>
#include <vector>
#include <functional>
#include <string_view>

#include <booleval/field.hpp>
#include <booleval/result.hpp>
#include <booleval/tree/node.hpp>

namespace booleval::tree
{

using namespace std::string_literals;

/**
 * @class result_visitor
 *
 * Represents a visitor for expression tree nodes in order to get the
 * final result of the expression based on the fields of an object being passed.
 */
class result_visitor
{
public:
    /**
     * Sets the fields used for evaluation of expression tree.
     *
     * @param fields Fields to be used in evaluation process
     */
    void fields( std::initializer_list< field_base * > fields ) noexcept
    {
        fields_ = std::vector< std::unique_ptr< field_base > >{ std::begin( fields ), std::end( fields ) };
    }

    /**
     * Visits tree node by checking token type and passing node itself
     * to specialized visitor's function.
     *
     * @param node Currently visited tree node
     *
     * @return Result
     */
    template< typename T >
    [[ nodiscard ]] constexpr result visit( node const & node, T && obj ) const noexcept;

private:
    /**
     * Visits tree node representing one of logical operations.
     *
     * @param node Currently visited tree node
     * @param obj  Object to be evaluated
     * @param func Logical operation function
     *
     * @return Result
     */
    template< typename T, typename F >
    [[ nodiscard ]] constexpr result visit_logical( node const & node, T && obj, F && f ) const noexcept
    {
        auto const left { visit( *node.left , std::forward< T >( obj ) ) };
        auto const right{ visit( *node.right, std::forward< T >( obj ) ) };

        // always pick the error message closer to the beginning of the expression
        auto const message
        {
            left.message.empty() ? right.message : left.message
        };

        return { f( left.success, right.success ), message };
    }

    /**
     * Visits tree node representing one of relational operations.
     *
     * @param node Currently visited tree node
     * @param obj  Object to be evaluated
     * @param func Comparison function
     *
     * @return Result
     */
    template< typename T, typename F >
    [[ nodiscard ]] constexpr result visit_relational( node const & node, T && obj, F && f ) const noexcept
    {
        auto const key{ node.left->token };

        auto const it
        {
            std::find_if
            (
                std::cbegin( fields_ ),
                std::cend  ( fields_ ),
                [ key ]( auto && field ) noexcept
                {
                    return field->name == key.value();
                }
            )
        };

        if ( it == std::end( fields_ ) )
        {
            return { false, "Unknown field" };
        }

        auto const success
        {
            f
            (
                ( *it )->template invoke( std::forward< T >( obj ) ),
                node.right->token.value()
            )
        };

        return { success };
    }

private:
    std::vector< std::unique_ptr< field_base > > fields_;
};

template< typename T >
constexpr result result_visitor::visit( node const & node, T && obj ) const noexcept
{
    if ( nullptr == node.left || nullptr == node.right )
    {
        return { false, "Missing operand" };
    }

    switch ( node.token.type() )
    {
        case token::token_type::logical_and: return visit_logical   ( node, std::forward< T >( obj ), std::logical_and<>()   );
        case token::token_type::logical_or : return visit_logical   ( node, std::forward< T >( obj ), std::logical_or<>()    );
        case token::token_type::eq         : return visit_relational( node, std::forward< T >( obj ), std::equal_to<>()      );
        case token::token_type::neq        : return visit_relational( node, std::forward< T >( obj ), std::not_equal_to<>()  );
        case token::token_type::gt         : return visit_relational( node, std::forward< T >( obj ), std::greater<>()       );
        case token::token_type::lt         : return visit_relational( node, std::forward< T >( obj ), std::less<>()          );
        case token::token_type::geq        : return visit_relational( node, std::forward< T >( obj ), std::greater_equal<>() );
        case token::token_type::leq        : return visit_relational( node, std::forward< T >( obj ), std::less_equal<>()    );

        default:
            return { false, "Unknown token type" };
    }
}

} // namespace booleval::tree

#endif // BOOLEVAL_RESULT_VISITOR_HPP
