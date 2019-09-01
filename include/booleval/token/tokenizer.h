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

#include <memory>
#include <string>
#include <vector>
#include <booleval/token/base_token.h>

namespace booleval {

namespace token {

/**
 * class Tokenizer
 *
 * This class is used for tokenizing the expression, i.e.
 * transforming the expression in a form of a string to the
 * collection of tokens.
 */
class Tokenizer {
public:
    Tokenizer() = default;
    Tokenizer(Tokenizer&& other) = default;
    Tokenizer(Tokenizer const& other) = default;

    /**
     * Constructor from an expression creates an
     * object which will tokenize specified expression.
     *
     * @param expression String expression
     */
    Tokenizer(std::string const& expression) noexcept;

    Tokenizer& operator=(Tokenizer&& other) = default;
    Tokenizer& operator=(Tokenizer const& other) = default;

    /**
     * Prefix incrementation of the current index
     * in the collection of tokens.
     *
     * @return Tokenizer
     */
    Tokenizer& operator++();

    /**
     * Postfix incrementation of the current index
     * in the collection of tokens.
     *
     * @param Unused parameter
     *
     * @return Tokenizer
     */
    Tokenizer operator++(int);

    ~Tokenizer() = default;

    /**
     * Setter for the expression that is supposed to
     * be tokenized.
     *
     * @param expression Expression to be tokenized
     */
    void expression(std::string const& expression) noexcept;

    /**
     * Getter for the expression that is supposed to
     * be tokenized.
     *
     * @return Expression to be tokenized
     */
    std::string const& expression() const noexcept;

    /**
     * Checks whether the next token in collection exists or not.
     *
     * @return True if the next token exists, otherwise false
     */
    bool has_token() const noexcept;

    /**
     * Getter for the current token.
     *
     * @return Current token
     */
    std::shared_ptr<BaseToken> const& token() const;

    /**
     * Tokenizes given expression and transforms it
     * into the collection of tokens.
     */
    void tokenize();

private:
    /**
     * Resets the current tokenizer state, i.e. clears
     * the collection of tokens and sets the current
     * index to zero.
     */
    void reset() noexcept;

    /**
     * Builds regex pattern from the standard token
     * expressions.
     *
     * @return Regex pattern
     */
    std::string build_regex_pattern() const;

private:
    std::string expression_;
    size_t current_token_index_;
    std::vector<std::shared_ptr<BaseToken>> tokens_;
};

} // token

} // booleval

#endif // BOOLEVAL_TOKENIZER_H