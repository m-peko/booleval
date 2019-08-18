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

#ifndef BOOLEVAL_FIELD_TOKEN_H
#define BOOLEVAL_FIELD_TOKEN_H

#include <string>
#include <booleval/token/base_token.h>
#include <booleval/token/token_type.h>

namespace booleval {

namespace token {

template <typename FieldType = std::string>
class FieldToken : public BaseToken {
public:
    FieldToken() noexcept;
    FieldToken(FieldToken&& other) = default;
    FieldToken(FieldToken const& other) = default;
    FieldToken(FieldType const& field) noexcept;

    FieldToken& operator=(FieldToken&& other) = default;
    FieldToken& operator=(FieldToken const& other) = default;
    bool operator==(FieldToken const& other) const noexcept;

    virtual ~FieldToken() = default;

    virtual void type(TokenType const type) noexcept;
    using BaseToken::type;

    void field(FieldType const& field) noexcept;
    FieldType const& field() const noexcept;

private:
    template <typename T>
    void ignore(T&&) const noexcept;

private:
    FieldType field_;
};

template <typename FieldType>
FieldToken<FieldType>::FieldToken() noexcept
    : BaseToken(TokenType::FIELD),
      field_{}
{}

template <typename FieldType>
FieldToken<FieldType>::FieldToken(FieldType const& field) noexcept
    : BaseToken(TokenType::FIELD),
      field_(field)
{}

template <typename FieldType>
void FieldToken<FieldType>::type(TokenType const type) noexcept {
    ignore(type);
    BaseToken::type(TokenType::FIELD);
}

template <typename FieldType>
void FieldToken<FieldType>::field(FieldType const& field) noexcept {
    field_ = field;
}

template <typename FieldType>
FieldType const& FieldToken<FieldType>::field() const noexcept {
    return field_;
}

template <typename FieldType>
template <typename T>
void FieldToken<FieldType>::ignore(T&&) const noexcept
{}

} // token

} // booleval

#endif // BOOLEVAL_FIELD_TOKEN_H