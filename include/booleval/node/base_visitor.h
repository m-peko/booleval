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

#include <booleval/node/tree_node.h>
#include <booleval/token/token_type.h>

namespace booleval {

namespace node {

template <typename ReturnType>
class BaseVisitor {
public:
    BaseVisitor() = default;
    BaseVisitor(BaseVisitor&& other) = default;
    BaseVisitor(BaseVisitor const& other) = default;

    BaseVisitor& operator=(BaseVisitor&& other) = default;
    BaseVisitor& operator=(BaseVisitor const& other) = default;

    ~BaseVisitor() = default;

    ReturnType visit(TreeNode const& node);

    virtual ReturnType visit_and(TreeNode const& node) = 0;
    virtual ReturnType visit_or(TreeNode const& node) = 0;
    virtual ReturnType visit_eq(TreeNode const& node) = 0;
    virtual ReturnType visit_neq(TreeNode const& node) = 0;
    virtual ReturnType visit_gt(TreeNode const& node) = 0;
    virtual ReturnType visit_lt(TreeNode const& node) = 0;
};

template <typename ReturnType>
ReturnType BaseVisitor<ReturnType>::visit(TreeNode const& node) {
    switch (node.token->type()) {
    case token::TokenType::AND:
        return visit_and(node);

    case token::TokenType::OR:
        return visit_or(node);

    case token::TokenType::EQ:
        return visit_eq(node);

    case token::TokenType::NEQ:
        return visit_neq(node);

    case token::TokenType::GT:
        return visit_gt(node);

    case token::TokenType::LT:
        return visit_lt(node);

    default:
        return ReturnType{};
    }
}

} // node

} // booleval

#endif // BOOLEVAL_BASE_VISITOR_H