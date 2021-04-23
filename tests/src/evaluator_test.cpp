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
#include <booleval/evaluator.hpp>

namespace
{

    template< typename T >
    class foo
    {
    public:
        foo()             : value_{}        {}
        foo( T && value ) : value_{ value } {}

        void value( T && value ) { value_ = value; }

        T value() const noexcept { return value_; }

    private:
        T value_{};
    };

    template< typename T, typename U >
    class bar
    {
    public:
        bar( T && value_1, U && value_2 )
        : value_1_{ value_1 }
        , value_2_{ value_2 }
        {}

        void value_1( T && value ) { value_1_ = value; }
        void value_2( U && value ) { value_2_ = value; }

        T value_1() const noexcept { return value_1_; }
        U value_2() const noexcept { return value_2_; }

    private:
        T value_1_{};
        U value_2_{};
    };

} // namespace

TEST( EvaluatorTest, DefaultConstructor )
{
    booleval::evaluator<> evaluator;

    ASSERT_FALSE( evaluator.is_activated() );
}

TEST( EvaluatorTest, EmptyExpression )
{
    booleval::evaluator<> evaluator;

    ASSERT_TRUE ( evaluator.expression("")                            );
    ASSERT_FALSE( evaluator.is_activated()                            );
    ASSERT_FALSE( evaluator.evaluate( foo< std::uint8_t >{} ).success );
}

TEST( EvaluatorTest, MissingParenthesesExpression )
{
    booleval::evaluator<> evaluator;

    ASSERT_FALSE( evaluator.expression( "(field_x foo or field_y bar" ) );
    ASSERT_FALSE( evaluator.is_activated()                              );
    ASSERT_FALSE( evaluator.evaluate( foo< std::uint8_t >{} ).success   );
}

TEST( EvaluatorTest, MultipleFieldsInRowExpression )
{
    booleval::evaluator<> evaluator;

    ASSERT_FALSE( evaluator.expression( "field_x foo field_y" )       );
    ASSERT_FALSE( evaluator.is_activated()                            );
    ASSERT_FALSE( evaluator.evaluate( foo< std::uint8_t >{} ).success );
}

TEST( EvaluatorTest, EqualToOperator )
{
    foo< std::string > x{ "foo" };
    foo< std::string > y{ "bar" };

    booleval::evaluator<> evaluator
    {
        {
            { "field", &foo< std::string >::value }
        }
    };

    {
        ASSERT_TRUE ( evaluator.expression( "field foo" ) );
        ASSERT_TRUE ( evaluator.is_activated()            );
        ASSERT_TRUE ( evaluator.evaluate( x ).success     );
        ASSERT_FALSE( evaluator.evaluate( y ).success     );
    }
    {
        ASSERT_TRUE ( evaluator.expression( "field eq foo" ) );
        ASSERT_TRUE ( evaluator.is_activated()               );
        ASSERT_TRUE ( evaluator.evaluate( x ).success        );
        ASSERT_FALSE( evaluator.evaluate( y ).success        );
    }
    {
        ASSERT_TRUE ( evaluator.expression( "field == foo" ) );
        ASSERT_TRUE ( evaluator.is_activated()               );
        ASSERT_TRUE ( evaluator.evaluate( x ).success        );
        ASSERT_FALSE( evaluator.evaluate( y ).success        );
    }
    {
        x.value( "foo foo" );

        ASSERT_TRUE ( evaluator.expression( "field == \"foo foo\"" ) );
        ASSERT_TRUE ( evaluator.is_activated()                       );
        ASSERT_TRUE ( evaluator.evaluate( x ).success                );
        ASSERT_FALSE( evaluator.evaluate( y ).success                );
    }
}

