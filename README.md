# booleval

[![license](https://img.shields.io/badge/license-MIT-brightgreen.svg?style=flat)](https://github.com/m-peko/booleval/blob/master/LICENSE) [![Build status](https://ci.appveyor.com/api/projects/status/gusrrn0mn67q2yaj?svg=true)](https://ci.appveyor.com/project/m-peko/booleval)

Recursive descent parser for evaluating boolean expressions.

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

class Object {
public:
    Object(std::string const& field_a, std::string const& field_b)
        : field_a_(field_a), field_b_(field_b) {}

    std::map<std::string, std::string> fields() const noexcept {
        return {
            { "field_a", field_a_ },
            { "field_b", field_b_ }
        };
    }

private:
    std::string field_a_;
    std::string field_b_;
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
        std::cout << evaluator.evaluate(valid_obj.fields()) << std::endl;   // 1
        std::cout << evaluator.evaluate(invalid_obj.fields()) << std::endl; // 0
    } else {
        std::cerr << "Evaluator is not activated!" << std::endl;
    }

    return 0;
}

```
