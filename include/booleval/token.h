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

#include <string>
#include <cstdint>
#include <unordered_map>

namespace booleval {

class Token {
public:
    enum class Type : uint8_t {
        UNKNOWN = 0,
        VALUE   = 1,

        // Logical operators
        AND = 2,
        OR  = 3,

        // Relational operators
        NEQ = 4,
        GT  = 5,
        LT  = 6,

        // Parentheses
        LP = 7,
        RP = 8
    };

public:
    Token() noexcept;
    Token(Token&& other) = default;
    Token(Token const& other) = default;
    Token(Type type) noexcept;

    Token& operator=(Token&& other) = default;
    Token& operator=(Token const& other) = default;
    bool operator==(Token const& other) const noexcept;

    virtual ~Token() = default;

    void type(Type const type) noexcept;
    Type type() const noexcept;

    bool is(Type const type) const noexcept;
    bool is_not(Type const type) const noexcept;
    bool is_one_of(Type const type1, Type const type2) const noexcept;

    template <typename... Types>
    bool is_one_of(Type const type1, Type const type2, Types const ... types) const noexcept;

    static std::unordered_map<std::string, Type> type_expressions() noexcept;

private:
    Type type_;
};

template <typename... Types>
bool Token::is_one_of(Type const type1, Type const type2, Types const ... types) const noexcept {
    return is(type1) || is_one_of(type2, types...);
}

} // booleval

#endif // BOOLEVAL_TOKEN_H