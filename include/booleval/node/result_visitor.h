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
#include <memory>
#include <string>
#include <booleval/node/tree_node.h>
#include <booleval/node/base_visitor.h>
#include <booleval/token/field_token.h>

namespace booleval {

namespace node {

/**
 * class ResultVisitor
 *
 * This class is used for visiting expression
 * tree nodes in order to get the final result of
 * the expression based on the specified field map.
 */
class ResultVisitor : public BaseVisitor<bool> {
    using FieldMap = std::map<std::string, std::string>;

public:
    ResultVisitor() = default;
    ResultVisitor(ResultVisitor&& other) = default;
    ResultVisitor(ResultVisitor const& other) = default;

    ResultVisitor& operator=(ResultVisitor&& other) = default;
    ResultVisitor& operator=(ResultVisitor const& other) = default;

    ~ResultVisitor() = default;

    /**
     * Setter for the key value map used for evaluation
     * of expression tree.
     *
     * @param fields Key value map
     */
    void fields(FieldMap const& fields) noexcept;

    /**
     * Getter for the key value map used for evaluation
     * of expression tree.
     *
     * @return Key value map
     */
    FieldMap const& fields() const noexcept;

    /**
     * Visits tree node representing logical operation AND.
     *
     * @param node Currently visited tree node
     *
     * @return Result of logical operation AND
     */
    virtual bool visit_and(TreeNode const& node);

    /**
     * Visits tree node representing logical operation OR.
     *
     * @param node Currently visited tree node
     *
     * @return Result of logical operation OR
     */
    virtual bool visit_or(TreeNode const& node);

    /**
     * Visits tree node representing relational operation
     * EQ (EQUAL TO).
     *
     * @param node Currently visited tree node
     *
     * @return Result of relational operation EQ
     */
    virtual bool visit_eq(TreeNode const& node);

    /**
     * Visits tree node representing relational operation
     * NEQ (NOT EQUAL TO).
     *
     * @param node Currently visited tree node
     *
     * @return Result of relational operation NEQ
     */
    virtual bool visit_neq(TreeNode const& node);

    /**
     * Visits tree node representing relational operation
     * GT (GREATER THAN).
     *
     * @param node Currently visited tree node
     *
     * @return Result of relational operation GT
     */
    virtual bool visit_gt(TreeNode const& node);

    /**
     * Visits tree node representing relational operation
     * LT (LESS THAN).
     *
     * @param node Currently visited tree node
     *
     * @return Result of relational operation LT
     */
    virtual bool visit_lt(TreeNode const& node);

private:
    /**
     * Extracts token from the left leaf node that represents
     * actual field that gets evaluated.
     *
     * @param node Tree node to extract token from
     *
     * @return Field token
     */
    std::shared_ptr<token::FieldToken<>> left_field_token(TreeNode const& node);

    /**
     * Extracts token from the right leaf node that represents
     * actual field that gets evaluated.
     *
     * @param node Tree node to extract token from
     *
     * @return Field token
     */
    std::shared_ptr<token::FieldToken<>> right_field_token(TreeNode const& node);

private:
    FieldMap fields_;
};

} // node

} // booleval

#endif // BOOLEVAL_RESULT_VISITOR_H