TEST( EvaluatorTest, NotEqualToOperator )
{
    foo< std::string > x{ "foo" };
    foo< std::string > y{ "bar" };

    booleval::evaluator<> evaluator
    {
        {
            { "field", &foo< std::string >::value }
        }
    };

    {
        ASSERT_TRUE ( evaluator.expression( "field neq foo" ) );
        ASSERT_TRUE ( evaluator.is_activated()                );
        ASSERT_FALSE( evaluator.evaluate( x ).success         );
        ASSERT_TRUE ( evaluator.evaluate( y ).success         );
    }
    {
        ASSERT_TRUE ( evaluator.expression( "field != foo" ) );
        ASSERT_TRUE ( evaluator.is_activated()               );
        ASSERT_FALSE( evaluator.evaluate( x ).success        );
        ASSERT_TRUE ( evaluator.evaluate( y ).success        );
    }
    {
        x.value( "foo foo" );

        ASSERT_TRUE ( evaluator.expression( "field != \"foo foo\"" ) );
        ASSERT_TRUE ( evaluator.is_activated()                       );
        ASSERT_FALSE( evaluator.evaluate( x ).success                );
        ASSERT_TRUE ( evaluator.evaluate( y ).success                );
    }
}

TEST( EvaluatorTest, GreaterThanOperator )
{
    foo< float > x{ 1.22 };
    foo< float > y{ 1.24 };

    foo< std::string > m{ "1000" };
    foo< std::string > n{ "50"   };

    booleval::evaluator<> evaluator_digits
    {
        {
            { "field", &foo< float >::value }
        }
    };

    booleval::evaluator<> evaluator_strings
    {
        {
            { "field", &foo< std::string >::value }
        }
    };

    {
        ASSERT_TRUE ( evaluator_digits.expression( "field gt 1.23" ) );
        ASSERT_TRUE ( evaluator_digits.is_activated()                );
        ASSERT_FALSE( evaluator_digits.evaluate( x ).success         );
        ASSERT_TRUE ( evaluator_digits.evaluate( y ).success         );
    }
    {
        ASSERT_TRUE ( evaluator_digits.expression( "field > 1.23" ) );
        ASSERT_TRUE ( evaluator_digits.is_activated()               );
        ASSERT_FALSE( evaluator_digits.evaluate( x ).success        );
        ASSERT_TRUE ( evaluator_digits.evaluate( y ).success        );
    }
    {
        ASSERT_TRUE ( evaluator_strings.expression( "field gt \"200\"" ) );
        ASSERT_TRUE ( evaluator_strings.is_activated()                   );
        ASSERT_FALSE( evaluator_strings.evaluate( m ).success            );
        ASSERT_TRUE ( evaluator_strings.evaluate( n ).success            );
    }
    {
        ASSERT_TRUE ( evaluator_strings.expression( "field > \"200\"" ) );
        ASSERT_TRUE ( evaluator_strings.is_activated()                  );
        ASSERT_FALSE( evaluator_strings.evaluate( m ).success           );
        ASSERT_TRUE ( evaluator_strings.evaluate( n ).success           );
    }
}

TEST( EvaluatorTest, GreaterThanOrEqualToOperator )
{
    foo< double > x{ 1.234567 };
    foo< double > y{ 2.345678 };
    foo< double > z{ 0.123456 };

    booleval::evaluator<> evaluator
    {
        {
            { "field", &foo< double >::value }
        }
    };

    {
        ASSERT_TRUE ( evaluator.expression( "field geq 1.234567" ) );
        ASSERT_TRUE ( evaluator.is_activated()                     );
        ASSERT_TRUE ( evaluator.evaluate( x ).success              );
        ASSERT_TRUE ( evaluator.evaluate( y ).success              );
        ASSERT_FALSE( evaluator.evaluate( z ).success              );
    }
    {
        ASSERT_TRUE ( evaluator.expression( "field >= 1.234567" ) );
        ASSERT_TRUE ( evaluator.is_activated()                    );
        ASSERT_TRUE ( evaluator.evaluate( x ).success             );
        ASSERT_TRUE ( evaluator.evaluate( y ).success             );
        ASSERT_FALSE( evaluator.evaluate( z ).success             );
    }
}

