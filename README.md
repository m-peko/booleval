<div align="center">

<img src="docs/booleval-title.png"/>

[![license](https://img.shields.io/badge/license-MIT-brightgreen.svg?style=flat)](https://github.com/m-peko/booleval/blob/master/LICENSE)
[![Build Status](https://travis-ci.org/m-peko/booleval.svg?branch=master)](https://travis-ci.org/m-peko/booleval)
[![codecov](https://codecov.io/gh/m-peko/booleval/branch/master/graph/badge.svg)](https://codecov.io/gh/m-peko/booleval)

Modern C++17 library for evaluating logical expressions.

</div>

<br/>

The library is under development and subject to change. Contributions are welcome. You can also log an issue if you have a wish for enhancement or if you spot a bug.

## Overview

1. [About](#about)
2. [Motivation](#motivation)
3. [Specification](#specification)
4. [Requirements](#requirements)
5. [Compilation](#compilation)
6. [Tests](#tests)
7. [Example](#example)
8. [Support](#support)

<a name="about"></a>

## About

`booleval` is a small C++17 library for evaluating logical expressions. It implements recursive descent parser mechanism for building an expression tree for a user-defined logical expression. After the expression tree is being built, map of fields and values representing a certain object can be passed to the `evaluator` component of this library which will evaluate those values to `true` or `false` according to the user-defined logical expression.

<br/>
<p align="center">
    <img src="docs/booleval-tree.png"/>
</p>
<br/>
<br/>

<a name="motivation"></a>

## Motivation

In programming languages like Java and C# accessing arbitrary fields inside a class represents an omnipresent problem. However, it got solved by introducing a **reflections** feature. This feature provides us information about the class to which a certain object belongs to and, also, the methods of that class which we can invoke at runtime.

Since the reflection feature is missing in C++, `booleval` library is implemented. It checks whether the members of a class to which an object belongs to have certain values. Members of a class are specified in a string format and can be used to form a logical expression.

Providing an end-user a functionality of specifying a logical expression is a common way of filtering out a large amounts of objects. E.g. [`tcpdump`](https://www.tcpdump.org/manpages/tcpdump.1.html) and [BPF](https://en.wikipedia.org/wiki/Berkeley_Packet_Filter) (Berkeley Packet Filter), network tools available on most UNIX-like operating systems, have pretty much the same syntax for their filter expression.

<br/>
<p align="center">
    <img src="docs/command-line.gif"/>
</p>
<br/>

<a name="specification"></a>

## Specification

EQUAL TO operator is an optional operator. Therefore, logical expression that checks whether a field with the name `field_a` has a value of `foo` can be constructed in a two different ways:

- EQUAL TO operator is specified in the expression: `field_a eq foo`
- EQUAL TO operator is **not** specified in the expression: `field_a foo`

To conclude, equality operator is a default operator between two fields. Thus, it **does not need** to be specified in the logical expression.

### Examples of valid expressions
- `(field_a foo and field_b bar) or field_a bar`
- `(field_a eq foo and field_b eq bar) or field_a eq bar`

### Examples of invalid expressions
- `(field_a foo and field_b bar` _Note: Missing closing parentheses_
- `field_a foo bar` _Note: Two field values in a row_

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

<a name="requirements"></a>

## Requirements

`booleval` project requires C++17 compiler and has been tested on:

- gcc 8.4.0
- clang 7.0.0
- msvc 19.16

There are no 3rd party dependencies.

<a name="compilation"></a>

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

<a name="tests"></a>

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

If you find that any tests **fail**, please create a ticket in the
issue tracker indicating the following information:
- platform
- architecture
- library version
- minimal reproducible example

<a name="example"></a>

## Example

Let's say we have a large number of objects coming through our interface. Objects can be of the following `class obj` type:

```c++
struct obj {
    std::string field_a_;
    uint32_t field_b_;

public:
    obj(std::string const& field_a, uint32_t const field_b)
        : field_a_(field_a),
          field_b_(field_b)
    {}

    std::string const& field_a() {
        return field_a_;
    }

    uint32_t field_b() {
        return field_b_;
    }
};
```

In our application, we want to let end-users to specify some sort of a rule which will filter out only those objects that contain certain field values. This rule can have a form of a following logical expression `field_a foo and field_b 123`. Now, we can use `booleval::evaluator` component to check whether objects conform specified rule.

```c++
#include <string>
#include <iostream>
#include <booleval/evaluator.hpp>

int main() {
    obj pass("foo", 123);
    obj fail("bar", 456);

    booleval::evaluator evaluator({
        { "field_a", &obj::field_a },
        { "field_b", &obj::field_b }
    });

    auto valid = evaluator.expression("field_a foo and field_b 123");
    if (!valid) {
        std::cerr << "Expression not valid!" << std::endl;
    }

    if (evaluator.is_activated()) {
        std::cout << std::boolalpha << evaluator.evaluate(pass) << std::endl;  // output: true
        std::cout << std::boolalpha << evaluator.evaluate(fail) << std::endl;  // output: false
    } else {
        std::cerr << "Evaluator is not activated!" << std::endl;
    }

    return 0;
}
```

## Support

If you like the work `booleval` library is doing, please consider supporting it:

<a href="https://www.buymeacoffee.com/mpeko" target="_blank"><img src="https://cdn.buymeacoffee.com/buttons/v2/default-red.png" alt="Buy Me A Coffee" style="height: 60px !important;width: 217px !important;" ></a>
