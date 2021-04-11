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

#ifndef BOOLEVAL_TOKEN_TYPE_HPP
#define BOOLEVAL_TOKEN_TYPE_HPP

#include <array>
#include <cstdint>
#include <utility>
#include <string_view>
#include <booleval/utils/algo_utils.hpp>

namespace booleval::token {

/**
 * enum class token_type
 *
 * Represents a token type. Supported types are logical operators,
 * relational operators, parentheses and field.
 */
enum class [[nodiscard]] token_type : uint8_t {
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
    geq = 8,
    leq = 9,

    // Parentheses
    lp = 10,
    rp = 11
};

constexpr std::size_t count_of_keyword_expressions{ 16 };
constexpr std::array<
    std::pair<std::string_view, token_type>,
    count_of_keyword_expressions
> keyword_expressions = {{
    { "and", token_type::logical_and },
    { "AND", token_type::logical_and },
    { "or",  token_type::logical_or  },
    { "OR",  token_type::logical_or  },
    { "eq",  token_type::eq  },
    { "EQ",  token_type::eq  },
    { "neq", token_type::neq },
    { "NEQ", token_type::neq },
    { "gt",  token_type::gt  },
    { "GT",  token_type::gt  },
    { "lt",  token_type::lt  },
    { "LT",  token_type::lt  },
    { "geq", token_type::geq },
    { "GEQ", token_type::geq },
    { "leq", token_type::leq },
    { "LEQ", token_type::leq }
}};

constexpr std::size_t count_of_symbol_expressions{ 10 };
constexpr std::array<
    std::pair<std::string_view, token_type>,
    count_of_symbol_expressions
> symbol_expressions = {{
    { "&&", token_type::logical_and },
    { "||", token_type::logical_or  },
    { "==", token_type::eq  },
    { "!=", token_type::neq },
    { ">",  token_type::gt  },
    { "<",  token_type::lt  },
    { ">=", token_type::geq },
    { "<=", token_type::leq },
    { "(",  token_type::lp  },
    { ")",  token_type::rp  }
}};

/**
 * Filters parenthesis symbol expressions from all symbol expressions.
 *
 * @return Parenthesis symbol expressions
 */
constexpr auto parenthesis_symbol_expressions() {
    constexpr auto count = utils::count_if(
        std::begin(symbol_expressions),
        std::end(symbol_expressions),
        [](auto&& p) {
            return token_type::lp == p.second || token_type::rp == p.second;
        }
    );

    std::size_t i{ 0 };
    std::array<char, count> parenthesis_symbols{};
    for (auto const& p : symbol_expressions) {
        if (token_type::lp == p.second || token_type::rp == p.second) {
            parenthesis_symbols[i++] = p.first.front();
        }
    }

    return parenthesis_symbols;
}

/**
 * Maps token value to token type.
 *
 * @param value Token value
 *
 * @return Token type
 */
constexpr token_type map_to_token_type(std::string_view const value) {
    auto keyword_search = utils::find_if(
        std::begin(keyword_expressions),
        std::end(keyword_expressions),
        [value](auto&& p) {
            return p.first == value;
        }
    );

    if (std::end(keyword_expressions) != keyword_search) {
        return keyword_search->second;
    }

    auto symbol_search = utils::find_if(
        std::begin(symbol_expressions),
        std::end(symbol_expressions),
        [value](auto&& p) {
            return p.first == value;
        }
    );

    if (std::end(symbol_expressions) != symbol_search) {
        return symbol_search->second;
    }

    return token_type::field;
}

/**
 * Maps token type to token value.
 *
 * @param type Token type
 *
 * @return Token value
 */
[[nodiscard]] constexpr std::string_view map_to_token_value(token_type const type) {
    auto keyword_search = utils::find_if(
        std::begin(keyword_expressions),
        std::end(keyword_expressions),
        [type](auto&& p) {
            return p.second == type;
        }
    );

    if (std::end(keyword_expressions) != keyword_search) {
        return keyword_search->first;
    }

    return {};
}

} // namespace booleval::token

#endif // BOOLEVAL_TOKEN_TYPE_HPP