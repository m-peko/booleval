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

#include <optional>
#include <string_view>
#include <type_traits>
#include <booleval/token/token_type.h>
#include <booleval/utils/string_utils.h>

namespace booleval {

namespace token {

/**
 * class token
 *
 * Represents all tokens ('and', 'or', 'eq', ...).
 */
class token {
public:
    token() noexcept;
    token(token&& rhs) = default;
    token(token const& rhs) = default;
    token(token_type const type) noexcept;
    token(std::string_view const value) noexcept;
    token(token_type const type, std::string_view const value) noexcept;

    token& operator=(token&& rhs) = default;
    token& operator=(token const& rhs) = default;
    bool operator==(token const& rhs) const noexcept;

    ~token() = default;

    /**
     * Sets the token type.
     *
     * @param type Token type
     */
    void type(token_type const type) noexcept;

    /**
     * Gets the token type.
     *
     * @return Token type
     */
    token_type type() const noexcept;

    /**
     * Sets the token value.
     *
     * @param value Token value
     */
    void value(std::string_view const value) noexcept;

    /**
     * Gets the token value.
     *
     * @return Token value
     */
    [[nodiscard]] std::string_view value() const noexcept;

    /**
     * Gets the token value as an arithmetic value.
     * If value cannot be parsed, std::nullopt is returned.
     *
     * @return Optional token value
     */
    template <typename T,
              typename = std::enable_if_t<std::is_arithmetic_v<T>>>
    [[nodiscard]] std::optional<T> value() const noexcept;

    /**
     * Checks whether the token is of the specified type.
     *
     * @return True if the token is of the specified type, false otherwise
     */
    [[nodiscard]] bool is(token_type const type) const noexcept;

    /**
     * Checks whether the token is not of the specified type.
     *
     * @return True if the token is not of the specified type, false otherwise
     */
    [[nodiscard]] bool is_not(token_type const type) const noexcept;

    /**
     * Checks whether the token is of the first or second specified type.
     *
     * @return True if the token is of the first or second specified type, false otherwise
     */
    [[nodiscard]] bool is_one_of(token_type const first, token_type const second) const noexcept;

    /**
     * Checks whether the token is one of the multiple specified types.
     *
     * @return True if the token is one of the multiple specified types, false otherwise
     */
    template <typename... TokenType>
    [[nodiscard]] bool is_one_of(token_type const first, token_type const second, TokenType const ... nth) const noexcept;

private:
    token_type type_;
    std::string_view value_;
};

template <typename T, typename>
std::optional<T> token::value() const noexcept {
    return utils::from_chars<T>(value_);
}

template <typename... TokenType>
bool token::is_one_of(token_type const first, token_type const second, TokenType const ... nth) const noexcept {
    return is(first) || is_one_of(second, nth...);
}

} // token

} // booleval

#endif // BOOLEVAL_TOKEN_H