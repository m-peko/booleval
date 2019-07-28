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

#include <regex>
#include <algorithm>
#include <boost/algorithm/string.hpp>

#include "tokenizer.h"

namespace Booleval {

std::unordered_map<std::string, TokenType> const Tokenizer::token_expressions_ = {
    { "and",     TokenType::AND },
    { "&&" ,     TokenType::AND },
    { "or" ,     TokenType::OR },
    { "||" ,     TokenType::OR },
    { "not",     TokenType::NEQ },
    { "!"  ,     TokenType::NEQ },
    { "greater", TokenType::GT },
    { ">",       TokenType::GT },
    { "less",    TokenType::LT },
    { "<",       TokenType::LT },
    { "(",       TokenType::LP },
    { ")",       TokenType::RP },
    { "field_a", TokenType::FIELD_A },
    { "field_b", TokenType::FIELD_B },
    { "field_c", TokenType::FIELD_C }
};

Tokenizer::Tokenizer() noexcept
    : current_index_(0)
{}

Tokenizer::Tokenizer(std::string const& expression) noexcept
    : expression_(expression),
      current_index_(0)
{}

Tokenizer& Tokenizer::operator++() {
    current_index_++;
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

std::shared_ptr<BaseToken> const& Tokenizer::token() const {
    return tokens_.at(current_index_);
}

bool const Tokenizer::has_token() const noexcept {
    return current_index_ < tokens_.size();
}

void Tokenizer::tokenize() {
    current_index_ = 0;
    tokens_.clear();

    auto raw_tokens = extract_raw_tokens();

    for (auto const& raw_token : raw_tokens) {
        auto search = token_expressions_.find(raw_token);
        if (search != token_expressions_.end()) {
            tokens_.emplace_back(new BaseToken(search->second));
        } else {
            tokens_.emplace_back(new Token<std::string>(TokenType::VALUE, raw_token));
        }
    }
}

std::vector<std::string> Tokenizer::extract_raw_tokens() const {
    auto is_empty = [](std::string const& s) {
        return s.empty() || s.find_first_not_of(' ') == std::string::npos;
    };

    std::vector<std::string> raw_tokens;

    std::regex re(build_regex_pattern(),
                  std::regex_constants::ECMAScript |
                  std::regex_constants::icase);
    std::string tmp = expression_;
    std::string token;

    auto i = tmp.begin();
    while (i != tmp.end()) {
        std::match_results<std::string::iterator> m;
        if (std::regex_match(i, tmp.end(), m, re)) {
            if (!is_empty(token)) {
                boost::trim(token);
                raw_tokens.push_back(token);
                token.clear();
            }
            raw_tokens.push_back(std::string(m[1].first, m[1].second));
            i += raw_tokens.back().size();
        } else {
            token += *i++;
        }
    }

    if (!is_empty(token)) {
        boost::trim(token);
        raw_tokens.push_back(token);
    }

    return raw_tokens;
}

std::string Tokenizer::build_regex_pattern() const {
    std::string pattern = "\(";

    bool is_first = true;
    for (auto const& expr : token_expressions_) {
        if (is_first) {
            is_first = false;
        } else {
            pattern += "|";
        }

        for (auto const& c : expr.first) {
            if (!isalnum(c)) {
                // escape each non-alphanumeric character
                pattern += "\\";
            }
            pattern += c;
        }
    }

    pattern += ").*";
    return pattern;
}

} // Booleval
