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

#include <booleval/token/tokenizer.h>
#include <booleval/utils/string_utils.h>

namespace booleval {

namespace token {

tokenizer::tokenizer(std::string_view expression) noexcept
    : expression_(expression),
      current_token_index_(0)
{}

void tokenizer::expression(std::string_view expression) noexcept {
    expression_ = expression;
}

std::string_view tokenizer::expression() const noexcept {
    return expression_;
}

bool tokenizer::has_tokens() const noexcept {
    return current_token_index_ < tokens_.size();
}

void tokenizer::pass_token() noexcept {
    current_token_index_++;
}

token const& tokenizer::next_token() {
    return tokens_.at(current_token_index_++);
}

token const& tokenizer::weak_next_token() {
    return tokens_.at(current_token_index_);
}

void tokenizer::tokenize() {
    tokens_.clear();
    reset();

    std::vector<char> single_char_symbols;
    for (auto const& p : symbol_expressions) {
        if (1 == p.first.size()) {
            single_char_symbols.push_back(p.first.front());
        }
    }

    auto raw_tokens = utils::split<
        utils::split_options::include_whitespace |
        utils::split_options::include_delimiters
    >(expression_, utils::join(std::begin(single_char_symbols), std::end(single_char_symbols)));

    for (auto const& token : raw_tokens) {
        auto type = map_to_token_type(token);
        if (token_type::field == type) {
            if (!tokens_.empty() && tokens_.back().is(token_type::field)) {
                tokens_.emplace_back(token_type::eq, map_to_token_value(token_type::eq));
            }
        }

        tokens_.emplace_back(type, token);
    }
}

void tokenizer::reset() noexcept {
    current_token_index_ = 0;
}

} // token

} // booleval
