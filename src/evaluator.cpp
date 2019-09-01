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
    auto left_and_operation = parse_and_operation();

    while (tokenizer_.has_token() && tokenizer_.token()->is(token::TokenType::OR)) {
        auto or_operation = std::make_shared<node::TreeNode>(token::TokenType::OR);
        tokenizer_++;

        auto right_and_operation = parse_and_operation();
        or_operation->left = left_and_operation;
        or_operation->right = right_and_operation;
        left_and_operation = or_operation;
    }

    return left_and_operation;
}

std::shared_ptr<node::TreeNode> Evaluator::parse_and_operation() {
    auto left_operation = parse_operation();

    while (tokenizer_.has_token() && tokenizer_.token()->is(token::TokenType::AND)) {
        auto and_operation = std::make_shared<node::TreeNode>(token::TokenType::AND);
        tokenizer_++;

        auto right_operation = parse_operation();
        and_operation->left = left_operation;
        and_operation->right = right_operation;
        left_operation = and_operation;
    }

    return left_operation;
}

std::shared_ptr<node::TreeNode> Evaluator::parse_operation() {
    if (tokenizer_.has_token() && tokenizer_.token()->is(token::TokenType::LP)) {
        tokenizer_++;

        auto expression = parse_expression();
        if (tokenizer_.has_token() && tokenizer_.token()->is(token::TokenType::RP)) {
            tokenizer_++;
            return expression;
        }

        return nullptr;
    }

    auto left_terminal = parse_terminal();
    if (tokenizer_.has_token()) {
        auto operation = std::make_shared<node::TreeNode>(tokenizer_.token());
        tokenizer_++;

        auto right_terminal = parse_terminal();
        operation->left = left_terminal;
        operation->right = right_terminal;
        return operation;
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
