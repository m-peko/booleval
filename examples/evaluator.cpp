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
#include <booleval/evaluator.h>

struct obj {
public:
    obj(std::string const& field_a, uint32_t const field_b)
        : field_a_(field_a),
          field_b_(field_b)
    {}

    std::string const& field_a() const noexcept {
        return field_a_;
    }

    uint32_t field_b() const noexcept {
        return field_b_;
    }

private:
    std::string field_a_;
    uint32_t field_b_;
};

int main() {
    obj pass("foo", 123);
    obj fail("bar", 456);

    booleval::evaluator evaluator;

    auto valid = evaluator.expression("field_a foo and field_b 123");
    if (!valid) {
        std::cerr << "Expression not valid!" << std::endl;
    }

    if (evaluator.is_activated()) {
        evaluator.map({
            { "field_a", &obj::field_a },
            { "field_b", &obj::field_b }
        });

        std::cout << std::boolalpha << evaluator.evaluate(pass) << std::endl;
        std::cout << std::boolalpha << evaluator.evaluate(fail) << std::endl;
    } else {
        std::cerr << "Evaluator is not activated!" << std::endl;
    }

    return 0;
}