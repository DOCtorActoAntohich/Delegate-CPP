/*****************************************************************//**
 * \file   SimplestUsageExample.cpp
 * \brief  Example usage of simplest delegates
 *
 * \author DOCtorActoAntohich
 * \date   December 2020
 *********************************************************************/

#include <iostream>
#include "Delegate.h"


int sum(int a, int b) {
	return a + b;
}

int sub(int a, int b) {
	return a - b;
}

struct multiplier {
	int operator()(int a, int b) {
		return a * b;
	}
};


int main() {
	// Ways to create the delegate and add callable objects to invocation list.
	// Removal is done in the similar way.
	Delegate<int(int, int)> operation(sum);
	operation.add(sub).add(sum);
	operation += multiplier();

	// Only the last return value is used.
	int someResult = operation(5, 7);
	std::cout << someResult << std::endl; // 35

	// Override invocation list.
	operation = sum;
	std::cout << operation(1, 2) << std::endl; // 3.

	return 0;
}
