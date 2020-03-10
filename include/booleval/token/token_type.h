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

#ifndef BOOLEVAL_TOKEN_TYPE_H
#define BOOLEVAL_TOKEN_TYPE_H

#include <cstdint>
#include <string_view>
#include <unordered_map>

namespace booleval {

namespace token {

/**
 * enum class token_type
 *
 * Represents a token type. Supported types are logical operators,
 * relational operators, parentheses and field.
 */
enum class token_type : uint8_t {
    unknown = 0,
    field   = 1,

    // Logical operators
    logical_and = 2,
    logical_or  = 3,

    // Relational operators
    eq  = 4,
    neq = 5,
    gt  = 6,
    lt  = 7,

    // Parentheses
    lp = 8,
    rp = 9
};

inline std::unordered_map<
    std::string_view,
    token_type
> const keyword_expressions = {
    { "and",     token_type::logical_and },
    { "or",      token_type::logical_or  },
    { "eq",      token_type::eq  },
    { "neq",     token_type::neq },
    { "greater", token_type::gt  },
    { "less",    token_type::lt  }
};

inline std::unordered_map<
    std::string_view,
    token_type
> const symbol_expressions = {
    { "&&", token_type::logical_and },
    { "||", token_type::logical_or  },
    { "==", token_type::eq  },
    { "!=", token_type::neq },
    { ">",  token_type::gt  },
    { "<",  token_type::lt  },
    { "(",  token_type::lp  },
    { ")",  token_type::rp  }
};

/**
 * Maps token value to token type.
 *
 * @param value Token value
 *
 * @return Token type
 */
token_type map_to_token_type(std::string_view const value);

/**
 * Maps token type to token value.
 *
 * @param type Token type
 *
 * @return Token value
 */
std::string_view map_to_token_value(token_type const type);

} // token

} // booleval

#endif // BOOLEVAL_TOKEN_TYPE_H