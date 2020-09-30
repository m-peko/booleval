/*
 * Copyright (c) 2020, Marin Peko
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

#ifndef BOOLEVAL_EXPRESSION_TREE_H
#define BOOLEVAL_EXPRESSION_TREE_H

#include <memory>
#include <string_view>
#include <booleval/tree/tree_node.hpp>
#include <booleval/token/tokenizer.hpp>

namespace booleval {

namespace tree {

/**
 * class expression_tree
 *
 * Represents a class for building an expression tree by using a recursive
 * descent parser method.
 */
class expression_tree {
public:
    expression_tree();
    expression_tree(expression_tree&& rhs) = default;
    expression_tree(expression_tree const& rhs) = default;

    expression_tree& operator=(expression_tree&& rhs) = default;
    expression_tree& operator=(expression_tree const& rhs) = default;

    ~expression_tree() = default;

    /**
     * Gets the root tree node.
     *
     * @return Root tree node
     */
    [[nodiscard]] std::shared_ptr<tree::tree_node> root() noexcept;

    /**
     * Builds the expression tree.
     *
     * @param expression Expression to build the tree for
     *
     * @return True if the expression tree is built successfully, otherwise false
     */
    [[nodiscard]] bool build(std::string_view expression, char quote_char);

private:
    /**
     * Parses root expression by trying first to parse logical operation OR.
     *
     * @return Root tree node for the current part of the expression
     */
    [[nodiscard]] std::shared_ptr<tree::tree_node> parse_expression();

    /**
     * Parses logical operation AND.
     *
     * @return Root tree node for the parsed logical operation
     */
    [[nodiscard]] std::shared_ptr<tree::tree_node> parse_and_operation();

    /**
     * Parses new expression within parentheses.
     *
     * @return Root tree node for the parsed expression within parentheses
     */
    [[nodiscard]] std::shared_ptr<tree::tree_node> parse_parentheses();

    /**
     * Parses relational operation (EQ, NEQ, GT, LT, GEQ and LEQ).
     *
     * @return Root tree node for the parsed relational operation
     */
    [[nodiscard]] std::shared_ptr<tree::tree_node> parse_relational_operation();

    /**
     * Parses terminal.
     *
     * @return Leaf node
     */
    [[nodiscard]] std::shared_ptr<tree::tree_node> parse_terminal();

private:
    token::tokenizer tokenizer_;
    std::shared_ptr<tree::tree_node> root_;
};

} // tree

} // booleval

#endif // BOOLEVAL_EXPRESSION_TREE_H