TEST( EvaluatorTest, LessThanOperator )
{
    foo< unsigned > x{ 1 };
    foo< unsigned > y{ 3 };

    foo< std::string > m{ "1000" };
    foo< std::string > n{ "50"   };

    booleval::evaluator<> evaluator_digits
    {
        {
            { "field", &foo< unsigned >::value }
        }
    };

    booleval::evaluator<> evaluator_strings
    {
        {
            { "field", &foo< std::string >::value }
        }
    };

    {
        ASSERT_TRUE ( evaluator_digits.expression( "field lt 2" ) );
        ASSERT_TRUE ( evaluator_digits.is_activated()             );
        ASSERT_TRUE ( evaluator_digits.evaluate( x ).success      );
        ASSERT_FALSE( evaluator_digits.evaluate( y ).success      );
    }
    {
        ASSERT_TRUE ( evaluator_digits.expression( "field < 2" ) );
        ASSERT_TRUE ( evaluator_digits.is_activated()            );
        ASSERT_TRUE ( evaluator_digits.evaluate( x ).success     );
        ASSERT_FALSE( evaluator_digits.evaluate( y ).success     );
    }
    {
        ASSERT_TRUE ( evaluator_strings.expression( "field lt \"200\"" ) );
        ASSERT_TRUE ( evaluator_strings.is_activated()                   );
        ASSERT_TRUE ( evaluator_strings.evaluate( m ).success            );
        ASSERT_FALSE( evaluator_strings.evaluate( n ).success            );
    }
    {
        ASSERT_TRUE ( evaluator_strings.expression( "field < \"200\"" ) );
        ASSERT_TRUE ( evaluator_strings.is_activated()                  );
        ASSERT_TRUE ( evaluator_strings.evaluate( m ).success           );
        ASSERT_FALSE( evaluator_strings.evaluate( n ).success           );
    }
}

TEST( EvaluatorTest, LessThanOrEqualToOperator )
{
    foo< double > x{ 1.234567 };
    foo< double > y{ 2.345678 };
    foo< double > z{ 0.123456 };

    booleval::evaluator<> evaluator
    {
        {
            { "field", &foo< double >::value }
        }
    };

    {
        ASSERT_TRUE ( evaluator.expression( "field leq 1.234567" ) );
        ASSERT_TRUE ( evaluator.is_activated()                     );
        ASSERT_TRUE ( evaluator.evaluate( x ).success              );
        ASSERT_FALSE( evaluator.evaluate( y ).success              );
        ASSERT_TRUE ( evaluator.evaluate( z ).success              );
    }
    {
        ASSERT_TRUE ( evaluator.expression( "field <= 1.234567" ) );
        ASSERT_TRUE ( evaluator.is_activated()                    );
        ASSERT_TRUE ( evaluator.evaluate( x ).success             );
        ASSERT_FALSE( evaluator.evaluate( y ).success             );
        ASSERT_TRUE ( evaluator.evaluate( z ).success             );
    }
}

TEST( EvaluatorTest, AndOperator )
{
    bar< unsigned, std::string > x{ 1, "bar"     };
    bar< unsigned, std::string > y{ 3, "bar bar" };

    booleval::evaluator<> evaluator
    {
        {
            { "field_1", &bar< unsigned, std::string >::value_1 },
            { "field_2", &bar< unsigned, std::string >::value_2 }
        }
    };

    {
        ASSERT_TRUE ( evaluator.expression( "field_1 1 and field_2 bar" ) );
        ASSERT_TRUE ( evaluator.is_activated()                            );
        ASSERT_TRUE ( evaluator.evaluate( x ).success                     );
        ASSERT_FALSE( evaluator.evaluate( y ).success                     );
    }
    {
        ASSERT_TRUE ( evaluator.expression( "field_1 eq 1 and field_2 eq bar" ) );
        ASSERT_TRUE ( evaluator.is_activated()                                  );
        ASSERT_TRUE ( evaluator.evaluate( x ).success                           );
        ASSERT_FALSE( evaluator.evaluate( y ).success                           );
    }
    {
        ASSERT_TRUE ( evaluator.expression( "field_1 == 1 and field_2 == bar" ) );
        ASSERT_TRUE ( evaluator.is_activated()                                  );
        ASSERT_TRUE ( evaluator.evaluate( x ).success                           );
        ASSERT_FALSE( evaluator.evaluate( y ).success                           );
    }
    {
        ASSERT_TRUE ( evaluator.expression( "field_1 == 1 && field_2 == bar" ) );
        ASSERT_TRUE ( evaluator.is_activated()                                 );
        ASSERT_TRUE ( evaluator.evaluate( x ).success                          );
        ASSERT_FALSE( evaluator.evaluate( y ).success                          );
    }
    {
        ASSERT_TRUE ( evaluator.expression( "field_1 == 3 && field_2 == bar" ) );
        ASSERT_TRUE ( evaluator.is_activated()                                 );
        ASSERT_FALSE( evaluator.evaluate( x ).success                          );
        ASSERT_FALSE( evaluator.evaluate( y ).success                          );
    }
}

