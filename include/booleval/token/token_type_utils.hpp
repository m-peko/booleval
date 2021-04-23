/*
 * Copyright (c) 2021, Marin Peko
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

#ifndef BOOLEVAL_TOKEN_TYPE_UTILS_HPP
#define BOOLEVAL_TOKEN_TYPE_UTILS_HPP

#include <array>
#include <cassert>
#include <utility>
#include <string_view>

#include <booleval/token/token_type.hpp>
#include <booleval/utils/algorithm.hpp>

namespace booleval::token
{

namespace internal
{

    using token_type_pair = std::pair< std::string_view, token_type >;

    constexpr inline std::array keywords
    {
        token_type_pair{ "and", token_type::logical_and },
        token_type_pair{ "AND", token_type::logical_and },
        token_type_pair{ "or" , token_type::logical_or  },
        token_type_pair{ "OR" , token_type::logical_or  },
        token_type_pair{ "eq" , token_type::eq          },
        token_type_pair{ "EQ" , token_type::eq          },
        token_type_pair{ "neq", token_type::neq         },
        token_type_pair{ "NEQ", token_type::neq         },
        token_type_pair{ "gt" , token_type::gt          },
        token_type_pair{ "GT" , token_type::gt          },
        token_type_pair{ "lt" , token_type::lt          },
        token_type_pair{ "LT" , token_type::lt          },
        token_type_pair{ "geq", token_type::geq         },
        token_type_pair{ "GEQ", token_type::geq         },
        token_type_pair{ "leq", token_type::leq         },
        token_type_pair{ "LEQ", token_type::leq         }
    };

    constexpr inline std::array symbols
    {
        token_type_pair{ "&&", token_type::logical_and },
        token_type_pair{ "||", token_type::logical_or  },
        token_type_pair{ "==", token_type::eq          },
        token_type_pair{ "!=", token_type::neq         },
        token_type_pair{ ">" , token_type::gt          },
        token_type_pair{ "<" , token_type::lt          },
        token_type_pair{ ">=", token_type::geq         },
        token_type_pair{ "<=", token_type::leq         },
        token_type_pair{ "(" , token_type::lp          },
        token_type_pair{ ")" , token_type::rp          }
    };

} // namespace internal

/**
 * Gets parentheses symbols out of all symbols.
 *
 * @return Parentheses symbols
 */
[[ nodiscard ]] constexpr auto get_parentheses_symbols() noexcept
{
    auto is_parenthesis
    {
        []( auto && symbol ) noexcept
        {
            return symbol.second == token_type::lp ||
                   symbol.second == token_type::rp;
        }
    };

    constexpr auto count
    {
        utils::count_if
        (
            std::cbegin( internal::symbols ),
            std::cend  ( internal::symbols ),
            is_parenthesis
        )
    };

    auto i{ 0u };
    std::array< char, count > parentheses_symbols{};

    for ( auto && symbol : internal::symbols )
    {
        if ( is_parenthesis( symbol ) )
        {
            assert( std::size( symbol.first ) == 1 && "Parenthesis symbol must have only 1 character." );
            assert( i < count                      && "Index out of scope."                            );
            parentheses_symbols[ i++ ] = symbol.first.front();
        }
    }

    return parentheses_symbols;
}

/**
 * Maps token value to token type.
 *
 * @param value Token value
 *
 * @return Token type
 */
constexpr token_type to_token_type( std::string_view const value ) noexcept
{
    auto find_matching
    {
        [ value ]( auto const & collection ) noexcept
        {
            return utils::find_if
            (
                std::cbegin( collection ),
                std::cend  ( collection ),
                [ value ]( auto && item )
                {
                    return item.first == value;
                }
            );
        }
    };

    if
    (
        auto const keyword{ find_matching( internal::keywords ) };
        keyword != std::cend( internal::keywords )
    )
    {
        return keyword->second;
    }

    if
    (
        auto const symbol{ find_matching( internal::symbols ) };
        symbol != std::cend( internal::symbols )
    )
    {
        return symbol->second;
    }

    return token_type::field;
}

/**
 * Maps token type to token keyword.
 *
 * @param type Token type
 *
 * @return Token keyword
 */
[[ nodiscard ]] constexpr std::string_view to_token_keyword( token_type const type ) noexcept
{
    auto const keyword
    {
        utils::find_if
        (
            std::cbegin( internal::keywords ),
            std::cend  ( internal::keywords ),
            [ type ]( auto && item ) noexcept
            {
                return item.second == type;
            }
        )
    };

    if ( keyword != std::cend( internal::keywords ) )
    {
        return keyword->first;
    }

    return {};
}

} // namespace booleval::token

#endif // BOOLEVAL_TOKEN_TYPE_HPP