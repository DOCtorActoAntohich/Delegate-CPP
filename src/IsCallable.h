/*****************************************************************//**
 * \file   IsCallable.h
 * \brief  Type trait to determine if an object is callable.
 * 
 * \author DOCtorActoAntohich
 * \date   January 2021
 *********************************************************************/

#ifndef DOC_IS_CALLABLE_H_INCLUDED
#define DOC_IS_CALLABLE_H_INCLUDED

// Found here:
// https://stackoverflow.com/questions/15393938/find-out-whether-a-c-object-is-callable
// Thanks SFINAE.


template<typename T>
class _my_is_callable_impl {
	using success = char(&)[1];
	using fail = char(&)[2];

	struct Fallback { void operator()(); };
	struct Derived : T, Fallback { };

	template<typename U, U> struct Check;

	template<typename>
	static success test(...);

	template<typename C>
	static fail test(Check<void (Fallback::*)(), &C::operator()>*);

public:
	static constexpr bool value = sizeof(test<Derived>(0)) == sizeof(success);
};



/// <summary>
/// Type trait to check if the object is callable (i.e. has operator() defined).
/// </summary>
/// <typeparam name="T">Any class or struct.</typeparam>
template<typename T>
struct is_callable :
	std::conditional<std::is_class_v<T>, _my_is_callable_impl<T>, std::false_type>::type
{ };

/// <summary>
/// An alias for is_callable<T>::value.
/// </summary>
/// <typeparam name="T">Any class or struct.</typeparam>
template<class T>
constexpr bool is_callable_v = is_callable<T>::value;

#endif // !DOC_IS_CALLABLE_H_INCLUDED
