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
#include <booleval/token/base_token.h>
#include <booleval/token/token_type.h>

namespace booleval {

namespace node {

/**
 * struct TreeNode
 *
 * This struct represents the tree node containing
 * references to left and right child nodes as well
 * as the token that the node represents in the actual
 * expression tree.
 */
struct TreeNode {
    std::shared_ptr<token::BaseToken> token;
    std::shared_ptr<TreeNode> left;
    std::shared_ptr<TreeNode> right;

    TreeNode() = default;
    TreeNode(TreeNode&& other) = default;
    TreeNode(TreeNode const& other) = default;

    /**
     * Constructor from token type creates a tree
     * node representing the token of specified type
     * in the expression tree.
     *
     * @param type Token type
     */
    TreeNode(token::TokenType const type);

    /**
     * Constructor from token creates a tree node
     * representing the specified token in the
     * expression tree.
     *
     * @param token Base token
     */
    TreeNode(std::shared_ptr<token::BaseToken> const& token);

    TreeNode& operator=(TreeNode&& other) = default;
    TreeNode& operator=(TreeNode const& other) = default;

    ~TreeNode() = default;
};

} // node

} // booleval

#endif // BOOLEVAL_TREE_NODE_H