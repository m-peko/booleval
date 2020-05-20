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

#ifndef BOOLEVAL_SPLIT_RANGE_H
#define BOOLEVAL_SPLIT_RANGE_H

#include <string>
#include <algorithm>
#include <string_view>
#include <booleval/utils/string_utils.h>

namespace booleval {

namespace utils {

constexpr auto whitespace_char{ ' ' };
constexpr auto single_quote_char{ '\'' };
constexpr auto double_quote_char{ '\"' };

/**
 * class split_range
 *
 * Represents the range of tokens computed by splitting the given
 * string view by the specified delimiters.
 */
template <split_options options = split_options::exclude_delimiters |
                                  split_options::allow_quoted_strings,
          char quote_char = double_quote_char>
class split_range {
public:
    template <split_options iter_options, char iter_quote_char>
    class iterator {
        friend split_range;

    public:
        struct element {
            bool quoted{ false };
            std::size_t index{ 0 };
            std::string_view value{ "" };
        };

        using iterator_category = std::forward_iterator_tag;
        using value_type        = element;
        using difference_type   = std::ptrdiff_t;
        using pointer           = value_type*;
        using reference         = value_type&;

    public:
        explicit constexpr iterator(std::string_view strv, std::string const& delims) noexcept
            : strv_(strv),
              delims_(delims),
              prev_(std::cbegin(strv_)) {
            apply_split_options();
            next();
        }

        constexpr iterator(iterator&& rhs) = default;
        constexpr iterator(iterator const& rhs) = default;

        iterator& operator=(iterator&& rhs) = default;
        iterator& operator=(iterator const& rhs) = default;

        [[nodiscard]] constexpr auto operator*() const noexcept {
            return curr_elem_;
        }

        [[nodiscard]] constexpr auto operator->() const noexcept {
            return &curr_elem_;
        }

        constexpr iterator& operator++() noexcept {
            ++curr_elem_.index;

            if (curr_elem_.quoted) {
                curr_ = skip_char(curr_, whitespace_char);
                curr_ = std::next(curr_);
                prev_ = std::next(curr_);
            } else {
                if constexpr (is_set(iter_options, split_options::include_delimiters)) {
                    curr_ = skip_char(curr_, whitespace_char);
                    prev_ = curr_;
                } else {
                    prev_ = std::next(curr_);
                }
            }

            curr_elem_.quoted = false;

            next();
            return *this;
        }

        constexpr iterator operator++(int) noexcept {
            iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        [[nodiscard]] constexpr bool operator!=(iterator const& rhs) const noexcept {
            return prev_ != rhs.prev_;
        }

        [[nodiscard]] constexpr bool operator==(iterator const& rhs) const noexcept {
            return prev_ == rhs.prev_;
        }

        ~iterator() = default;

    private:
        explicit constexpr iterator(char const * const end) noexcept
            : prev_(end)
        {}

        /**
         * Applies splitting options to the string containing delimiters.
         */
        constexpr void apply_split_options() noexcept {
            if constexpr (is_set(iter_options, split_options::split_by_whitespace)) {
                delims_.append(1, ' ');
            }
        }

        /**
         * Finds the iterator pointing to the beginning of the next token in the specified range.
         *
         * @param first Iterator to the first element of the range to examine
         * @param last  Iterator to the last element of the range to examine
         *
         * @return Iterator to the beginning of the next token
         */
        [[nodiscard]] constexpr char const* find_next(char const* first, char const* last) const noexcept {
            if constexpr (is_set(iter_options, split_options::allow_quoted_strings)) {
                if (curr_elem_.quoted) {
                    return find_next_quote(first, last);
                } else {
                    return std::min(
                        find_next_quote(first, last),
                        find_next_delim(first, last)
                    );
                }
            } else {
                return find_next_delim(first, last);
            }
        }

        /**
         * Finds the iterator pointing to the first delimiter in the specified range.
         *
         * @param first Iterator to the first element of the range to examine
         * @param last  Iterator to the last element of the range to examine
         *
         * @return Iterator to the first delimiter
         */
        [[nodiscard]] constexpr char const* find_next_delim(char const* first, char const* last) const noexcept {
            return std::find_first_of(first, last, std::cbegin(delims_), std::cend(delims_));
        }

        /**
         * Finds the iterator pointing to the first quote character in the specified range.
         *
         * @param first Iterator to the first element of the range to examine
         * @param last  Iterator to the last element of the range to examine
         *
         * @return Iterator to the first quote character
         */
        [[nodiscard]] constexpr char const* find_next_quote(char const* first, char const* last) const noexcept {
            return std::find(first, last, iter_quote_char);
        }

        /**
         * Skips all the characters equal to the specified character.
         *
         * @param first Iterator to the first element to examine
         *
         * @return Iterator to the first element which is not equal to the specified character
         */
        [[nodiscard]] constexpr char const* skip_char(char const* first, char const c) const noexcept {
            while (std::cend(strv_) != first && c == *first) {
                first = std::next(first);
            }
            return first;
        }

        /**
         * Finds the next token and sets the current element properties.
         */
        constexpr void next() noexcept {
            if (std::cend(strv_) == prev_) {
                return;
            }

            curr_ = find_next(prev_, std::cend(strv_));
            if (iter_quote_char == *curr_) {
                curr_elem_.quoted = true;
                prev_ = skip_char(curr_, iter_quote_char);
                curr_ = find_next(prev_, std::cend(strv_));
            } else {
                curr_elem_.quoted = false;
                if (prev_ == curr_) {
                    curr_ = std::next(curr_);
                }
            }

            if (std::distance(std::begin(strv_), prev_) >
                std::distance(std::begin(strv_), std::end(strv_))) {
                prev_ = std::end(strv_);
                return;
            }

            curr_elem_.value = strv_.substr(
                std::distance(std::begin(strv_), prev_),
                std::distance(prev_, curr_)
            );
        }

    private:
        std::string_view strv_;
        std::string delims_;

        char const* prev_;
        char const* curr_;

        element curr_elem_;
    };

public:
    constexpr split_range() = default;

	constexpr split_range(std::string_view strv, std::string const& delims = " ")
        : strv_(strv),
          delims_(delims)
    {}

    constexpr split_range(split_range&& rhs) = default;
    constexpr split_range(split_range const& rhs) = default;

    split_range& operator=(split_range&& rhs) = default;
    split_range& operator=(split_range const& rhs) = default;

    ~split_range() = default;

    /**
     * Returns an iterator to the first element of the range.
     *
     * @return Iterator to the first element
     */
    [[nodiscard]] constexpr auto begin() const noexcept {
        return iterator<options, quote_char>(strv_, delims_);
    }

    /**
     * Returns an iterator to the element following the last element of the range.
     *
     * @return Iterator to the element following the last element
     */
    [[nodiscard]] constexpr auto end() const noexcept {
        return iterator<options, quote_char>(strv_.data() + strv_.size());
    }

private:
    std::string_view strv_;
    std::string delims_;
};

} // utils

} // booleval

#endif // BOOLEVAL_SPLIT_RANGE_H