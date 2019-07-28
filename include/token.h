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

#ifndef BOOLEVAL_TOKEN_H
#define BOOLEVAL_TOKEN_H

#include "base_token.h"

namespace Booleval {

template <typename T = uint8_t>
class Token : public BaseToken {
public:
    Token() noexcept;
    Token(TokenType type) noexcept;
    Token(TokenType type, T const& value) noexcept;

    bool operator==(Token<T> const& other) const;

    void value(T const& value) noexcept;
    T const& value() const noexcept;

private:
    T value_;
};

template <typename T>
Token<T>::Token() noexcept
    : BaseToken(),
      value_()
{}

template <typename T>
Token<T>::Token(TokenType type) noexcept
    : BaseToken(type),
      value_()
{}

template <typename T>
Token<T>::Token(TokenType type, T const& value) noexcept
    : BaseToken(type),
      value_(value)
{}

template <typename T>
bool Token<T>::operator==(Token<T> const& other) const {
    return BaseToken::operator==(other) &&
           value_ == other.value_;
}

template <typename T>
void Token<T>::value(T const& value) noexcept {
    value_ = value;
}

template <typename T>
T const& Token<T>::value() const noexcept {
    return value_;
}

} // Booleval

#endif // BOOLEVAL_TOKEN_H