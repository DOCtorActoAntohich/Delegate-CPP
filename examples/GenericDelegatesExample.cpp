/*****************************************************************//**
 * \file   GenericDelegatesExample.cpp
 * \brief  Example usage of templated delegates.
 * 
 * \author DOCtorActoAntohich
 * \date   December 2020
 *********************************************************************/

#include <iostream>
#include "Delegate.h"



template<class T>
void print(T t) {
	std::cout << t << " ";
}

class StaticClass {
public:
	template<class T>
	static void print_squared(T t) {
		std::cout << t * t << std::endl;
	}
};



// Delegate can be a member of some object (let it be public here for simplicity).
template<class T>
class SomeObject {
public:
	Delegate<void(T)> printer = { print<T>, StaticClass::print_squared<T> };
};

// Delegate can be some global variable (same as within a class).
template<class T>
Delegate<void(T)> member = print<T>;



int main() {
	auto obj = SomeObject<int>();
	obj.printer(5); // 5 25

	member<float> += StaticClass::print_squared<float>; 
	member<float>.invoke(7); // 7 49

	member<char>.invoke('a');
	std::cout << std::endl; // 'a'

	return 0;
}
