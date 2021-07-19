<div align="center">

<img src="docs/booleval-title.png"/>

[![license](https://img.shields.io/badge/license-MIT-brightgreen.svg?style=flat)](https://github.com/m-peko/booleval/blob/master/LICENSE)
[![Build Status](https://app.travis-ci.com/m-peko/booleval.svg?branch=master)](https://travis-ci.org/m-peko/booleval)
[![codecov](https://codecov.io/gh/m-peko/booleval/branch/master/graph/badge.svg)](https://codecov.io/gh/m-peko/booleval)
[![Standard](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B17)

Header-only C++17 library for evaluating logical expressions.

</div>

<br/>

The library is under development and subject to change. Contributions are welcome. You can also log an issue if you have a wish for enhancement or if you spot a bug.

## Quick start

```cpp
#include <string>
#include <iostream>
#include <booleval/evaluator.hpp>

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

int main()
{
    foo< std::string > x{ "foo" };
    foo< std::string > y{ "bar" };

    booleval::evaluator evaluator
    {
        booleval::make_field( "field", &foo< std::string >::value )
    };

    if ( !evaluator.expression( "field eq foo" ) )
    {
        std::cerr << "Expression not valid!" << std::endl;
    }

    if ( evaluator.is_activated() )
    {
        std::cout << std::boolalpha << evaluator.evaluate( x ) << std::endl; // true
        std::cout << std::boolalpha << evaluator.evaluate( y ) << std::endl; // false
    }

    return 0;
}
```

## Table of Contents

* [Motivation](#motivation)
* [Getting Started](#getting-started)
    * [Zero Copy](#zero-copy)
    * [EQUAL TO Operator](#equal-to-operator)
    * [Valid Expressions](#valid-expressions)
    * [Invalid Expressions](#invalid-expressions)
    * [Evaluation Result](#evaluation-result)
    * [Supported Tokens](#supported-tokens)
* [Benchmark](#benchmark)
* [Compilation](#compilation)
* [Tests](#tests)
* [Compiler Compatibility](#compiler-compatibility)
* [Contributing](#contributing)
* [License](#license)
* [Support](#support)

## Motivation

`booleval` is a header-only C++17 library for evaluating logical expressions. It implements recursive descent parser mechanism for building an expression tree for a user-defined logical expression. After the expression tree is being built, map of fields and values representing a certain object can be passed to the `evaluator` component of this library which will evaluate those values to `true` or `false` according to the user-defined logical expression.

<br/>
<p align="center">
    <img src="docs/booleval-tree.png"/>
</p>
<br/>
<br/>

In programming languages like Java and C# accessing arbitrary fields inside a class represents an omnipresent problem. However, it got solved by introducing a **reflections** feature. This feature provides us information about the class to which a certain object belongs to and, also, the methods of that class which we can invoke at runtime.

Since the reflection feature is missing in C++, `booleval` library is implemented. It checks whether the members of a class to which an object belongs to have certain values. Members of a class are specified in a string format and can be used to form a logical expression.

Providing an end-user a functionality of specifying a logical expression is a common way of filtering out a large amounts of objects. E.g. [`tcpdump`](https://www.tcpdump.org/manpages/tcpdump.1.html) and [BPF](https://en.wikipedia.org/wiki/Berkeley_Packet_Filter) (Berkeley Packet Filter), network tools available on most UNIX-like operating systems, have pretty much the same syntax for their filter expression.

## Getting Started

`booleval` is a header-only C++17 library for evaluating logical expressions.

### Zero Copy

In order to improve performance, `booleval` library does not copy objects that are being evaluated.

### EQUAL TO operator

EQUAL TO operator is an optional operator. Therefore, logical expression that checks whether a field with the name `field_a` has a value of `foo` can be constructed in a two different ways:

- EQUAL TO operator is specified in the expression: `field_a eq foo`
- EQUAL TO operator is **not** specified in the expression: `field_a foo`

To conclude, equality operator is a default operator between two fields. Thus, it **does not need** to be specified in the logical expression.

### Valid expressions

- `(field_a foo and field_b bar) or field_a bar`
- `(field_a eq foo and field_b eq bar) or field_a eq bar`

### Invalid expressions

- `(field_a foo and field_b bar` _Note: Missing closing parentheses_
- `field_a foo bar` _Note: Two field values in a row_

### Evaluation Result

Result of evaluation process contains two information:

- `success`: `true` if evaluation process is successful; otherwise, `false`
- `message`: meaningful message if evaluation process is unsuccessful; otherwise, empty message

This kind of enriched lightweight result is being used instead of exceptions for performance reasons.

Examples of messages:

- `"Missing operand"`
- `"Unknown field"`
- `"Unknown token type"`

### Supported tokens

|Name|Keyword|Symbol|
|:---:|:---:|:---:|
|AND operator|AND / and|&&|
|OR operator|OR / or|\| \||
|EQUAL TO operator|EQ / eq|==|
|NOT EQUAL TO operator|NEQ / neq|!=|
|GREATER THAN operator|GT / gt|>|
|LESS THAN operator|LT / lt|<|
|GREATER THAN OR EQUAL TO operator|GEQ / geq|>=|
|LESS THAN OR EQUAL TO operator|LEQ / leq|<=|
|LEFT parentheses|&empty;|(|
|RIGHT parentheses|&empty;|)|

## Benchmark

Following table shows benchmark results:

|Benchmark|Time|Iterations
|:---:|:---:|:---:|
|Building expression tree|3817 ns|180904|
|Evaluation|1285 ns|532522|

In other words, it is possible to evaluate **2,413,045.84 objects per second**.

## Compilation

In order to compile the library, run the following commands:

```Shell
$ # create the build directory
$ mkdir build
$ cd build

$ # configure the project
$ cmake ../

$ # compile
$ make
```

## Tests

In order to run unit tests, run the following commands:

```Shell
$ # fetch the googletest submodule, needed for tests
$ git submodule init
$ git submodule update

$ mkdir build
$ cd build

$ # configure the project
$ cmake ..

$ # compile tests
$ make tests

$ # run tests
$ make test
```

## Compiler Compatibility

* Clang/LLVM >= 7
* MSVC++ >= 19.16
* GCC >= 8.4

There are no 3<sup>rd</sup> party dependencies.

## Contributing

Feel free to contribute.

If you find that any of the tests **fail**, please create a ticket in the issue tracker indicating the following information:

* platform
* architecture
* library version
* minimal reproducible example

## License

The project is available under the [MIT](https://opensource.org/licenses/MIT) license.

## Support

If you like the work `booleval` library is doing, please consider supporting it:

<a href="https://www.buymeacoffee.com/mpeko" target="_blank"><img src="https://cdn.buymeacoffee.com/buttons/v2/default-red.png" alt="Buy Me A Coffee" style="height: 60px !important;width: 217px !important;" ></a>
