# booleval

[![license](https://img.shields.io/badge/license-MIT-brightgreen.svg?style=flat)](https://github.com/m-peko/booleval/blob/master/LICENSE)

Recursive descent parser for evaluating boolean expressions.

## Use case

If there is a simple class named `Object` with the 3 fields: `field_A`, `field_B` and `field_C`, it is possible to have a string like this:

`(field_A foo and field_B bar) or (field_B john and field_C doe)`

which evaluates to `true` or `false` depending on the content of the instance of the class `Object`.

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

If you find that any tests fail, please create an ticket in the
issue tracker indicating the platform and architecture you're using.
