/*****************************************************************//**
 * \file   MemberFunctionsExample.cpp
 * \brief  Example usage of delegate with member functions.
 * 
 * \author DOCtorActoAntohich
 * \date   January 2020
 *********************************************************************/

#include <iostream>
#include "Delegate.h"



// Since C++ treats member functions and other functions as different entities,
// You will have to stick to lambdas or std::bind.

class Foo {
public:
	using MathBinaryOp = Delegate<int(int, int)>;

	int completeOperation(int a, int b) {
		static bool turn = true;

		if (turn) {
			m_operation = std::bind(&Foo::m_add, this, std::placeholders::_1, std::placeholders::_2);
		}
		else {
			m_operation = [this](int a, int b) { return this->m_sub(a, b); };
		}

		turn = !turn;

		return m_operation(a, b);
	}
private:
	MathBinaryOp m_operation;

	int m_add(int a, int b) {
		return a + b;
	}

	int m_sub(int a, int b) {
		return a - b;
	}
};



int main() {
	Foo obj;
	std::cout << obj.completeOperation(5, 7) << std::endl; // 12.
	std::cout << obj.completeOperation(5, 7) << std::endl; // -2

	return 0;
}