TEST( EvaluatorTest, OrOperator )
{
    bar< unsigned, std::string > x{ 1, "bar"     };
    bar< unsigned, std::string > y{ 3, "bar bar" };

    booleval::evaluator<> evaluator
    {
        {
            { "field_1", &bar< unsigned, std::string >::value_1 },
            { "field_2", &bar< unsigned, std::string >::value_2 }
        }
    };

    {
        ASSERT_TRUE ( evaluator.expression( "field_1 1 or field_1 2" ) );
        ASSERT_TRUE ( evaluator.is_activated()                         );
        ASSERT_TRUE ( evaluator.evaluate( x ).success                  );
        ASSERT_FALSE( evaluator.evaluate( y ).success                  );
    }
    {
        ASSERT_TRUE ( evaluator.expression( "field_1 eq 1 or field_1 eq 2" ) );
        ASSERT_TRUE ( evaluator.is_activated()                               );
        ASSERT_TRUE ( evaluator.evaluate( x ).success                        );
        ASSERT_FALSE( evaluator.evaluate( y ).success                        );
    }
    {
        ASSERT_TRUE ( evaluator.expression( "field_1 == 1 or field_1 == 2" ) );
        ASSERT_TRUE ( evaluator.is_activated()                               );
        ASSERT_TRUE ( evaluator.evaluate( x ).success                        );
        ASSERT_FALSE( evaluator.evaluate( y ).success                        );
    }
    {
        ASSERT_TRUE ( evaluator.expression( "field_1 1 || field_1 2" ) );
        ASSERT_TRUE ( evaluator.is_activated()                         );
        ASSERT_TRUE ( evaluator.evaluate( x ).success                  );
        ASSERT_FALSE( evaluator.evaluate( y ).success                  );
    }
    {
        ASSERT_TRUE ( evaluator.expression( "field_1 eq 1 || field_1 eq 2" ) );
        ASSERT_TRUE ( evaluator.is_activated()                               );
        ASSERT_TRUE ( evaluator.evaluate( x ).success                        );
        ASSERT_FALSE( evaluator.evaluate( y ).success                        );
    }
    {
        ASSERT_TRUE ( evaluator.expression( "field_1 == 1 || field_1 == 2" ) );
        ASSERT_TRUE ( evaluator.is_activated()                               );
        ASSERT_TRUE ( evaluator.evaluate( x ).success                        );
        ASSERT_FALSE( evaluator.evaluate( y ).success                        );
    }
    {
        ASSERT_TRUE( evaluator.expression( "field_1 == 1 || field_2 == \"bar bar\"" ) );
        ASSERT_TRUE( evaluator.is_activated()                                         );
        ASSERT_TRUE( evaluator.evaluate( x ).success                                  );
        ASSERT_TRUE( evaluator.evaluate( y ).success                                  );
    }
    {
        ASSERT_TRUE( evaluator.expression( "field_1 == 3 || field_2 == bar" ) );
        ASSERT_TRUE( evaluator.is_activated()                                 );
        ASSERT_TRUE( evaluator.evaluate( x ).success                          );
        ASSERT_TRUE( evaluator.evaluate( y ).success                          );
    }
}

