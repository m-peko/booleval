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

#ifndef BOOLEVAL_ANY_MEM_FN_HPP
#define BOOLEVAL_ANY_MEM_FN_HPP

#include <any>
#include <functional>
#include <booleval/utils/any_value.hpp>

namespace booleval::utils {

/**
 * @class any_mem_fn
 *
 * Represents class member function of any signature.
 */
class any_mem_fn
{
public:
    any_mem_fn() = default;
    any_mem_fn( any_mem_fn       && rhs ) = default;
    any_mem_fn( any_mem_fn const  & rhs ) = default;

    template< typename Ret, typename C >
    any_mem_fn( Ret ( C::*mem_fn )() )
    {
        fn_ = [ mem_fn ]( std::any obj )
        {
            return ( std::any_cast< C >( obj ).*mem_fn )();
        };
    }

    template< typename Ret, typename C >
    any_mem_fn( Ret ( C::*mem_fn )() const )
    {
        fn_ = [ mem_fn ]( std::any obj )
        {
            return ( std::any_cast< C >( obj ).*mem_fn )();
        };
    }

    any_mem_fn & operator=( any_mem_fn       && rhs ) = default;
    any_mem_fn & operator=( any_mem_fn const  & rhs ) = default;

    ~any_mem_fn() = default;

    template< typename T >
    any_value invoke( T && obj )
    {
        try
        {
            return fn_( std::forward< T >( obj ) );
        }
        catch ( std::bad_any_cast const & )
        {
            return {};
        }
    }

private:
    std::function< any_value( std::any ) > fn_;
};

/**
 * @class any_mem_fn_bool
 *
 * Represents class member function returning any signature taking a
 * is_valid parameter. If is_valid is set to false, the evaluation will fail.
 */
class any_mem_fn_bool
{
public:
    any_mem_fn_bool() = default;
    any_mem_fn_bool( any_mem_fn_bool       && rhs ) = default;
    any_mem_fn_bool( any_mem_fn_bool const  & rhs ) = default;

    template< typename Ret, typename C >
    any_mem_fn_bool( Ret ( C::* mem_fn )( bool & ) )
    {
        fn_ = [ mem_fn ]( std::any obj, bool & is_valid )
        {
            return ( std::any_cast< C >( obj ).*mem_fn )( is_valid );
        };
    }

    template< typename Ret, typename C >
    any_mem_fn_bool( Ret ( C::* mem_fn )( bool & ) const )
    {
        fn_ = [ mem_fn ]( std::any obj, bool & is_valid )
        {
            return ( std::any_cast< C >( obj ).*mem_fn )( is_valid );
        };
    }

    any_mem_fn_bool& operator=( any_mem_fn_bool       && rhs ) = default;
    any_mem_fn_bool& operator=( any_mem_fn_bool const  & rhs ) = default;

    ~any_mem_fn_bool() = default;

    template< typename T >
    any_value invoke( T && obj )
    {
        try
        {
            auto is_valid{ false };
            auto result{ fn_( std::forward< T >( obj ), is_valid ) };
            if ( is_valid ) { return result; }
            else            { return {};     }
        }
        catch ( std::bad_any_cast const & )
        {
            return {};
        }
    }

private:
    std::function< any_value( std::any, bool & )> fn_;
};

} // namespace booleval::utils

#endif // BOOLEVAL_ANY_MEM_FN_HPP
