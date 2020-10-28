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

#include <gtest/gtest.h>
#include <booleval/tree/tree_node.hpp>
#include <booleval/token/token_type.hpp>
#include <booleval/tree/result_visitor.hpp>

class ResultVisitorTest : public testing::Test {
public:
    template <typename T>
    class obj {
    public:
        obj() : value_a_{} {}
        obj(T value) : value_a_{ value } {}
        T value_a() const noexcept { return value_a_; }
        T value_a_valid(bool& is_valid) const noexcept { is_valid = true; return value_a_; }
        T value_a_notvalid(bool& is_valid) const noexcept { is_valid = false; return value_a_; }

    private:
        T value_a_;
    };

    template <typename T, typename U>
    class multi_obj {
    public:
        multi_obj() : value_a_{}, value_b_{} {}
        multi_obj(T value_a, U value_b) : value_a_{ value_a }, value_b_{ value_b } {}
        T value_a() const noexcept { return value_a_; }
        U value_b() const noexcept { return value_b_; }

    private:
        T value_a_;
        U value_b_;
    };

    std::shared_ptr<booleval::tree::tree_node>
    make_tree_node(booleval::token::token_type const type) {
        return std::make_shared<booleval::tree::tree_node>(type);
    }

    std::shared_ptr<booleval::tree::tree_node>
    make_tree_node(booleval::token::token_type const type, std::string_view const value) {
        booleval::token::token t(type, value);
        return std::make_shared<booleval::tree::tree_node>(t);
    }
};

TEST_F(ResultVisitorTest, VisitAndTreeNode) {
    using namespace booleval;

    multi_obj<uint8_t, uint8_t> foo{ 1, 2 };
    multi_obj<uint8_t, uint8_t> bar{ 2, 3 };
    multi_obj<uint8_t, uint8_t> baz{ 3, 4 };

    tree::result_visitor<> visitor;
    visitor.fields({
        { "field_a", &multi_obj<uint8_t, uint8_t>::value_a },
        { "field_b", &multi_obj<uint8_t, uint8_t>::value_b }
    });

    auto and_op = make_tree_node(token::token_type::logical_and);

    auto left  = make_tree_node(token::token_type::field, "field_a");
    auto op    = make_tree_node(token::token_type::eq);
    auto right = make_tree_node(token::token_type::field, "1");

    op->left  = left;
    op->right = right;

    and_op->left = op;

    left  = make_tree_node(token::token_type::field, "field_b");
    op    = make_tree_node(token::token_type::eq);
    right = make_tree_node(token::token_type::field, "2");

    op->left  = left;
    op->right = right;

    and_op->right = op;

    EXPECT_TRUE(visitor.visit(*and_op, foo));
    EXPECT_FALSE(visitor.visit(*and_op, bar));
    EXPECT_FALSE(visitor.visit(*and_op, baz));
}

TEST_F(ResultVisitorTest, VisitOrTreeNode) {
    using namespace booleval;

    obj<uint8_t> foo{ 1 };
    obj<uint8_t> bar{ 2 };
    obj<uint8_t> baz{ 3 };

    tree::result_visitor<> visitor;
    visitor.fields({
        { "field_a", &obj<uint8_t>::value_a }
    });

    auto or_op = make_tree_node(token::token_type::logical_or);

    auto left  = make_tree_node(token::token_type::field, "field_a");
    auto op    = make_tree_node(token::token_type::eq);
    auto right = make_tree_node(token::token_type::field, "1");

    op->left  = left;
    op->right = right;

    or_op->left = op;

    left  = make_tree_node(token::token_type::field, "field_a");
    op    = make_tree_node(token::token_type::eq);
    right = make_tree_node(token::token_type::field, "2");

    op->left  = left;
    op->right = right;

    or_op->right = op;

    EXPECT_TRUE(visitor.visit(*or_op, foo));
    EXPECT_TRUE(visitor.visit(*or_op, bar));
    EXPECT_FALSE(visitor.visit(*or_op, baz));
}

TEST_F(ResultVisitorTest, VisitEqualToTreeNode) {
    using namespace booleval;

    obj<uint8_t> foo{ 1 };
    obj<uint8_t> bar{ 2 };

    tree::result_visitor<> visitor;
    visitor.fields({
        { "field_a", &obj<uint8_t>::value_a }
    });

    auto left  = make_tree_node(token::token_type::field, "field_a");
    auto op    = make_tree_node(token::token_type::eq);
    auto right = make_tree_node(token::token_type::field, "1");

    op->left  = left;
    op->right = right;

    EXPECT_TRUE(visitor.visit(*op, foo));
    EXPECT_FALSE(visitor.visit(*op, bar));
}

TEST_F(ResultVisitorTest, VisitNotEqualToTreeNode) {
    using namespace booleval;

    obj<uint8_t> foo{ 1 };
    obj<uint8_t> bar{ 2 };

    tree::result_visitor<> visitor;
    visitor.fields({
        { "field_a", &obj<uint8_t>::value_a }
    });

    auto left  = make_tree_node(token::token_type::field, "field_a");
    auto op    = make_tree_node(token::token_type::neq);
    auto right = make_tree_node(token::token_type::field, "1");

    op->left  = left;
    op->right = right;

    EXPECT_FALSE(visitor.visit(*op, foo));
    EXPECT_TRUE(visitor.visit(*op, bar));
}

