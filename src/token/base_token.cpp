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

#include <booleval/token/base_token.h>

namespace booleval {

namespace token {

BaseToken::BaseToken() noexcept
    : type_(TokenType::UNKNOWN)
{}

BaseToken::BaseToken(TokenType const type) noexcept
    : type_(type)
{}

bool BaseToken::operator==(BaseToken const& other) const noexcept {
    return type_ == other.type_;
}

void BaseToken::type(TokenType const type) noexcept {
    type_ = type;
}

TokenType BaseToken::type() const noexcept {
    return type_;
}

bool BaseToken::is(TokenType const type) const noexcept {
    return type_ == type;
}

bool BaseToken::is_not(TokenType const type) const noexcept {
    return type_ != type;
}

bool BaseToken::is_one_of(TokenType const type1, TokenType const type2) const noexcept {
    return is(type1) || is(type2);
}

std::unordered_map<std::string, TokenType> BaseToken::type_expressions() noexcept {
    static std::unordered_map<std::string, TokenType> type_expressions = {
        { "and",     TokenType::AND },
        { "&&" ,     TokenType::AND },
        { "or" ,     TokenType::OR },
        { "||" ,     TokenType::OR },
        { "neq",     TokenType::NEQ },
        { "!="  ,    TokenType::NEQ },
        { "greater", TokenType::GT },
        { ">",       TokenType::GT },
        { "less",    TokenType::LT },
        { "<",       TokenType::LT },
        { "(",       TokenType::LP },
        { ")",       TokenType::RP }
    };
    return type_expressions;
}

} // token

} // booleval