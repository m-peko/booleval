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

#include <booleval/node/result_visitor.h>

namespace booleval {

namespace node {

void ResultVisitor::fields(FieldMap const& fields) noexcept {
    fields_ = fields;
}

ResultVisitor::FieldMap const& ResultVisitor::fields() const noexcept {
    return fields_;
}

bool ResultVisitor::visit_and(TreeNode const& node) {
    return visit(*node.left) && visit(*node.right);
}

bool ResultVisitor::visit_or(TreeNode const& node) {
    return visit(*node.left) || visit(*node.right);
}

bool ResultVisitor::visit_eq(TreeNode const& node) {
    auto key = left_field_token(node);
    auto value = right_field_token(node);

    if (nullptr != key && nullptr != value) {
        return fields_[key->field()] == value->field();
    } else {
        return false;
    }
}

bool ResultVisitor::visit_neq(TreeNode const& node) {
    auto key = left_field_token(node);
    auto value = right_field_token(node);

    if (nullptr != key && nullptr != value) {
        return fields_[key->field()] != value->field();
    } else {
        return false;
    }
}

bool ResultVisitor::visit_gt(TreeNode const& node) {
    auto key = left_field_token(node);
    auto value = right_field_token(node);

    if (nullptr != key && nullptr != value) {
        return fields_[key->field()] > value->field();
    } else {
        return false;
    }
}

bool ResultVisitor::visit_lt(TreeNode const& node) {
    auto key = left_field_token(node);
    auto value = right_field_token(node);

    if (nullptr != key && nullptr != value) {
        return fields_[key->field()] < value->field();
    } else {
        return false;
    }
}

std::shared_ptr<token::FieldToken<>> ResultVisitor::left_field_token(TreeNode const& node) {
    return std::dynamic_pointer_cast<token::FieldToken<>>(node.left->token);
}

std::shared_ptr<token::FieldToken<>> ResultVisitor::right_field_token(TreeNode const& node) {
    return std::dynamic_pointer_cast<token::FieldToken<>>(node.right->token);
}

} // node

} // booleval