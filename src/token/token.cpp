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

#include <booleval/token/token.h>
#include <booleval/token/token_type.h>

namespace booleval {

namespace token {

token::token() noexcept
    : type_(token_type::unknown)
{}

token::token(token_type const type) noexcept
    : type_(type),
      value_(map_to_token_value(type))
{}

token::token(std::string_view const value) noexcept
    : type_(map_to_token_type(value)),
      value_(value)
{}

token::token(token_type const type, std::string_view const value) noexcept
    : type_(type),
      value_(value)
{}

bool token::operator==(token const& rhs) const noexcept {
    return type_  == rhs.type_ &&
           value_ == rhs.value_;
}

void token::type(token_type const type) noexcept {
    if (type != type_) {
        type_ = type;
        value_ = map_to_token_value(type);
    }
}

token_type token::type() const noexcept {
    return type_;
}

void token::value(std::string_view value) noexcept {
    type_ = map_to_token_type(value);
    value_ = value;
}

std::string_view token::value() const noexcept {
    return value_;
}

bool token::is(token_type const type) const noexcept {
    return type_ == type;
}

bool token::is_not(token_type const type) const noexcept {
    return type_ != type;
}

bool token::is_one_of(token_type const first, token_type const second) const noexcept {
    return is(first) || is(second);
}

} // token

} // booleval