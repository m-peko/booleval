# booleval

[![license](https://img.shields.io/badge/license-MIT-brightgreen.svg?style=flat)](https://github.com/m-peko/booleval/blob/master/LICENSE) [![Build status](https://ci.appveyor.com/api/projects/status/gusrrn0mn67q2yaj?svg=true)](https://ci.appveyor.com/project/m-peko/booleval)

Recursive descent parser for evaluating boolean expressions.

## Expression

Valid: `(field_a foo and field_b bar) or (field_a bar and field_b baz)`

Invalid: `(field_a foo and field_b bar` _Note: Missing closing parenthesis_

## Supported tokens

Logical operators: `and` / `&&`, `or` / `||`

Relational operators: `neq` / `!=`, `greater` / `>`, `less` / `<` *

_* Note: Equality operator does not have a corresponding token. Instead it is a default operator between two fields._

Parentheses: `(`, `)`

## Compiling

In order to compile, execute:

```Shell
# create the build directory
mkdir build
cd build

# configure the project
cmake ../

# compile
make
```

## Running tests

In order to run unit tests, please execute following commands:

```Shell
# fetch the googletest submodule, needed for tests
git submodule init
git submodule update

mkdir build
cd build

# configure the project
cmake ..

# compile tests
make tests

# run tests
make test
```

If you find that any tests fail, please create a ticket in the
issue tracker indicating the platform and architecture you're using.

## Example

```c++
#include <map>
#include <string>
#include <iostream>
#include <booleval/evaluator.h>

struct Object {
    Object(std::string const& a, std::string const& b)
        : field_a(a),
          field_b(b)
    {}

    std::string field_a;
    std::string field_b;
};

int main() {
    Object valid_obj("foo", "bar");
    Object invalid_obj("bar", "baz");

    booleval::Evaluator evaluator;

    auto success = evaluator.build_expression_tree("field_a foo and field_b bar");
    if (!success) {
        std::cerr << "Error while building expression tree!" << std::endl;
    }

    if (evaluator.is_activated()) {
        // 'valid_obj' satisfies expression
        // output: 1 i.e. true
        std::cout << evaluator.evaluate({ "field_a": valid_obj.field_a, "field_b": valid_obj.field_b }) << std::endl;
        
        // 'invalid_obj' does not satisfy expression
        // output: 0 i.e. false
        std::cout << evaluator.evaluate({ "field_a": invalid_obj.field_a, "field_b": invalid_obj.field_b }) << std::endl;
    } else {
        std::cerr << "Evaluator is not activated!" << std::endl;
    }

    return 0;
}

```
