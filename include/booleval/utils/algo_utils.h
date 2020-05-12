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

namespace booleval {

namespace utils {

/**
 * Finds the first element in the range [first, last) that
 * is equal to the specified value.
 *
 * @param first Beginning of the range
 * @param last  End of the range
 * @param value Value to compare the elements to
 *
 * @return Iterator to the first element satisfying the condition or
 *         last if no such element is found.
 */
template <typename InputIt, typename T>
constexpr InputIt find(InputIt first, InputIt last, T const& value) {
    for (; first != last; ++first) {
        if (*first == value) {
            return first;
        }
    }
    return last;
}

/**
 * Finds the first element in the range [first, last) for
 * which the predicate returns true.
 *
 * @param first Beginning of the range
 * @param last  End of the range
 * @param p     Unary predicate which returns true for the required element
 *
 * @return Iterator to the first element satisfying the condition or
 *         last if no such element is found.
 */
template <typename InputIt, typename UnaryPredicate>
constexpr InputIt find_if(InputIt first, InputIt last, UnaryPredicate p) {
    for (; first != last; ++first) {
        if (p(*first)) {
            return first;
        }
    }
    return last;
}

/**
 * Finds the first element in the range [first, last) for
 * which the predicate returns false.
 *
 * @param first Beginning of the range
 * @param last  End of the range
 * @param p     Unary predicate which returns false for the required element
 *
 * @return Iterator to the first element satisfying the condition or
 *         last if no such element is found.
 */
template <typename InputIt, typename UnaryPredicate>
constexpr InputIt find_if_not(InputIt first, InputIt last, UnaryPredicate p) {
    for (; first != last; ++first) {
        if (!p(*first)) {
            return first;
        }
    }
    return last;
}

} // utils

} // booleval