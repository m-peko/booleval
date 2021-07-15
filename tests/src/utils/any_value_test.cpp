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

#include <string>
#include <string_view>
#include <gtest/gtest.h>
#include <booleval/utils/any_value.hpp>

TEST( AnyValueTest, ArithmeticValue )
{
    {
        booleval::utils::any_value value{ 1 };
        ASSERT_EQ( value, "1" );
        ASSERT_EQ( value, 1u  );
    }
    {
        booleval::utils::any_value value{ 1.234567f };
        ASSERT_EQ( value, "1.234567" );
        ASSERT_EQ( value, 1.234567f  );
    }
}

TEST( AnyValueTest, StringValue )
{
    {
        booleval::utils::any_value value{ "abc" };
        ASSERT_EQ( value, "abc" );
    }
    {
        booleval::utils::any_value value{ std::string{ "abc" } };
        ASSERT_EQ( value, "abc" );
    }
    {
        booleval::utils::any_value value{ std::string_view{ "abc" } };
        ASSERT_EQ( value, "abc" );
    }
}

TEST( AnyValueTest, Comparisons )
{
    {
        booleval::utils::any_value value{ 1 };

        ASSERT_TRUE( value == 1U );
        ASSERT_TRUE( value != 2U );

        ASSERT_TRUE( value > 0U );
        ASSERT_TRUE( value < 2U );

        ASSERT_TRUE( value >= 0U );
        ASSERT_TRUE( value >= 1U );

        ASSERT_TRUE( value <= 1U );
        ASSERT_TRUE( value <= 2U );
    }
    {
        booleval::utils::any_value value{ 1.234567 };

        ASSERT_TRUE( value == 1.234567F );
        ASSERT_TRUE( value != 7.654321F );

        ASSERT_TRUE( value > 0.123456F );
        ASSERT_TRUE( value < 2.345678F );

        ASSERT_TRUE( value >= 0.123456F );
        ASSERT_TRUE( value >= 1.234567F );

        ASSERT_TRUE( value <= 1.234567F );
        ASSERT_TRUE( value <= 2.345678F );
    }
}