TEST_F(ResultVisitorTest, VisitGreaterThanTreeNode) {
    using namespace booleval;

    obj<uint8_t> foo{ 0 };
    obj<uint8_t> bar{ 1 };
    obj<uint8_t> baz{ 2 };

    tree::result_visitor<> visitor;
    visitor.fields({
        { "field_a", &obj<uint8_t>::value_a }
    });

    auto left  = make_tree_node(token::token_type::field, "field_a");
    auto op    = make_tree_node(token::token_type::gt);
    auto right = make_tree_node(token::token_type::field, "1");

    op->left  = left;
    op->right = right;

    EXPECT_FALSE(visitor.visit(*op, foo));
    EXPECT_FALSE(visitor.visit(*op, bar));
    EXPECT_TRUE(visitor.visit(*op, baz));
}

TEST_F(ResultVisitorTest, VisitLessThanTreeNode) {
    using namespace booleval;

    obj<uint8_t> foo{ 0 };
    obj<uint8_t> bar{ 1 };
    obj<uint8_t> baz{ 2 };

    tree::result_visitor<> visitor;
    visitor.fields({
        { "field_a", &obj<uint8_t>::value_a }
    });

    auto left  = make_tree_node(token::token_type::field, "field_a");
    auto op    = make_tree_node(token::token_type::lt);
    auto right = make_tree_node(token::token_type::field, "1");

    op->left  = left;
    op->right = right;

    EXPECT_TRUE(visitor.visit(*op, foo));
    EXPECT_FALSE(visitor.visit(*op, bar));
    EXPECT_FALSE(visitor.visit(*op, baz));
}

TEST_F(ResultVisitorTest, VisitGreaterThanOrEqualTreeNode) {
    using namespace booleval;

    obj<uint8_t> foo{ 0 };
    obj<uint8_t> bar{ 1 };
    obj<uint8_t> baz{ 2 };

    tree::result_visitor<> visitor;
    visitor.fields({
        { "field_a", &obj<uint8_t>::value_a }
    });

    auto left  = make_tree_node(token::token_type::field, "field_a");
    auto op    = make_tree_node(token::token_type::geq);
    auto right = make_tree_node(token::token_type::field, "1");

    op->left  = left;
    op->right = right;

    EXPECT_FALSE(visitor.visit(*op, foo));
    EXPECT_TRUE(visitor.visit(*op, bar));
    EXPECT_TRUE(visitor.visit(*op, baz));
}

TEST_F(ResultVisitorTest, VisitLessThanOrEqualTreeNode) {
    using namespace booleval;

    obj<uint8_t> foo{ 0 };
    obj<uint8_t> bar{ 1 };
    obj<uint8_t> baz{ 2 };

    tree::result_visitor<> visitor;
    visitor.fields({
        { "field_a", &obj<uint8_t>::value_a }
    });

    auto left  = make_tree_node(token::token_type::field, "field_a");
    auto op    = make_tree_node(token::token_type::leq);
    auto right = make_tree_node(token::token_type::field, "1");

    op->left  = left;
    op->right = right;

    EXPECT_TRUE(visitor.visit(*op, foo));
    EXPECT_TRUE(visitor.visit(*op, bar));
    EXPECT_FALSE(visitor.visit(*op, baz));
}

TEST_F(ResultVisitorTest, VisitInvalidTreeNode) {
    using namespace booleval;

    obj<uint8_t> foo{ 1 };

    tree::result_visitor<utils::any_mem_fn_bool> visitor;
    visitor.fields({
        { "field_a_valid", &obj<uint8_t>::value_a_valid },
        { "field_a_notvalid", &obj<uint8_t>::value_a_notvalid }
    });

    auto left = make_tree_node(token::token_type::field, "field_a_valid");
    auto op = make_tree_node(token::token_type::eq);
    auto right = make_tree_node(token::token_type::field, "1");

    op->left = left;
    op->right = right;

    EXPECT_TRUE(visitor.visit(*op, foo));

    left = make_tree_node(token::token_type::field, "field_a_notvalid");
    op->left = left;

    EXPECT_FALSE(visitor.visit(*op, foo));
}

TEST_F(ResultVisitorTest, VisitNonExistantTreeNode) {
    using namespace booleval;

    obj<uint8_t> foo{ 1 };

    tree::result_visitor<> visitor;
    visitor.fields({
        { "field_a", &obj<uint8_t>::value_a }
    });

    auto left = make_tree_node(token::token_type::field, "field_not_exist");
    auto op = make_tree_node(token::token_type::eq);
    auto right = make_tree_node(token::token_type::field, "1");

    op->left = left;
    op->right = right;

    try {
        [[maybe_unused]] auto result = visitor.visit(*op, foo);
        FAIL() << "Expected booleval::field_not_found";
    } catch (field_not_found const& ex) {
        EXPECT_EQ(ex.what(), std::string("Field 'field_not_exist' not found"));
    }
}
