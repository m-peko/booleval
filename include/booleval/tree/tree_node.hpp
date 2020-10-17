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

#ifndef BOOLEVAL_TREE_NODE_H
#define BOOLEVAL_TREE_NODE_H

#include <memory>
#include <booleval/token/token.hpp>
#include <booleval/token/token_type.hpp>

namespace booleval {

namespace tree {

/**
 * struct tree_node
 *
 * Represents the tree node containing references to left and right child nodes
 * as well as the token that the node represents in the actual expression tree.
 */
struct tree_node {
    token::token token{ token::token_type::unknown };
    std::shared_ptr<tree_node> left;
    std::shared_ptr<tree_node> right;

    constexpr tree_node() = default;

    tree_node(tree_node&& rhs) = default;
    tree_node(tree_node const& rhs) = default;

    constexpr tree_node(token::token_type const type)
        : token(type)
    {}

    constexpr tree_node(token::token const& token)
        : token(token)
    {}

    tree_node& operator=(tree_node&& rhs) = default;
    tree_node& operator=(tree_node const& rhs) = default;

    ~tree_node() = default;
};

} // tree

} // booleval

#endif // BOOLEVAL_TREE_NODE_H