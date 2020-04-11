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

#include <gtest/gtest.h>
#include <booleval/evaluator.h>

class EvaluatorTest : public testing::Test {
public:
    template <typename T>
    class obj {
    public:
        obj() : value_a_{} {}
        obj(T value) : value_a_{ value } {}
        T value_a() const noexcept { return value_a_; }

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
};

TEST_F(EvaluatorTest, DefaultConstructor) {
    booleval::evaluator evaluator;
    EXPECT_FALSE(evaluator.is_activated());
}

TEST_F(EvaluatorTest, EmptyExpression) {
    booleval::evaluator evaluator;
    EXPECT_TRUE(evaluator.expression(""));
    EXPECT_FALSE(evaluator.is_activated());
    EXPECT_TRUE(evaluator.evaluate(obj<uint8_t>()));
}

TEST_F(EvaluatorTest, MissingClosingParenthesesExpression) {
    booleval::evaluator evaluator;
    EXPECT_FALSE(evaluator.expression("(field_a foo or field_b bar"));
    EXPECT_FALSE(evaluator.is_activated());
    EXPECT_TRUE(evaluator.evaluate(obj<uint8_t>()));
}

TEST_F(EvaluatorTest, MultipleFieldsExpression) {
    booleval::evaluator evaluator;
    EXPECT_FALSE(evaluator.expression("field_a foo field_b"));
    EXPECT_FALSE(evaluator.is_activated());
    EXPECT_TRUE(evaluator.evaluate(obj<uint8_t>()));
}

TEST_F(EvaluatorTest, EqualToOperator) {
    obj<std::string> foo{ "foo" };
    obj<std::string> bar{ "bar" };

    booleval::evaluator evaluator;
    evaluator.map({
        { "field_a", &obj<std::string>::value_a }
    });

    EXPECT_TRUE(evaluator.expression("field_a foo"));
    EXPECT_TRUE(evaluator.is_activated());
    EXPECT_TRUE(evaluator.evaluate(foo));
    EXPECT_FALSE(evaluator.evaluate(bar));
}

TEST_F(EvaluatorTest, NotEqualToOperator) {
    obj<std::string> foo{ "foo" };
    obj<std::string> bar{ "bar" };

    booleval::evaluator evaluator;
    evaluator.map({
        { "field_a", &obj<std::string>::value_a }
    });

    EXPECT_TRUE(evaluator.expression("field_a neq foo"));
    EXPECT_TRUE(evaluator.is_activated());
    EXPECT_FALSE(evaluator.evaluate(foo));
    EXPECT_TRUE(evaluator.evaluate(bar));
}

TEST_F(EvaluatorTest, GreaterThanOperator) {
    obj<float> foo{ 1.24F };
    obj<float> bar{ 1.22F };

    booleval::evaluator evaluator;
    evaluator.map({
        { "field_a", &obj<float>::value_a }
    });

    EXPECT_TRUE(evaluator.expression("field_a gt 1.23"));
    EXPECT_TRUE(evaluator.is_activated());
    EXPECT_TRUE(evaluator.evaluate(foo));
    EXPECT_FALSE(evaluator.evaluate(bar));
}

TEST_F(EvaluatorTest, LessThanOperator) {
    obj<uint8_t> foo{ 1 };
    obj<uint8_t> bar{ 3 };

    booleval::evaluator evaluator;
    evaluator.map({
        { "field_a", &obj<uint8_t>::value_a }
    });

    EXPECT_TRUE(evaluator.expression("field_a lt 2"));
    EXPECT_TRUE(evaluator.is_activated());
    EXPECT_TRUE(evaluator.evaluate(foo));
    EXPECT_FALSE(evaluator.evaluate(bar));
}

TEST_F(EvaluatorTest, GreaterThanOrEqualToOperator) {
    obj<double> foo{ 1.234567 };
    obj<double> bar{ 2.345678 };
    obj<double> baz{ 0.123456 };

    booleval::evaluator evaluator;
    evaluator.map({
        { "field_a", &obj<double>::value_a }
    });

    EXPECT_TRUE(evaluator.expression("field_a geq 1.234567"));
    EXPECT_TRUE(evaluator.is_activated());
    EXPECT_TRUE(evaluator.evaluate(foo));
    EXPECT_TRUE(evaluator.evaluate(bar));
    EXPECT_FALSE(evaluator.evaluate(baz));
}

TEST_F(EvaluatorTest, LessThanOrEqualToOperator) {
    obj<double> foo{ 1.234567 };
    obj<double> bar{ 0.123456 };
    obj<double> baz{ 2.345678 };

    booleval::evaluator evaluator;
    evaluator.map({
        { "field_a", &obj<double>::value_a }
    });

    EXPECT_TRUE(evaluator.expression("field_a leq 1.234567"));
    EXPECT_TRUE(evaluator.is_activated());
    EXPECT_TRUE(evaluator.evaluate(foo));
    EXPECT_TRUE(evaluator.evaluate(bar));
    EXPECT_FALSE(evaluator.evaluate(baz));
}

TEST_F(EvaluatorTest, AndOperator) {
    multi_obj<std::string, uint8_t> foo{ "one", 1 };
    multi_obj<std::string, uint8_t> bar{ "two", 2 };
    multi_obj<std::string, uint8_t> baz{ "two", 1 };

    booleval::evaluator evaluator;
    evaluator.map({
        { "field_a", &multi_obj<std::string, uint8_t>::value_a },
        { "field_b", &multi_obj<std::string, uint8_t>::value_b }
    });

    EXPECT_TRUE(evaluator.expression("field_a one and field_b 1"));
    EXPECT_TRUE(evaluator.is_activated());
    EXPECT_TRUE(evaluator.evaluate(foo));
    EXPECT_FALSE(evaluator.evaluate(bar));
    EXPECT_FALSE(evaluator.evaluate(baz));
}

TEST_F(EvaluatorTest, OrOperator) {
    multi_obj<std::string, uint8_t> foo{ "one", 1 };
    multi_obj<std::string, uint8_t> bar{ "one", 2 };
    multi_obj<std::string, uint8_t> baz{ "two", 1 };
    multi_obj<std::string, uint8_t> qux{ "two", 2 };

    booleval::evaluator evaluator;
    evaluator.map({
        { "field_a", &multi_obj<std::string, uint8_t>::value_a },
        { "field_b", &multi_obj<std::string, uint8_t>::value_b }
    });

    EXPECT_TRUE(evaluator.expression("field_a one or field_b 1"));
    EXPECT_TRUE(evaluator.is_activated());
    EXPECT_TRUE(evaluator.evaluate(foo));
    EXPECT_TRUE(evaluator.evaluate(bar));
    EXPECT_TRUE(evaluator.evaluate(baz));
    EXPECT_FALSE(evaluator.evaluate(qux));
}

TEST_F(EvaluatorTest, MultipleOperators) {
    multi_obj<std::string, uint8_t> foo{ "one", 1 };
    multi_obj<std::string, uint8_t> bar{ "one", 2 };
    multi_obj<std::string, uint8_t> baz{ "two", 1 };
    multi_obj<std::string, uint8_t> qux{ "two", 2 };

    booleval::evaluator evaluator;
    evaluator.map({
        { "field_a", &multi_obj<std::string, uint8_t>::value_a },
        { "field_b", &multi_obj<std::string, uint8_t>::value_b }
    });

    EXPECT_TRUE(evaluator.expression("(field_a one and field_b 1) or (field_a two and field_b 2)"));
    EXPECT_TRUE(evaluator.is_activated());
    EXPECT_TRUE(evaluator.evaluate(foo));
    EXPECT_FALSE(evaluator.evaluate(bar));
    EXPECT_FALSE(evaluator.evaluate(baz));
    EXPECT_TRUE(evaluator.evaluate(qux));
}
