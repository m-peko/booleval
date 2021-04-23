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

#include <cstdint>

namespace booleval::token
{

/**
 * enum class token_type
 *
 * Represents a token type. Supported types are logical operators, relational operators, parentheses and field.
 */
enum class [[ nodiscard ]] token_type : std::uint8_t
{
    // Unknown token type
    unknown,

    // Field key token type
    field,

    // Logical AND operator token type
    logical_and,

    // Logical OR operator token type
    logical_or,

    // 'Equal to' relational operator token type
    eq,

    // 'Not equal to' relational operator token type
    neq,

    // 'Greater than' relational operator token type
    gt,

    // 'Less than' relational operator token type
    lt,

    // 'Greater than or equal to' relational operator token type
    geq,

    // 'Less than or equal to' relational operator token type
    leq,

    // Left parenthesis token type
    lp,

    // Right parenthesis token type
    rp
};

} // namespace booleval::token

#endif // BOOLEVAL_TOKEN_TYPE_HPP