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

#ifndef BOOLEVAL_BASE_VISITOR_H
#define BOOLEVAL_BASE_VISITOR_H

#include <booleval/tree/tree_node.h>
#include <booleval/token/token_type.h>

namespace booleval {

namespace tree {

/**
 * class base_visitor
 *
 * Represents a base class for any other visitor class. It contains set of
 * functions for visiting tree nodes of different types.
 */
template <typename ReturnType>
class base_visitor {
public:
    base_visitor() = default;
    base_visitor(base_visitor&& rhs) = default;
    base_visitor(base_visitor const& rhs) = default;

    base_visitor& operator=(base_visitor&& rhs) = default;
    base_visitor& operator=(base_visitor const& rhs) = default;

    ~base_visitor() = default;

    /**
     * Visits tree node by checking token type and passing node itself
     * to specialized visitor's function.
     *
     * @param node Currently visited tree node
     *
     * @return ReturnType
     */
    [[nodiscard]] ReturnType visit(tree_node const& node);

    /**
     * Visits tree node representing logical operation AND.
     *
     * @param node Currently visited tree node
     *
     * @return ReturnType
     */
    [[nodiscard]] virtual ReturnType visit_and(tree_node const& node) = 0;

    /**
     * Visits tree node representing logical operation OR.
     *
     * @param node Currently visited tree node
     *
     * @return ReturnType
     */
    [[nodiscard]] virtual ReturnType visit_or(tree_node const& node) = 0;

    /**
     * Visits tree node representing relational operation EQ (EQUAL TO).
     *
     * @param node Currently visited tree node
     *
     * @return ReturnType
     */
    [[nodiscard]] virtual ReturnType visit_eq(tree_node const& node) = 0;

    /**
     * Visits tree node representing relational operation NEQ (NOT EQUAL TO).
     *
     * @param node Currently visited tree node
     *
     * @return ReturnType
     */
    [[nodiscard]] virtual ReturnType visit_neq(tree_node const& node) = 0;

    /**
     * Visits tree node representing relational operation GT (GREATER THAN).
     *
     * @param node Currently visited tree node
     *
     * @return ReturnType
     */
    [[nodiscard]] virtual ReturnType visit_gt(tree_node const& node) = 0;

    /**
     * Visits tree node representing relational operation LT (LESS THAN).
     *
     * @param node Currently visited tree node
     *
     * @return ReturnType
     */
    [[nodiscard]] virtual ReturnType visit_lt(tree_node const& node) = 0;

    /**
     * Visits tree node representing relational operation GEQ (GREATER THAN OR EQUAL TO).
     *
     * @param node Currently visited tree node
     *
     * @return ReturnType
     */
    [[nodiscard]] virtual ReturnType visit_geq(tree_node const& node) = 0;

    /**
     * Visits tree node representing relational operation LEQ (LESS THAN OR EQUAL TO).
     *
     * @param node Currently visited tree node
     *
     * @return ReturnType
     */
    [[nodiscard]] virtual ReturnType visit_leq(tree_node const& node) = 0;
};

template <typename ReturnType>
ReturnType base_visitor<ReturnType>::visit(tree_node const& node) {
    if (nullptr == node.left || nullptr == node.right) {
        return ReturnType{};
    }

    switch (node.token.type()) {
    case token::token_type::logical_and:
        return visit_and(node);

    case token::token_type::logical_or:
        return visit_or(node);

    case token::token_type::eq:
        return visit_eq(node);

    case token::token_type::neq:
        return visit_neq(node);

    case token::token_type::gt:
        return visit_gt(node);

    case token::token_type::lt:
        return visit_lt(node);

    case token::token_type::geq:
        return visit_geq(node);

    case token::token_type::leq:
        return visit_leq(node);

    default:
        return ReturnType{};
    }
}

} // tree

} // booleval

#endif // BOOLEVAL_BASE_VISITOR_H