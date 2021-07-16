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

#include <gtest/gtest.h>
#include <booleval/tree/node.hpp>
#include <booleval/token/token_type.hpp>
#include <booleval/tree/result_visitor.hpp>

namespace
{

    template< typename T >
    class foo
    {
    public:
        foo()             : value_{}        {}
        foo( T && value ) : value_{ value } {}

        void value( T && value ) { value_ = value; }

        T value() const noexcept { return value_; }

    private:
        T value_{};
    };

    template< typename T, typename U >
    class bar
    {
    public:
        bar( T && value_1, U && value_2 )
        : value_1_{ value_1 }
        , value_2_{ value_2 }
        {}

        void value_1( T && value ) { value_1_ = value; }
        void value_2( U && value ) { value_2_ = value; }

        T value_1() const noexcept { return value_1_; }
        U value_2() const noexcept { return value_2_; }

    private:
        T value_1_{};
        U value_2_{};
    };

    std::unique_ptr< booleval::tree::node > make_tree_node( booleval::token::token_type const type ) noexcept
    {
        return std::make_unique< booleval::tree::node >( type );
    }

    std::unique_ptr< booleval::tree::node > make_tree_node( booleval::token::token_type const type, std::string_view const value ) noexcept
    {
        return std::make_unique< booleval::tree::node >( booleval::token::token{ type, value } );
    }

} // namespace

TEST( ResultVisitorTest, AndTreeNode )
{
    using namespace booleval;

    bar< unsigned, unsigned > x{ 1, 2 };
    bar< unsigned, unsigned > y{ 2, 3 };
    bar< unsigned, unsigned > z{ 3, 4 };

    tree::result_visitor visitor;

    visitor.fields
    (
        {
            make_field( "field_1", &bar< unsigned, unsigned >::value_1 ),
            make_field( "field_2", &bar< unsigned, unsigned >::value_2 )
        }
    );

    auto and_op{ make_tree_node( token::token_type::logical_and ) };

    and_op->left  = make_tree_node( token::token_type::eq );
    and_op->right = make_tree_node( token::token_type::eq );

    and_op->left->left  = make_tree_node( token::token_type::field, "field_1" );
    and_op->left->right = make_tree_node( token::token_type::field, "1"       );

    and_op->right->left  = make_tree_node( token::token_type::field, "field_2" );
    and_op->right->right = make_tree_node( token::token_type::field, "2"       );

    ASSERT_TRUE ( visitor.visit( *and_op, x ).success );
    ASSERT_FALSE( visitor.visit( *and_op, y ).success );
    ASSERT_FALSE( visitor.visit( *and_op, z ).success );
}

TEST( ResultVisitorTest, OrTreeNode )
{
    using namespace booleval;

    foo< unsigned > x{ 1 };
    foo< unsigned > y{ 2 };
    foo< unsigned > z{ 3 };

    tree::result_visitor visitor;
    visitor.fields
    (
        {
            make_field( "field", &foo< unsigned >::value )
        }
    );

    auto or_op{ make_tree_node( token::token_type::logical_or ) };

    or_op->left  = make_tree_node( token::token_type::eq );
    or_op->right = make_tree_node( token::token_type::eq );

    or_op->left->left  = make_tree_node( token::token_type::field, "field" );
    or_op->left->right = make_tree_node( token::token_type::field, "1"     );

    or_op->right->left  = make_tree_node( token::token_type::field, "field" );
    or_op->right->right = make_tree_node( token::token_type::field, "2"     );

    ASSERT_TRUE ( visitor.visit( *or_op, x ).success );
    ASSERT_TRUE ( visitor.visit( *or_op, y ).success );
    ASSERT_FALSE( visitor.visit( *or_op, z ).success );
}

TEST( ResultVisitorTest, EqualToTreeNode )
{
    using namespace booleval;

    foo< unsigned > x{ 1 };
    foo< unsigned > y{ 2 };

    tree::result_visitor visitor;
    visitor.fields
    (
        {
            make_field( "field", &foo< unsigned >::value )
        }
    );

    auto eq_op{ make_tree_node( token::token_type::eq ) };

    eq_op->left  = make_tree_node( token::token_type::field, "field" );
    eq_op->right = make_tree_node( token::token_type::field, "1"     );

    ASSERT_TRUE ( visitor.visit( *eq_op, x ).success );
    ASSERT_FALSE( visitor.visit( *eq_op, y ).success );
}

