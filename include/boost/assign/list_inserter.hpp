// Boost.Assign library
//
//  Copyright Thorsten Ottosen 2003-2004. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see http://www.boost.org/libs/assign/
//

#ifndef BOOST_ASSIGN_LIST_INSERTER_HPP
#define BOOST_ASSIGN_LIST_INSERTER_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_void.hpp>
#include <cstddef>

//###
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/iteration/local.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>

namespace boost
{
namespace assign_detail
{
    template< class T >
    struct repeater
    {
        std::size_t  sz;
        T            val;

        repeater( std::size_t sz, T r ) : sz( sz ), val( r )
        { }
    };
    
    template< class Fun >
    struct fun_repeater
    {
        std::size_t  sz;
        Fun          val;
        fun_repeater( std::size_t sz, Fun r ) : sz( sz ), val( r )
        { }
    };
    
    /*
    template< class T >
    struct from_to_repeater
    {
        T from, to;
        from_to_repeater( T from, T to ) : from( from ), to( to )
        { }
    };
    
    template< class T >
    struct from_to_increment_repeater
    {
        T from, to, incr;
        from_to_increment_repeater( T from, T to, T incr )
         : from( from ), to( to ), incr( incr )
        { }
    };
    */

    template< class C >
    class call_push_back
    {
        C& c_;
    public:
        call_push_back( C& c ) : c_( c )
        { }
        
        template< class T >
        void operator()( T r ) 
        {
            c_.push_back( r );
        }
    };
    
    template< class C >
    class call_push_front
    {
        C& c_;
    public:
        call_push_front( C& c ) : c_( c )
        { }
        
        template< class T >
        void operator()( T r ) 
        {
            c_.push_front( r );
        }
    };
    
    template< class C >
    class call_push
    {
        C& c_;
    public:
        call_push( C& c ) : c_( c )
        { }
    
        template< class T >
        void operator()( T r ) 
        {
            c_.push( r );
        }
    };
    
    template< class C >
    class call_insert
    {
        C& c_;
    public:
        call_insert( C& c ) : c_( c )
        { }
    
        template< class T >
        void operator()( T r ) 
        {
            c_.insert( r );
        }
    };

} // namespace 'assign_detail'

namespace assign
{

    template< class T >
    inline assign_detail::repeater<T>
    repeat( std::size_t sz, T r )
    {
        return assign_detail::repeater<T>( sz, r );
    }
    
    template< class Function >
    inline assign_detail::fun_repeater<Function>
    repeat_fun( std::size_t sz, Function r )
    {
        return assign_detail::fun_repeater<Function>( sz, r );
    }
    
    /*
    template< class T >
    inline detail::from_to_repeater<T>
    repeat_from_to( T from, T to )
    {
        return detail::from_to_repeater<T>( from, to );
    }

    template< class T >
    inline detail::from_to_increment_repeater<T>
    repeat_from_to( T from, T to, T increment )
    {
        return detail::from_to_increment_repeater<T>( from, to, increment );
    }
    */
    
    
    
    template< class Function, class Argument = void > 
    class list_inserter
    {
        struct single_arg_type {};
        struct n_arg_type      {};

        typedef typename mpl::if_c< is_void<Argument>::value,
                                    n_arg_type,
                                    single_arg_type >::type arg_type;  
            
    public:
        
        explicit list_inserter( Function fun ) : insert_( fun )
        {}
        
        template< class Function2, class Arg >
        list_inserter( list_inserter<Function2,Arg> r ) 
        : insert_( r.fun_private() ) 
        {}

        list_inserter& operator()()
        {
            insert_( Argument() );
            return *this;
        }
        
        template< class T >
        list_inserter& operator=( T r )
        {
            insert_( r );
            return *this;
        }
        
        template< template <class> class Repeater, class T >
        list_inserter& operator=( Repeater<T> r )
        {
            return operator,( r );
        }
        
        template< class T >
        list_inserter& operator,( T r )
        {
            insert_( r  );
            return *this;
        }

        template< class T >
        list_inserter& operator,( assign_detail::repeater<T> r )
        {
            return repeat( r.sz, r.val ); 
        }
        
        template< class Nullary_function >
        list_inserter& operator,( assign_detail::fun_repeater<Nullary_function> r )
        {
            return repeat_fun( r.sz, r.val ); 
        }

        /*
        template< class T >
        list_inserter& operator,( detail::from_to_repeater<T> r )
        {
            return repeat_from_to( r.from, r.to ); 
        }

        template< class T >
        list_inserter& operator,( detail::from_to_increment_repeater<T> r )
        {
            return repeat_from_to( r.from, r.to, r.incr ); 
        }
        */

        template< class T >
        list_inserter& repeat( std::size_t sz, T r )
        {
            std::size_t i = 0;
            while( i++ != sz )
                insert_( r );
            return *this;
        }
        
