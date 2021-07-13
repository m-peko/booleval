/*
 * Copyright (c) 2021, Marin Peko
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

#ifndef BOOLEVAL_FIELD_HPP
#define BOOLEVAL_FIELD_HPP

#include <functional>
#include <string_view>
#include <booleval/utils/any_value.hpp>

namespace booleval
{

template< typename C >
struct field;

/**
 * @class field_base
 *
 * Represents a base class for the field class.
 */
struct field_base
{
    field_base() noexcept = default;

    field_base( field_base       && rhs ) noexcept = default;
    field_base( field_base const  & rhs ) noexcept = default;

    field_base( std::string_view const name ) noexcept : name{ name } {}

    field_base & operator=( field_base       && rhs ) noexcept = default;
    field_base & operator=( field_base const  & rhs ) noexcept = default;

    virtual ~field_base() noexcept = default;

    template< typename C >
    utils::any_value invoke( C && obj ) noexcept
    {
        auto const * f{ dynamic_cast< field< std::remove_reference_t< C > >* >( this ) };

        if ( f == nullptr ) { return {}; }

        return f->get( std::move( obj ));
    }

    std::string_view name{};
};

/**
 * @class field
 *
 * Contains string representation of a certain class field and
 * getter class member function associated to this field.
 */
template< typename C >
struct field : field_base
{
    field() noexcept = default;

    field( field       && rhs ) noexcept = default;
    field( field const  & rhs ) noexcept = default;

    template< typename R >
    field( std::string_view const name, R ( C::*m )() ) noexcept : field_base{ name }
    {
        get = [ m ]( C && obj )
        {
            return ( obj.*m )();
        };
    }

    template< typename R >
    field( std::string_view const name, R ( C::*m )() const ) noexcept : field_base{ name }
    {
        get = [ m ]( C && obj )
        {
            return ( obj.*m )();
        };
    }

    field & operator=( field       && rhs ) noexcept = default;
    field & operator=( field const  & rhs ) noexcept = default;

    ~field() noexcept = default;

    std::function< utils::any_value( C && ) > get{ nullptr };
};

template< typename C, typename R >
auto make_field( std::string_view const name, R( C::*m )() const ) noexcept
{
    return new field< C >( name, m );
}

} // namespace booleval

#endif // BOOLEVAL_FIELD_HPP
