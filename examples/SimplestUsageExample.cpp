/*****************************************************************//**
 * \file   SimplestUsageExample.cpp
 * \brief  Basic example of delegate usage.
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

int multiply(int a, int b) {
	return a * b;
}


int main() {
	Delegate<int(int, int)> operation(add);
	operation.add(sub);
	operation += multiply;

	int someResult = operation(5, 7);
	
	// 35 because the last function's return value is used;
	std::cout << someResult << std::endl;
    
    operation = add; // Override invocation list.
    std::cout << operation(1, 2) << std::endl; // 3.

	return 0;
}
