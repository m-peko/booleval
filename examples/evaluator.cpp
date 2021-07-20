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
#include <iostream>
#include <booleval/evaluator.hpp>

class bar
{
public:
    bar( std::string value_1, unsigned value_2 ) noexcept
    : value_1_{ std::move( value_1 ) }
    , value_2_{            value_2   }
    {}

    std::string const & value_1() const noexcept { return value_1_; }
    unsigned            value_2() const noexcept { return value_2_; }

private:
    std::string value_1_{};
    unsigned    value_2_{ 0 };
};

int main()
{
    bar x{ "foo", 1 };
    bar y{ "bar", 2 };

    booleval::evaluator evaluator
    {
        {
            booleval::make_field( "field_1", &bar::value_1 ),
            booleval::make_field( "field_2", &bar::value_2 )
        }
    };

    if ( !evaluator.expression( "(field_1 foo and field_2 1) or (field_1 qux and field_2 2)" ) )
    {
        std::cerr << "Expression not valid!" << std::endl;
    }

    if ( evaluator.is_activated() )
    {
        std::cout << std::boolalpha << evaluator.evaluate( x ).success << std::endl;
        std::cout << std::boolalpha << evaluator.evaluate( y ).success << std::endl;
    }
    else
    {
        std::cerr << "Evaluator is not activated!" << std::endl;
    }

    return 0;
}