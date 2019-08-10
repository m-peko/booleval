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

namespace booleval {

Token::Token() noexcept
    : type_(Type::UNKNOWN)
{}

Token::Token(Type type) noexcept
    : type_(type)
{}

bool Token::operator==(Token const& other) const noexcept {
    return type_ == other.type_;
}

void Token::type(Type const type) noexcept {
    type_ = type;
}

Token::Type Token::type() const noexcept {
    return type_;
}

bool Token::is(Type const type) const noexcept {
    return type_ == type;
}

bool Token::is_not(Type const type) const noexcept {
    return type_ != type;
}

bool Token::is_one_of(Type const type1, Type const type2) const noexcept {
    return is(type1) || is(type2);
}

std::unordered_map<std::string, Token::Type> Token::type_expressions() noexcept {
    static std::unordered_map<std::string, Type> type_expressions = {
        { "and",     Type::AND },
        { "&&" ,     Type::AND },
        { "or" ,     Type::OR },
        { "||" ,     Type::OR },
        { "neq",     Type::NEQ },
        { "!="  ,    Type::NEQ },
        { "greater", Type::GT },
        { ">",       Type::GT },
        { "less",    Type::LT },
        { "<",       Type::LT },
        { "(",       Type::LP },
        { ")",       Type::RP }
    };
    return type_expressions;
}

} // booleval