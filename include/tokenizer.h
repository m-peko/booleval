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

#ifndef BOOLEVAL_TOKENIZER_H
#define BOOLEVAL_TOKENIZER_H

#include <string>
#include <vector>
#include <unordered_map>

#include "token.h"

namespace Booleval {

class Tokenizer {
public:
    Tokenizer() noexcept;
    Tokenizer(std::string const& expression) noexcept;

    Tokenizer& operator++();
    Tokenizer operator++(int);

    void expression(std::string const& expression) noexcept;
    std::string const& expression() const noexcept;

    std::shared_ptr<BaseToken> const& token() const;
    bool const has_token() const noexcept;

    void tokenize();

private:
    std::vector<std::string> extract_raw_tokens() const;
    std::string build_regex_pattern() const;

private:
    std::string expression_;
    std::vector<std::shared_ptr<BaseToken>> tokens_;
    size_t current_index_;

    static std::unordered_map<std::string, TokenType> const token_expressions_;
};

} // Booleval

#endif // BOOLEVAL_TOKENIZER_H