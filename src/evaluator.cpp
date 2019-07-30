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

#include <booleval/token.h>
#include <booleval/evaluator.h>

namespace Booleval {

Evaluator::Evaluator() noexcept
    : is_activated_(false)
{}

bool Evaluator::is_activated() const {
    return is_activated_;
}

bool Evaluator::build_expression_tree(std::string const& expression) {
    tokenizer_.expression(expression);
    tokenizer_.tokenize();

    is_activated_ = false;

    if (expression.empty()) {
        return true;
    }

    root = parse_expression();
    if (nullptr != root) {
        is_activated_ = true;
    }

    return is_activated_;
}

bool Evaluator::evaluate(Object const& obj) {
    if (is_activated_) {
        return evaluate_recursive(root, obj);
    } else {
        return true;
    }
}

std::shared_ptr<TreeNode> Evaluator::parse_expression() {
    auto and_condition1 = parse_and();
    while (tokenizer_.has_token() && tokenizer_.token()->is(TokenType::OR)) {
        auto or_condition = std::make_shared<TreeNode>(TokenType::OR);
        tokenizer_++;

        auto and_condition2 = parse_and();
        or_condition->left = and_condition1;
        or_condition->right = and_condition2;
        and_condition1 = or_condition;
    }
    return and_condition1;
}

std::shared_ptr<TreeNode> Evaluator::parse_and() {
    auto condition1 = parse_condition();
    while (tokenizer_.has_token() && tokenizer_.token()->is(TokenType::AND)) {
        tokenizer_++;
        auto condition2 = parse_condition();
        auto condition = std::make_shared<TreeNode>(TokenType::AND);
        condition->left = condition1;
        condition->right = condition2;
        condition1 = condition;
    }
    return condition1;
}

std::shared_ptr<TreeNode> Evaluator::parse_condition() {
    if (tokenizer_.has_token() && tokenizer_.token()->is(TokenType::LP)) {
        tokenizer_++;
        auto expression = parse_expression();
        if (tokenizer_.has_token() && tokenizer_.token()->is(TokenType::RP)) {
            tokenizer_++;
            return expression;
        }

        return nullptr;
    }

    auto terminal_left = parse_terminal();
    if (tokenizer_.has_token()) {
        auto condition = std::make_shared<TreeNode>(tokenizer_.token()->type());

        auto terminal_right = parse_terminal();
        condition->left = terminal_left;
        condition->right = terminal_right;
        return condition;
    }

    return nullptr;
}

std::shared_ptr<TreeNode> Evaluator::parse_terminal() {
    if (tokenizer_.has_token()) {
        auto token = tokenizer_.token();
        tokenizer_++;

        if (token->is_field_type()) {
            return std::make_shared<TreeNode>(token->type());
        } else if (token->is(TokenType::VALUE)) {
            auto value_token = std::dynamic_pointer_cast<Token<std::string>>(token);
            return std::make_shared<TreeNode>(value_token->type(), value_token->value());
        }
    }

    return nullptr;
}

bool Evaluator::evaluate_recursive(std::shared_ptr<TreeNode>& node, Object const& obj) const {
    auto left = node->left;
    auto right = node->right;

    if (right->token->is(TokenType::VALUE)) {
        auto value_token = std::dynamic_pointer_cast<Token<std::string>>(right->token);

        if (left->token->is(TokenType::FIELD_A)) {
            return obj.field_A() == value_token->value();
        } else if (left->token->is(TokenType::FIELD_B)) {
            return obj.field_B() == value_token->value();
        } else if (left->token->is(TokenType::FIELD_C)) {
            return obj.field_C() == value_token->value();
        }
    }

    auto left_result = evaluate_recursive(left, obj);
    auto right_result = evaluate_recursive(right, obj);

    if (node->token->is(TokenType::AND)) {
        return left_result && right_result;
    } else if (node->token->is(TokenType::OR)) {
        return left_result || right_result;
    } else if (node->token->is(TokenType::NEQ)) {
        return left_result != right_result;
    } else if (node->token->is(TokenType::GT)) {
        return left_result > right_result;
    } else if (node->token->is(TokenType::LT)) {
        return left_result > right_result;
    } else {
        return true;
    }
}

} // Booleval
