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

#include <algorithm>
#include <booleval/token/tokenizer.h>
#include <booleval/token/field_token.h>
#include <booleval/utils/regex_pattern.h>
#include <booleval/utils/string_utils.h>

namespace booleval {

namespace token {

Tokenizer::Tokenizer() noexcept
    : current_token_index_(0)
{}

Tokenizer::Tokenizer(std::string const& expression) noexcept
    : expression_(expression),
      current_token_index_(0)
{}

Tokenizer& Tokenizer::operator++() {
    current_token_index_++;
    return *this;
}

Tokenizer Tokenizer::operator++(int) {
    Tokenizer Tokenizer(*this);
    ++*this;
    return Tokenizer;
}

void Tokenizer::expression(std::string const& expression) noexcept {
    expression_ = expression;
}

std::string const& Tokenizer::expression() const noexcept {
    return expression_;
}

bool Tokenizer::has_token() const noexcept {
    return current_token_index_ < tokens_.size();
}

std::shared_ptr<BaseToken> const& Tokenizer::token() const {
    return tokens_.at(current_token_index_);
}

void Tokenizer::tokenize() {
    reset();

    auto raw_tokens = utils::split(expression_, build_regex_pattern());
    auto token_type_expressions = BaseToken::type_expressions();

    for (auto const& token : raw_tokens) {
        auto search_result = token_type_expressions.find(token);
        if (search_result != token_type_expressions.end()) {
            tokens_.emplace_back(new BaseToken(search_result->second));
        } else {
            if (!tokens_.empty() && tokens_.back()->is(TokenType::FIELD)) {
                tokens_.emplace_back(new BaseToken(TokenType::EQ));
            }
            tokens_.emplace_back(new FieldToken<>(token));
        }
    }
}

void Tokenizer::reset() noexcept {
    tokens_.clear();
    current_token_index_ = 0;
}

std::string Tokenizer::build_regex_pattern() const {
    auto token_type_expressions = BaseToken::type_expressions();

    utils::RegexPattern pattern;
    pattern.match_whitespaces();

    for (auto const& expression : token_type_expressions) {
        pattern.logical_or()
               .match_word(expression.first);
    }

    return pattern.to_string();
}

} // token

} // booleval
