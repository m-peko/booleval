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

#include <booleval/token/token_type.h>
#include <booleval/tree/expression_tree.h>

namespace booleval {

namespace tree {

expression_tree::expression_tree()
    : root_(nullptr)
{}

std::shared_ptr<tree::tree_node> expression_tree::root() noexcept {
    return root_;
}

bool expression_tree::build(std::string_view expression) {
    tokenizer_.reset();
    tokenizer_.expression(expression);
    tokenizer_.tokenize();

    root_ = parse_expression();
    if (nullptr == root_) {
        return false;
    } else if (tokenizer_.has_tokens()) {
        root_ = nullptr;
        return false;
    }

    return true;
}

std::shared_ptr<tree::tree_node> expression_tree::parse_expression() {
    auto left = parse_and_operation();

    auto const is_relational_operator =
        tokenizer_.has_tokens() &&
        tokenizer_.weak_next_token().is_one_of(
            token::token_type::eq,
            token::token_type::neq,
            token::token_type::gt,
            token::token_type::lt,
            token::token_type::geq,
            token::token_type::leq
        );

    if (is_relational_operator) {
        return nullptr;
    }

    if (tokenizer_.has_tokens() && tokenizer_.weak_next_token().is_not(token::token_type::logical_or)) {
        return left;
    }

    while (tokenizer_.has_tokens() && tokenizer_.weak_next_token().is(token::token_type::logical_or)) {
        tokenizer_.pass_token();
        auto logical_or = std::make_shared<tree::tree_node>(token::token_type::logical_or);

        auto right = parse_and_operation();
        if (nullptr == right) {
            return nullptr;
        }

        logical_or->left  = left;
        logical_or->right = right;
        left = logical_or;
    }

    return left;
}

std::shared_ptr<tree::tree_node> expression_tree::parse_and_operation() {
    auto left = parse_parentheses();
    if (nullptr == left) {
        left = parse_relational_operation();
    }

    while (tokenizer_.has_tokens() && tokenizer_.weak_next_token().is(token::token_type::logical_and)) {
        tokenizer_.pass_token();

        auto logical_and = std::make_shared<tree::tree_node>(token::token_type::logical_and);

        auto right = parse_parentheses();
        if (nullptr == right) {
            right = parse_relational_operation();
        }

        if (nullptr == right) {
            return nullptr;
        }

        logical_and->left  = left;
        logical_and->right = right;
        left = logical_and;
    }

    return left;
}

std::shared_ptr<tree::tree_node> expression_tree::parse_parentheses() {
    if (tokenizer_.has_tokens() && tokenizer_.weak_next_token().is(token::token_type::lp)) {
        tokenizer_.pass_token();
        auto expression = parse_expression();
        if (tokenizer_.has_tokens() && tokenizer_.weak_next_token().is(token::token_type::rp)) {
            tokenizer_.pass_token();
            return expression;
        }
    }

    return nullptr;
}

std::shared_ptr<tree::tree_node> expression_tree::parse_relational_operation() {
    auto left = parse_terminal();
    if (tokenizer_.has_tokens()) {
        auto operation = std::make_shared<tree::tree_node>(tokenizer_.next_token());
        auto right = parse_terminal();
        operation->left  = left;
        operation->right = right;
        return operation;
    }

    return nullptr;
}

std::shared_ptr<tree::tree_node> expression_tree::parse_terminal() {
    if (tokenizer_.has_tokens()) {
        auto token = tokenizer_.next_token();
        if (token.is(token::token_type::field)) {
            return std::make_shared<tree::tree_node>(token);
        }
    }

    return nullptr;
}

} // tree

} // booleval