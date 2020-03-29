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
#include <string_view>
#include <booleval/tree/tree_node.h>
#include <booleval/utils/any_value.h>
#include <booleval/tree/base_visitor.h>

namespace booleval {

namespace tree {

/**
 * class result_visitor
 *
 * Represents a visitor for expression tree nodes in order to get the
 * final result of the expression based on the specified field map.
 */
class result_visitor : public base_visitor<bool> {
    using field_map = std::map<std::string_view, utils::any_value>;

public:
    result_visitor() = default;
    result_visitor(result_visitor&& rhs) = default;
    result_visitor(result_visitor const& rhs) = default;

    result_visitor& operator=(result_visitor&& rhs) = default;
    result_visitor& operator=(result_visitor const& rhs) = default;

    ~result_visitor() = default;

    /**
     * Sets the key value map used for evaluation of expression tree.
     *
     * @param fields Key value map
     */
    void fields(field_map const& fields) noexcept;

    /**
     * Gets the key value map used for evaluation of expression tree.
     *
     * @return Key value map
     */
    [[nodiscard]] field_map const& fields() const noexcept;

    /**
     * Visits tree node representing logical operation AND.
     *
     * @param node Currently visited tree node
     *
     * @return Result of logical operation AND
     */
    [[nodiscard]] bool visit_and(tree_node const& node) override;

    /**
     * Visits tree node representing logical operation OR.
     *
     * @param node Currently visited tree node
     *
     * @return Result of logical operation OR
     */
    [[nodiscard]] bool visit_or(tree_node const& node) override;

    /**
     * Visits tree node representing relational operation EQ (EQUAL TO).
     *
     * @param node Currently visited tree node
     *
     * @return Result of relational operation EQ
     */
    [[nodiscard]] bool visit_eq(tree_node const& node) override;

    /**
     * Visits tree node representing relational operation NEQ (NOT EQUAL TO).
     *
     * @param node Currently visited tree node
     *
     * @return Result of relational operation NEQ
     */
    [[nodiscard]] bool visit_neq(tree_node const& node) override;

    /**
     * Visits tree node representing relational operation GT (GREATER THAN).
     *
     * @param node Currently visited tree node
     *
     * @return Result of relational operation GT
     */
    [[nodiscard]] bool visit_gt(tree_node const& node) override;

    /**
     * Visits tree node representing relational operation LT (LESS THAN).
     *
     * @param node Currently visited tree node
     *
     * @return Result of relational operation LT
     */
    [[nodiscard]] bool visit_lt(tree_node const& node) override;

    /**
     * Visits tree node representing relational operation GEQ (GREATER THAN OR EQUAL TO).
     *
     * @param node Currently visited tree node
     *
     * @return Result of relational operation GT
     */
    [[nodiscard]] bool visit_geq(tree_node const& node) override;

    /**
     * Visits tree node representing relational operation LEQ (LESS THAN OR EQUAL TO).
     *
     * @param node Currently visited tree node
     *
     * @return Result of relational operation LT
     */
    [[nodiscard]] bool visit_leq(tree_node const& node) override;

private:
    field_map fields_;
};

} // tree

} // booleval

#endif // BOOLEVAL_RESULT_VISITOR_H