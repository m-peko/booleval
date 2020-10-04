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

#ifndef BOOLEVAL_EXCEPTIONS_H
#define BOOLEVAL_EXCEPTIONS_H

#include <string>
#include <stdexcept>

/**
 * class base_exception
 *
 * Base class for all booleval exceptions.
 */
struct base_exception : std::runtime_error {
    base_exception()
        : std::runtime_error(std::string{})
    {}

    base_exception(std::string const& message)
        : std::runtime_error(message)
    {}

    base_exception(char const* message)
        : std::runtime_error(message)
    {}
};

/**
 * struct field_not_found
 *
 * Exception thrown when a field is not found.
 */
struct field_not_found : base_exception {
    field_not_found()
        : base_exception("Field not found")
    {}

    field_not_found(std::string const& field)
        : base_exception("Field '" + field + "' not found")
    {}

    field_not_found(std::string_view field)
        : base_exception("Field '" + std::string(field) + "' not found")
    {}

    field_not_found(char const* field)
        : base_exception("Field '" + std::string(field) + "' not found")
    {}
};

#endif // BOOLEVAL_EXCEPTIONS_H