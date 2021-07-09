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

#ifndef BOOLEVAL_EVALUATOR_HPP
#define BOOLEVAL_EVALUATOR_HPP

#include <map>
#include <string_view>

#include <booleval/result.hpp>
#include <booleval/utils/any_mem_fn.hpp>
#include <booleval/tree/result_visitor.hpp>
#include <booleval/tree/tree.hpp>

namespace booleval
{

/**
 * @class evaluator
 *
 * Represents a class for evaluating logical expressions in a form of a string.
 * It builds an expression tree and traverses that tree in order to evaluate fields.
 */
template< typename MemFn = utils::any_mem_fn >
class evaluator
{
    using field_map = std::map< std::string_view, MemFn >;

public:
    evaluator() = default;
    evaluator( evaluator       && rhs ) = default;
    evaluator( evaluator const  & rhs ) = default;

    evaluator( field_map const & fields )
    {
        result_visitor_.fields( fields );
    }

    evaluator& operator=( evaluator       && rhs ) = default;
    evaluator& operator=( evaluator const  & rhs ) = default;

    ~evaluator() = default;

    /**
     * Sets the key - member function map used for evaluation of expression tree.
     *
     * @param fields Key - member function map
     */
    void fields( field_map const & fields ) noexcept
    {
        result_visitor_.fields( fields );
    }

    /**
     * Checks whether the evaluation is activated or not, i.e.
     * if the expression tree is successfully built.
     *
     * @return True if the evaluation is activated, otherwise false
     */
    [[ nodiscard ]] bool is_activated() const noexcept
    {
        return is_activated_;
    }

    /**
     * Sets the expression to be used for evaluation.
     *
     * @param expression Expression to be used for evaluation
     *
     * @return True if the expression is valid, otherwise false
     */
    [[ nodiscard ]] bool expression( std::string_view expression )
    {
        is_activated_ = false;

        if ( expression.empty() ) { return true; }

        root_ = tree::build( expression );
        if ( root_ != nullptr )
        {
            is_activated_ = true;
        }

        return is_activated_;
    }

    /**
     * Evaluates expression tree for the object passed in.
     *
     * @param obj Object to be evaluated
     *
     * @return True if the object's members satisfy the expression, otherwise false
     */
    template< typename T >
    [[ nodiscard ]] result evaluate( T const & obj )
    {
        if ( is_activated_ )
        {
            return result_visitor_.visit( *root_, obj );
        }
        else
        {
            return
            {
                .success = false,
                .message = "Evaluator not activated"
            };
        }
    }

private:
    bool                          is_activated_{ false   };
    std::shared_ptr< tree::node > root_        { nullptr };
    tree::result_visitor< MemFn > result_visitor_;
};

} // namespace booleval

#endif // BOOLEVAL_EVALUATOR_HPP