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

#ifndef BOOLEVAL_ANY_MEM_FN_H
#define BOOLEVAL_ANY_MEM_FN_H

#include <any>
#include <functional>
#include <booleval/utils/any_value.hpp>

namespace booleval {

namespace utils {

/**
 * class any_mem_fn
 *
 * Represents class member function of any signature.
 */
class any_mem_fn {
public:
    any_mem_fn() = default;
    any_mem_fn(any_mem_fn&& rhs) = default;
    any_mem_fn(any_mem_fn const& rhs) = default;

    template <typename Ret, typename C>
    any_mem_fn(Ret (C::*m)()) {
        fn_ = [m](std::any a) {
            return (std::any_cast<C>(a).*m)();
        };
    }

    template <typename Ret, typename C>
    any_mem_fn(Ret (C::*m)() const) {
        fn_ = [m](std::any a) {
            return (std::any_cast<C>(a).*m)();
        };
    }

    any_mem_fn& operator=(any_mem_fn&& rhs) = default;
    any_mem_fn& operator=(any_mem_fn const& rhs) = default;

    ~any_mem_fn() = default;

    template <typename T>
    any_value invoke(T obj) {
        try {
            return fn_(obj);
        } catch (std::bad_any_cast const&) {
            return {};
        }
    }

private:
    std::function<any_value(std::any)> fn_;
};

} // utils

} // booleval

#endif // BOOLEVAL_ANY_MEM_FN_H