TEST( ResultVisitorTest, NotEqualToTreeNode )
{
    using namespace booleval;

    foo< unsigned > x{ 1 };
    foo< unsigned > y{ 2 };

    tree::result_visitor visitor;
    visitor.fields
    (
        {
            make_field( "field", &foo< unsigned >::value )
        }
    );

    auto neq_op{ make_tree_node( token::token_type::neq ) };

    neq_op->left  = make_tree_node( token::token_type::field, "field" );
    neq_op->right = make_tree_node( token::token_type::field, "1"     );

    ASSERT_FALSE( visitor.visit( *neq_op, x ).success );
    ASSERT_TRUE ( visitor.visit( *neq_op, y ).success );
}

TEST( ResultVisitorTest, GreaterThanTreeNode )
{
    using namespace booleval;

    foo< unsigned > x{ 0 };
    foo< unsigned > y{ 1 };
    foo< unsigned > z{ 2 };

    tree::result_visitor visitor;
    visitor.fields
    (
        {
            make_field( "field", &foo< unsigned >::value )
        }
    );

    auto gt_op{ make_tree_node( token::token_type::gt ) };

    gt_op->left  = make_tree_node( token::token_type::field, "field" );
    gt_op->right = make_tree_node( token::token_type::field, "1"     );

    ASSERT_FALSE( visitor.visit( *gt_op, x ).success );
    ASSERT_FALSE( visitor.visit( *gt_op, y ).success );
    ASSERT_TRUE ( visitor.visit( *gt_op, z ).success );
}

TEST( ResultVisitorTest, LessThanTreeNode )
{
    using namespace booleval;

    foo< unsigned > x{ 0 };
    foo< unsigned > y{ 1 };
    foo< unsigned > z{ 2 };

    tree::result_visitor visitor;
    visitor.fields
    (
        {
            make_field( "field", &foo< unsigned >::value )
        }
    );

    auto lt_op{ make_tree_node( token::token_type::lt ) };

    lt_op->left  = make_tree_node( token::token_type::field, "field" );
    lt_op->right = make_tree_node( token::token_type::field, "1"     );

    ASSERT_TRUE ( visitor.visit( *lt_op, x ).success );
    ASSERT_FALSE( visitor.visit( *lt_op, y ).success );
    ASSERT_FALSE( visitor.visit( *lt_op, z ).success );
}

TEST( ResultVisitorTest, GreaterThanOrEqualToTreeNode )
{
    using namespace booleval;

    foo< unsigned > x{ 0 };
    foo< unsigned > y{ 1 };
    foo< unsigned > z{ 2 };

    tree::result_visitor visitor;
    visitor.fields
    (
        {
            make_field( "field", &foo< unsigned >::value )
        }
    );

    auto geq_op{ make_tree_node( token::token_type::geq ) };

    geq_op->left  = make_tree_node( token::token_type::field, "field" );
    geq_op->right = make_tree_node( token::token_type::field, "1"     );

    ASSERT_FALSE( visitor.visit( *geq_op, x ).success );
    ASSERT_TRUE ( visitor.visit( *geq_op, y ).success );
    ASSERT_TRUE ( visitor.visit( *geq_op, z ).success );
}

TEST( ResultVisitorTest, LessThanOrEqualToTreeNode )
{
    using namespace booleval;

    foo< unsigned > x{ 0 };
    foo< unsigned > y{ 1 };
    foo< unsigned > z{ 2 };

    tree::result_visitor visitor;
    visitor.fields
    (
        {
            make_field( "field", &foo< unsigned >::value )
        }
    );

    auto leq_op{ make_tree_node( token::token_type::leq ) };

    leq_op->left  = make_tree_node( token::token_type::field, "field" );
    leq_op->right = make_tree_node( token::token_type::field, "1"     );

    ASSERT_TRUE ( visitor.visit( *leq_op, x ).success );
    ASSERT_TRUE ( visitor.visit( *leq_op, y ).success );
    ASSERT_FALSE( visitor.visit( *leq_op, z ).success );
}

TEST( ResultVisitorTest, UnknownTreeNode )
{
    using namespace booleval;

    foo< unsigned > x{ 1 };
    foo< unsigned > y{ 2 };

    tree::result_visitor visitor;
    visitor.fields
    (
        {
            make_field( "field", &foo< unsigned >::value )
        }
    );

    auto eq_op{ make_tree_node( token::token_type::eq ) };

    eq_op->left  = make_tree_node( token::token_type::field, "unknown_field" );
    eq_op->right = make_tree_node( token::token_type::field, "1"             );

    {
        auto const result{ visitor.visit( *eq_op, x ) };
        ASSERT_FALSE( result.success                  );
        ASSERT_EQ   ( result.message, "Unknown field" );
    }
    {
        auto const result{ visitor.visit( *eq_op, y ) };
        ASSERT_FALSE( result.success                  );
        ASSERT_EQ   ( result.message, "Unknown field" );
    }
}
