/*
 * ct_array.hpp
 *
 *  Created on: Aug 21, 2014
 *      Author: Pietro Incardona
 */



/*! \brief These set of classes generate an array definition at compile-time
 *
 * These set of classes generate an array definition at compile-time
 *
 * \see generate_array
 *
 */

#ifndef CT_ARRAY_HPP_
#define CT_ARRAY_HPP_


#include <boost/fusion/mpl.hpp>

/////////////////////////////////////////////////// Make indexes ///////////////////////

template<int ...> struct index_tuple{};

//! the array itself
template<class T, int... args> struct ArrayHolder_indexes {
	//! convert variadic into a tuple struct containing the list
    typedef index_tuple<args ... > type;
};

//! generate compile time index array
template<class T,long int N, size_t orig_N, template<size_t,size_t> class F, int... args>
struct generate_indexes_impl {
	//! Metafunction to generate indexes
    typedef typename generate_indexes_impl<T,N-1,orig_N, F, F<N,orig_N>::value, args...>::result result;
};

//! terminator of the variadic template
template<class T, size_t orig_N, template<size_t,size_t> class F, int... args>
struct generate_indexes_impl<T,0,orig_N, F, args...> {
	//! generate the compile-time array
    typedef typename ArrayHolder_indexes<T,F<0,orig_N>::value, args...>::type result;
};

//! In case of an empty list
template<class T, size_t orig_N, template<size_t,size_t> class F, int... args>
struct generate_indexes_impl<T,-1,orig_N, F, args...> {
	//! empty array
    typedef index_tuple<> result;
};

/*! \brief Main class to generate indexes data structure
 *
 *
 * ### Meta-function definition
 * \snippet util_test.hpp Metafunction definition
 * ### Usage
 * \snippet util_test.hpp indexes array
 *
 * \param T is the type of the output array
 * \param N size of the sequence
 * \param F Meta function it take two template arguments
 *
 */
template<class T, long int N, template<size_t,size_t> class F>
struct generate_indexes {
	//! generate compile time array
    typedef typename generate_indexes_impl<T,N-1, N, F>::result result;
};

///////////////////////////////////////////////////

#ifndef COVERTY_SCAN

//! the array itself
template<class T, unsigned long... args> struct ArrayHolder_constexpr {
	//! compile-time array
    constexpr static T data[sizeof...(args)] = { args... };
};


//! recursive meta-function to generate compile-time array
template<class T,size_t N, size_t orig_N, template<size_t,size_t> class F, unsigned... args>
struct generate_array_constexpr_impl {
	//! recursive meta-function to generate compile-time array
    typedef typename generate_array_constexpr_impl<T,N-1,orig_N, F, F<N,orig_N>::value, args...>::result result;
};

//! terminator of the variadic template
template<class T, size_t orig_N, template<size_t,size_t> class F, unsigned... args>
struct generate_array_constexpr_impl<T,0,orig_N, F, args...> {
	//! Compile-time array
    typedef ArrayHolder_constexpr<T,F<0,orig_N>::value, args...> result;
};

/*! \brief Main class to generate constexpr compile-time array
 *
 *
 * A constexpr compile time array is for example
 *
 * \code{.cpp}
 * constexpr size_t array[5] = {1,2,3,4,5}
 * \endcode
 *
 *
 * ### Metafunction definition
 * \snippet util_test.hpp Metafunction definition
 * ### Usage
 * \snippet util_test.hpp constexpr array
 *
 * \param T is the type ot the output array
 * \param N size of the sequence
 * \param F Meta function it take two template arguments
 *
 */
template<class T, size_t N, template<size_t,size_t> class F>
struct generate_array_constexpr {
	//! meta-function to generate compile time array
    typedef typename generate_array_constexpr_impl<T,N-1, N, F>::result result;
};

#endif

//////////////////////////////////////////////////

//! the array itself
template<class T, unsigned long... args> struct ArrayHolder {
    static const T data[sizeof...(args)];
};

//! initialize the array from variadic template
template<class T, unsigned long... args>
const T ArrayHolder<T,args...>::data[sizeof...(args)] = { args... };

//! Generate the array specializing ArrayHolder
template<class T,size_t N, size_t orig_N, template<size_t,size_t> class F, unsigned... args>
struct generate_array_impl {
	//! recursive meta-function to generate compile-time array
    typedef typename generate_array_impl<T,N-1,orig_N, F, F<N,orig_N>::value, args...>::result result;
};

//! terminator of the variadic template
template<class T, size_t orig_N, template<size_t,size_t> class F, unsigned... args>
struct generate_array_impl<T,0,orig_N, F, args...> {
	//! compile-time array
    typedef ArrayHolder<T,F<0,orig_N>::value, args...> result;
};

/*! \brief Main class to generate compile-time array
 *
 * A compile time array is for example
 *
 * \code{.cpp}
 * const size_t array[5] = {1,2,3,4,5}
 * \endcode
 *
 * ### Metafunction definition
 * \snippet util_test.hpp Metafunction definition
 * ### Usage
 * \snippet util_test.hpp compile time array
 *
 * \param T is the type ot the output array
 * \param N size of the sequence
 * \param F Meta function it take two template arguments
 *
 */
template<class T, size_t N, template<size_t,size_t> class F>
struct generate_array {
    typedef typename generate_array_impl<T,N-1, N, F>::result result;
};

/////////////// Classes to generate at compile time arrays from a boost::mpl::vector

//! Generate the array specializing ArrayHolder
template<class T, size_t N ,class F, unsigned... args>
struct generate_array_vector_impl {
	//! recursive meta-function to generate compile-time array
    typedef typename generate_array_vector_impl<T,N-1,F, boost::mpl::at<F,boost::mpl::int_<N> >::type::value , args...>::result result;
};

//! terminator of the variadic template
template<class T, class F, unsigned... args>
struct generate_array_vector_impl<T,0, F, args...> {
	//! compile-time array generation
    typedef ArrayHolder<T,boost::mpl::at<F,boost::mpl::int_<0> >::type::value, args...> result;
};

/*! \brief Main class to generate an array from a boost::mpl::vector of numbers
 *
 * Main class to generate an array from a boost::mpl::vector of numbers
 *
 * Usage:
 *
 * boost::mpl::vector<int_<4>,int_<4>,......> v
 *
 * typdef generate_array_vector<size_t,v>::result B;
 *
 * B is an array or size_t of two element {4,4}
 *
 * \param T type of the output buffer
 * \param F boost::mpl::vector
 *
 */
template<class T, class F>
struct generate_array_vector {
	//! generate compile-time array vector
    typedef typename generate_array_vector_impl<T,boost::mpl::size<F>::value-1 , F>::result result;
};

#endif
