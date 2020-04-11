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

#ifndef BOOLEVAL_RESULT_VISITOR_H
#define BOOLEVAL_RESULT_VISITOR_H

#include <map>
#include <functional>
#include <string_view>
#include <booleval/tree/tree_node.h>
#include <booleval/utils/any_mem_fn.h>

namespace booleval {

namespace tree {

/**
 * class result_visitor
 *
 * Represents a visitor for expression tree nodes in order to get the
 * final result of the expression based on the fields of an object being passed.
 */
class result_visitor {
    using field_map = std::map<std::string_view, utils::any_mem_fn>;

public:
    result_visitor() = default;
    result_visitor(result_visitor&& rhs) = default;
    result_visitor(result_visitor const& rhs) = default;

    result_visitor& operator=(result_visitor&& rhs) = default;
    result_visitor& operator=(result_visitor const& rhs) = default;

    ~result_visitor() = default;

    /**
     * Sets the key - member function map used for evaluation of expression tree.
     *
     * @param fields Key - member function map
     */
    void fields(field_map const& fields) noexcept {
        fields_ = fields;
    }

    /**
     * Visits tree node by checking token type and passing node itself
     * to specialized visitor's function.
     *
     * @param node Currently visited tree node
     *
     * @return ReturnType
     */
    template <typename T>
    [[nodiscard]] bool visit(tree_node const& node, T const& obj);

private:

    /**
     * Visits tree node representing one of logical operations.
     *
     * @param node Currently visited tree node
     * @param obj  Object to be evaluated
     * @param func Logical operation function
     *
     * @return Result of logical operation
     */
    template <typename T, typename F>
    [[nodiscard]] bool visit_logical(tree_node const& node, T const& obj, F&& func) {
        return func(visit(*node.left, obj), visit(*node.right, obj));
    }

    /**
     * Visits tree node representing one of relational operations.
     *
     * @param node Currently visited tree node
     * @param obj  Object to be evaluated
     * @param func Comparison function
     *
     * @return Result of relational operation
     */
    template <typename T, typename F>
    [[nodiscard]] bool visit_relational(tree_node const& node, T const& obj, F&& func) {
        auto key = node.left->token;
        auto value = node.right->token;
        return func(fields_[key.value()].invoke(obj), value.value());
    }

private:
    field_map fields_;
};

template <typename T>
bool result_visitor::visit(tree_node const& node, T const& obj) {
    if (nullptr == node.left || nullptr == node.right) {
        return false;
    }

    switch (node.token.type()) {
    case token::token_type::logical_and:
        return visit_logical(node, obj, std::logical_and<>());

    case token::token_type::logical_or:
        return visit_logical(node, obj, std::logical_or<>());

    case token::token_type::eq:
        return visit_relational(node, obj, std::equal_to<>());

    case token::token_type::neq:
        return visit_relational(node, obj, std::not_equal_to<>());

    case token::token_type::gt:
        return visit_relational(node, obj, std::greater<>());

    case token::token_type::lt:
        return visit_relational(node, obj, std::less<>());

    case token::token_type::geq:
        return visit_relational(node, obj, std::greater_equal<>());

    case token::token_type::leq:
        return visit_relational(node, obj, std::less_equal<>());

    default:
        return false;
    }
}

} // tree

} // booleval

#endif // BOOLEVAL_RESULT_VISITOR_H