TEST( EvaluatorTest, MultipleOperators )
{
    bar< std::string, unsigned > x{ "foo", 1 };
    bar< std::string, unsigned > y{ "bar", 2 };
    bar< std::string, unsigned > m{ "baz", 1 };
    bar< std::string, unsigned > n{ "qux", 2 };

    booleval::evaluator<> evaluator
    {
        {
            { "field_1", &bar< std::string, unsigned >::value_1 },
            { "field_2", &bar< std::string, unsigned >::value_2 }
        }
    };

    {
        ASSERT_TRUE ( evaluator.expression( "(field_1 foo and field_2 1)" ) );
        ASSERT_TRUE ( evaluator.is_activated()        );
        ASSERT_TRUE ( evaluator.evaluate( x ).success );
        ASSERT_FALSE( evaluator.evaluate( y ).success );
        ASSERT_FALSE( evaluator.evaluate( m ).success );
        ASSERT_FALSE( evaluator.evaluate( n ).success );
    }
    {
        ASSERT_TRUE ( evaluator.expression( "field_1 foo and field_2 1 and field_1 bar" ) );
        ASSERT_TRUE ( evaluator.is_activated()        );
        ASSERT_FALSE( evaluator.evaluate( x ).success );
        ASSERT_FALSE( evaluator.evaluate( y ).success );
        ASSERT_FALSE( evaluator.evaluate( m ).success );
        ASSERT_FALSE( evaluator.evaluate( n ).success );
    }
    {
        ASSERT_TRUE ( evaluator.expression( "(field_1 foo or field_1 bar) and (field_2 2 or field_2 1)" ) );
        ASSERT_TRUE ( evaluator.is_activated()        );
        ASSERT_TRUE ( evaluator.evaluate( x ).success );
        ASSERT_TRUE ( evaluator.evaluate( y ).success );
        ASSERT_FALSE( evaluator.evaluate( m ).success );
        ASSERT_FALSE( evaluator.evaluate( n ).success );
    }
    {
        ASSERT_TRUE ( evaluator.expression( "(field_1 foo and field_2 1) or (field_1 qux and field_2 2)" ) );
        ASSERT_TRUE ( evaluator.is_activated()        );
        ASSERT_TRUE ( evaluator.evaluate( x ).success );
        ASSERT_FALSE( evaluator.evaluate( y ).success );
        ASSERT_FALSE( evaluator.evaluate( m ).success );
        ASSERT_TRUE ( evaluator.evaluate( n ).success );
    }
}

TEST( EvaluatorTest, DifferentClasses )
{
    foo< unsigned              > x{ 1        };
    bar< unsigned, std::string > y{ 2, "bar" };

    booleval::evaluator<> evaluator
    {
        {
            { "field_1", &foo< unsigned              >::value   },
            { "field_2", &bar< unsigned, std::string >::value_2 }
        }
    };

    {
        ASSERT_TRUE ( evaluator.expression( "field_1 one and field_2 2" ) );
        ASSERT_TRUE ( evaluator.is_activated()                            );
        ASSERT_FALSE( evaluator.evaluate( x ).success                     );
        ASSERT_FALSE( evaluator.evaluate( y ).success                     );
    }
}

TEST( EvaluatorTest, UnknownField )
{
    foo< unsigned > x{ 1 };

    booleval::evaluator<> evaluator
    {
        {
            { "field", &foo< unsigned >::value }
        }
    };

    {
        ASSERT_TRUE ( evaluator.expression( "unknown_field 1" )        );
        ASSERT_TRUE ( evaluator.is_activated()                         );

        auto const result{ evaluator.evaluate( x ) };
        ASSERT_FALSE( result.success                  );
        ASSERT_EQ   ( result.message, "Unknown field" );
    }
}

// TEST_F(EvaluatorTest, FieldNotValid) {
//     obj<std::string> foo{ "foo" };

//     booleval::evaluator<booleval::utils::any_mem_fn_bool> evaluator({
//         { "field_a_valid", &obj<std::string>::value_a_valid },
//         { "field_a_not_valid",& obj<std::string>::value_a_notvalid }
//     });

//     ASSERT_TRUE(evaluator.expression("field_a_valid foo"));
//     ASSERT_TRUE(evaluator.is_activated());
//     ASSERT_TRUE(evaluator.evaluate(foo));

//     ASSERT_TRUE(evaluator.expression("field_a_not_valid foo"));
//     ASSERT_TRUE(evaluator.is_activated());
//     ASSERT_FALSE(evaluator.evaluate(foo));
// }
