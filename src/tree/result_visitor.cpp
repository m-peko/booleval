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

#include <booleval/tree/result_visitor.h>

namespace booleval {

namespace tree {

void result_visitor::fields(field_map const& fields) noexcept {
    fields_ = fields;
}

result_visitor::field_map const& result_visitor::fields() const noexcept {
    return fields_;
}

bool result_visitor::visit_and(tree_node const& node) {
    return visit(*node.left) && visit(*node.right);
}

bool result_visitor::visit_or(tree_node const& node) {
    return visit(*node.left) || visit(*node.right);
}

bool result_visitor::visit_eq(tree_node const& node) {
    auto key = node.left->token;
    auto value = node.right->token;
    return fields_[key.value()] == value.value();
}

bool result_visitor::visit_neq(tree_node const& node) {
    auto key = node.left->token;
    auto value = node.right->token;
    return fields_[key.value()] != value.value();
}

bool result_visitor::visit_gt(tree_node const& node) {
    auto key = node.left->token;
    auto value = node.right->token;
    return fields_[key.value()] > value.value();
}

bool result_visitor::visit_lt(tree_node const& node) {
    auto key = node.left->token;
    auto value = node.right->token;
    return fields_[key.value()] < value.value();
}

bool result_visitor::visit_geq(tree_node const& node) {
    auto key = node.left->token;
    auto value = node.right->token;
    return fields_[key.value()] >= value.value();
}

bool result_visitor::visit_leq(tree_node const& node) {
    auto key = node.left->token;
    auto value = node.right->token;
    return fields_[key.value()] <= value.value();
}

} // tree

} // booleval