        template< class Nullary_function >
        list_inserter& repeat_fun( std::size_t sz, Nullary_function fun )
        {
            std::size_t i = 0;
            while( i++ != sz )
                insert_( fun() );
            return *this;
        }
        
        /*
        template< class T >
        list_inserter& repeat_from_to( T from, T to )
        {
            while( from < to )
            {
                insert_( from );
                ++from;
            }
            return *this;
        }
        
        template< class T >
        list_inserter& repeat_from_to( T from, T to, T increment )
        {
            while( from < to )
            {
                insert_( from );
                from += increment;
            }
            return *this;
        }
        */
        
        template< class T >
        list_inserter& operator()( T t )
        {
            insert_( t );
            return *this;
        }

//###
#ifndef BOOST_ASSIGNMENT_MAX_PARAMS // use user's value
#define BOOST_ASSIGNMENT_MAX_PARAMS 4        
#endif
#define BOOST_ASSIGNMENT_PARAMS1(n) BOOST_PP_ENUM_PARAMS(n, class T)
#define BOOST_ASSIGNMENT_PARAMS2(n) BOOST_PP_ENUM_BINARY_PARAMS(n, T, t)
#define BOOST_ASSIGNMENT_PARAMS3(n) BOOST_PP_ENUM_PARAMS(n, t)
        
#define BOOST_PP_LOCAL_LIMITS (1, BOOST_ASSIGNMENT_MAX_PARAMS)
#define BOOST_PP_LOCAL_MACRO(n) \
    template< class T, BOOST_ASSIGNMENT_PARAMS1(n) > \
    list_inserter& operator()(T t, BOOST_ASSIGNMENT_PARAMS2(n) ) \
        { \
            BOOST_PP_CAT(insert, BOOST_PP_INC(n))(t, BOOST_ASSIGNMENT_PARAMS3(n), arg_type()); \
            return *this; \
        } \
        /**/

#include BOOST_PP_LOCAL_ITERATE()
        

#define BOOST_PP_LOCAL_LIMITS (1, BOOST_ASSIGNMENT_MAX_PARAMS)
#define BOOST_PP_LOCAL_MACRO(n) \
    template< class T, BOOST_ASSIGNMENT_PARAMS1(n) > \
    void BOOST_PP_CAT(insert, BOOST_PP_INC(n))(T t, BOOST_ASSIGNMENT_PARAMS2(n), single_arg_type) \
    { \
        insert_( Argument(t, BOOST_ASSIGNMENT_PARAMS3(n) )); \
    } \
    /**/
        
#include BOOST_PP_LOCAL_ITERATE()

#define BOOST_PP_LOCAL_LIMITS (1, BOOST_ASSIGNMENT_MAX_PARAMS)
#define BOOST_PP_LOCAL_MACRO(n) \
    template< class T, BOOST_ASSIGNMENT_PARAMS1(n) > \
    void BOOST_PP_CAT(insert, BOOST_PP_INC(n))(T t, BOOST_ASSIGNMENT_PARAMS2(n), n_arg_type) \
    { \
        insert_(t, BOOST_ASSIGNMENT_PARAMS3(n) ); \
    } \
    /**/
        
#include BOOST_PP_LOCAL_ITERATE()

        
        Function fun_private() const
        {
            return insert_;
        }

    private:
        
        list_inserter& operator=( const list_inserter& );
        Function insert_;
    };
    
    template< class Function >
    inline list_inserter< Function >
    make_list_inserter( Function fun )
    {
        return list_inserter< Function >( fun );
    }
    
    template< class C >
    inline list_inserter< assign_detail::call_push_back<C>, 
                          BOOST_DEDUCED_TYPENAME C::value_type >
    push_back( C& c )
    {
        return make_list_inserter( assign_detail::call_push_back<C>( c ) );
    }
    
    template< class C >
    inline list_inserter< assign_detail::call_push_front<C>,
                          BOOST_DEDUCED_TYPENAME C::value_type >
    push_front( C& c )
    {
        return make_list_inserter( assign_detail::call_push_front<C>( c ) );
    }

    template< class C >
    inline list_inserter< assign_detail::call_insert<C>, 
                          BOOST_DEDUCED_TYPENAME C::value_type >
    insert( C& c )
    {
        return make_list_inserter( assign_detail::call_insert<C>( c ) );
    }

    template< class C >
    inline list_inserter< assign_detail::call_push<C>, 
                          BOOST_DEDUCED_TYPENAME C::value_type >
    push( C& c )
    {
        return make_list_inserter( assign_detail::call_push<C>( c ) );
    }
    
} // namespace 'assign'
} // namespace 'boost'

//###
#undef BOOST_ASSIGNMENT_PARAMS1
#undef BOOST_ASSIGNMENT_PARAMS2
#undef BOOST_ASSIGNMENT_PARAMS3
#undef BOOST_ASSIGNMENT_MAX_PARAMS

#endif