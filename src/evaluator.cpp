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

#include <booleval/evaluator.h>
#include <booleval/token/base_token.h>
#include <booleval/token/field_token.h>

namespace booleval {

Evaluator::Evaluator() noexcept
    : is_activated_(false)
{}

bool Evaluator::is_activated() const noexcept {
    return is_activated_;
}

bool Evaluator::build_expression_tree(std::string const& expression) {
    is_activated_ = false;

    tokenizer_.expression(expression);
    tokenizer_.tokenize();

    if (expression.empty()) {
        return true;
    }

    root = parse_expression();
    if (nullptr != root) {
        is_activated_ = true;
    }

    return is_activated_;
}

bool Evaluator::evaluate(std::map<std::string, std::string> const& fields) {
    if (is_activated_) {
        result_visitor_.fields(fields);
        return result_visitor_.visit(*root);
    } else {
        return true;
    }
}

std::shared_ptr<node::TreeNode> Evaluator::parse_expression() {
    auto and_node = parse_and();
    while (tokenizer_.has_token() && tokenizer_.token()->is(token::TokenType::OR)) {
        auto or_node = std::make_shared<node::TreeNode>(token::TokenType::OR);
        tokenizer_++;

        auto and_node_right = parse_and();
        or_node->left = and_node;
        or_node->right = and_node_right;
        and_node = or_node;
    }
    return and_node;
}

std::shared_ptr<node::TreeNode> Evaluator::parse_and() {
    auto condition1 = parse_condition();
    while (tokenizer_.has_token() && tokenizer_.token()->is(token::TokenType::AND)) {
        tokenizer_++;
        auto condition2 = parse_condition();
        auto condition = std::make_shared<node::TreeNode>(token::TokenType::AND);
        condition->left = condition1;
        condition->right = condition2;
        condition1 = condition;
    }
    return condition1;
}

std::shared_ptr<node::TreeNode> Evaluator::parse_condition() {
    if (tokenizer_.has_token() && tokenizer_.token()->is(token::TokenType::LP)) {
        tokenizer_++;
        auto expression = parse_expression();
        if (tokenizer_.has_token() && tokenizer_.token()->is(token::TokenType::RP)) {
            tokenizer_++;
            return expression;
        }

        return nullptr;
    }

    auto terminal_left = parse_terminal();
    if (tokenizer_.has_token()) {
        auto condition = std::make_shared<node::TreeNode>(tokenizer_.token());
        tokenizer_++;

        auto terminal_right = parse_terminal();
        condition->left = terminal_left;
        condition->right = terminal_right;
        return condition;
    }

    return nullptr;
}

std::shared_ptr<node::TreeNode> Evaluator::parse_terminal() {
    if (tokenizer_.has_token()) {
        auto token = tokenizer_.token();
        tokenizer_++;

        if (token->is(token::TokenType::FIELD)) {
            auto field_token = std::dynamic_pointer_cast<token::FieldToken<>>(token);
            return std::make_shared<node::TreeNode>(field_token);
        }
    }

    return nullptr;
}

} // booleval
