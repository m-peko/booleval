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
#include <string>
#include <memory>
#include <booleval/node/tree_node.h>
#include <booleval/node/base_visitor.h>
#include <booleval/token/field_token.h>

namespace booleval {

namespace node {

class ResultVisitor : public BaseVisitor<bool> {
    using FieldMap = std::map<std::string, std::string>;

public:
    ResultVisitor() = default;
    ResultVisitor(ResultVisitor&& other) = default;
    ResultVisitor(ResultVisitor const& other) = default;

    ResultVisitor& operator=(ResultVisitor&& other) = default;
    ResultVisitor& operator=(ResultVisitor const& other) = default;

    ~ResultVisitor() = default;

    void fields(FieldMap const& fields) noexcept;
    FieldMap const& fields() const noexcept;

    virtual bool visit_and(TreeNode const& node);
    virtual bool visit_or(TreeNode const& node);
    virtual bool visit_eq(TreeNode const& node);
    virtual bool visit_neq(TreeNode const& node);
    virtual bool visit_gt(TreeNode const& node);
    virtual bool visit_lt(TreeNode const& node);

private:
    std::shared_ptr<token::FieldToken<>> left_field_token(TreeNode const& node);
    std::shared_ptr<token::FieldToken<>> right_field_token(TreeNode const& node);

private:
    FieldMap fields_;
};

} // node

} // booleval

#endif // BOOLEVAL_RESULT_VISITOR_H