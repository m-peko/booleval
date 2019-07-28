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

#include <memory>
#include <gtest/gtest.h>

#include "token.h"
#include "tree_node.h"

class TreeNodeTest : public testing::Test {};

TEST_F(TreeNodeTest, DefaultConstructor) {
    using namespace Booleval;

    TreeNode node;
    EXPECT_EQ(node.token, nullptr);
    EXPECT_EQ(node.left, nullptr);
    EXPECT_EQ(node.right, nullptr);
}

TEST_F(TreeNodeTest, ConstructorFromType) {
    using namespace Booleval;

    TreeNode node(TokenType::AND);
    EXPECT_EQ(node.token->type(), TokenType::AND);
    EXPECT_EQ(node.left, nullptr);
    EXPECT_EQ(node.right, nullptr);
}

TEST_F(TreeNodeTest, ConstructorFromTypeAndValue) {
    using namespace Booleval;

    TreeNode node(TokenType::AND, static_cast<uint8_t>(1));

    auto token = std::dynamic_pointer_cast<Token<>>(node.token);
    EXPECT_EQ(token->type(), TokenType::AND);
    EXPECT_EQ(token->value(), 1U);
    EXPECT_EQ(node.left, nullptr);
    EXPECT_EQ(node.right, nullptr);
}
