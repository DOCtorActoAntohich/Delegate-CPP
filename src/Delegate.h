/*****************************************************************//**
 * \file   Delegate.h
 * \brief  Definition of Delegate object.
 * 
 * \author DOCtorActoAntohich
 * \date   December 2020
 *********************************************************************/

#ifndef DOC_DELEGATE_H_INCLUDED
#define DOC_DELEGATE_H_INCLUDED

#include <vector>
#include <functional>

#include "DelegateInvocationException.h"

// Undefined.
template<class _Func> class Delegate;
// ^ Needed for the template specialization bellow.



/// <summary>
/// Represents a delegate from C#.
/// Must accept template parameter in the same format as std::function.
/// </summary>
/// <typeparam name="_Rt">Return value type.</typeparam>
/// <typeparam name="..._Args">Input parameters types.</typeparam>
template<class _Rt, class ..._Args>
class Delegate<_Rt(_Args...)> {

#pragma region Member Types
public:
	using return_type = _Rt;
	using function_type = _Rt(_Args...);
	using std_function_type = std::function<function_type>;
	using c_function_type = return_type(*)(_Args...);
private:
	using _delegate = Delegate<function_type>;
#pragma /* Member Types */ endregion

public:
#pragma region Constructors

	/// <summary>
	/// Constructs an empty object.
	/// </summary>
	Delegate() : m_invokationList()
	{	}

	/// <summary>
	/// Constructs an object with a given function.
	/// </summary>
	/// <param name="func">A function that can be invoked.</param>
	Delegate(function_type func) : Delegate() {
		this->add(func);
	}

	/// <summary>
	/// Constructs an object from a list of given functions.
	/// </summary>
	/// <param name="list">A list of functions to use</param>
	Delegate(std::initializer_list<std_function_type> list) : Delegate() {
		for (std_function_type func : list) {
			m_invokationList.push_back(func);
		}
	}

	/// <summary>
	/// Copies the invocation list from other delegate.
	/// </summary>
	/// <param name="copy">A delegate to copy from.</param>
	Delegate(const _delegate& copy) {
		this->m_invokationList = copy.m_invokationList;
	}

	/// <summary>
	/// Moves the invocation list from other delegate.
	/// </summary>
	/// <param name="other">A delegate to move from.</param>
	/// <returns></returns>
	Delegate(_delegate&& other) noexcept {
		this->m_invokationList = std::move(other.m_invokationList);
	}

#pragma /* Constructors */ endregion



#pragma region Methods

	/// <summary>
	/// Checks if the delegate has any function to invoke.
	/// </summary>
	/// <returns>true if there are no functions to invoke, false otherwise.</returns>
	bool is_null() const noexcept {
		return m_invokationList.empty();
	}



	/// <summary>
	/// Adds a function to invocation list.
	/// Alternatively, use += operator.
	/// Chain calls are allowed.
	/// </summary>
	/// <param name="func">A function to add.</param>
	/// <returns>An updated delegate.</returns>
	_delegate& add(function_type func) {
		m_invokationList.push_back(func);
		return *this;
	}

	/// <summary>
	/// Adds all functions from invocation list to a current delegate.
	/// Alternatively, use += operator.
	/// Chain calls are allowed.
	/// </summary>
	/// <param name="other">Another delegate to get functions from.</param>
	/// <returns>An updated delegate.</returns>
	_delegate& add(const _delegate& other) {
		for (std_function_type func : other.m_invokationList) {
			m_invokationList.push_back(func);
		}
		return *this;
	}

	/// <summary>
	/// Removes the last occurrence of a function from invocation list.
	/// If there is no such function, there will be no effect.
	/// Alternatively, use -= operator.
	/// Chain calls are allowed.
	/// </summary>
	/// <param name="func">A function to remove.</param>
	/// <returns>An updated delegate.</returns>
	_delegate& remove(function_type func) noexcept {
		if (this->is_null()) {
			return *this;
		}

		for (uint32_t i = 0; i < m_invokationList.size(); ++i) {
			uint32_t index = m_invokationList.size() - i - 1;

			c_function_type* func_ptr = m_invokationList[index].target<c_function_type>();
			if (func_ptr != nullptr && *func_ptr == func) {
				m_invokationList.erase(m_invokationList.begin() + index);
				break;
			}
		}
		return *this;
	}

	/// <summary>
	/// Removes all methods from invocation list.
	/// Chain calls are allowed.
	/// </summary>
	/// <returns>An updated delegate.</returns>
	_delegate& clear() {
		m_invokationList.clear();
		return *this;
	}

	/// <summary>
	/// Calls all functions in invocation list in the order they were provided.
	/// If functions return non-void, the last function's result is returned.
	/// If no functions provided, DelegateInvocationException is thrown.
	/// Alternatively, use operator().
	/// </summary>
	/// <param name="...params">Pack of parameters to function(s).</param>
	/// <returns>The value of last called function or void.</returns>
	return_type invoke(_Args... params) {
		if (this->is_null()) {
			throw DelegateInvocationException("Failed to invoke delegate");
		}

		if constexpr (std::is_same<void, return_type>::value) {
			this->invoke_void(params...);
			return;
		}
		return this->invoke_nonvoid(params...);
	}

#pragma /* Methods */ endregion



#pragma region Overloaded Operators

	/// <summary>
	/// Copy assignment operator.
	/// </summary>
	/// <param name="copy">A delegate to copy from.</param>
	/// <returns>Left operand.</returns>
	_delegate& operator=(const _delegate& copy) noexcept {
		this->m_invokationList = copy.m_invokationList;
		return *this;
	}

	/// <summary>
	/// Replaces current invocation list with the new one containing only a given function.
	/// </summary>
	/// <param name="func">New function.</param>
	/// <returns>Left operand.</returns>
	_delegate& operator=(function_type& func) {
		m_invokationList.clear();
		m_invokationList.push_back(func);
		return *this;
	}

	/// <summary>
	/// Replaces current invocation list with the new one containing only a given std::function.
	/// </summary>
	/// <param name="func">New function.</param>
	/// <returns>Left operand.</returns>
	_delegate& operator=(std_function_type& func) {
		m_invokationList.clear();
		m_invokationList.push_back(func);
		return *this;
	}

	/// <summary>
	/// Move assignment operator.
	/// </summary>
	/// <param name="other">A delegate to move from.</param>
	/// <returns>Left operand.</returns>
	_delegate& operator=(_delegate&& other) noexcept {
		if (this != &other) {
			this->m_invokationList = std::move(other.m_invokationList);
		}
		return *this;
	}

	/// <summary>
	/// Adds a function to invocation list.
	/// Alternatively, use add() method.
	/// </summary>
	/// <param name="func">A function to add.</param>
	/// <returns>Left operand.</returns>
	_delegate& operator+=(function_type func) {
		return this->add(func);
	}

	/// <summary>
	/// Adds all functions from invocation list to a current delegate.
	/// Alternatively, use add() method.
	/// </summary>
	/// <param name="other">Another delegate to get functions from.</param>
	/// <returns>Left operand.</returns>
	_delegate& operator+=(_delegate& other) {
		return this->add(other);
	}

	/// <summary>
	/// Removes the last occurrence of a function from invocation list.
	/// If there is no such function, there will be no effect.
	/// Alternatively, use remove() method.
	/// </summary>
	/// <param name="func">A function to remove.</param>
	/// <returns>Left operand.</returns>
	_delegate& operator-=(function_type func) {
		return this->remove(func);
	}

	/// <summary>
	/// Adds two delegates by concatenation of their invocation lists.
	/// </summary>
	/// <param name="other">Right operand</param>
	/// <returns>A new delegate with combined invocation list.</returns>
	_delegate operator+(_delegate other) {
		_delegate result;
		result.add(*this).add(other);
		return result;
	}

	/// <summary>
	/// Calls all functions in invocation list in the order they were provided.
	/// If functions return non-void, the last function's result is returned.
	/// If no functions provided, DelegateInvocationException is thrown.
	/// Alternatively, use invoke() method.
	/// </summary>
	/// <param name="...params">Pack of parameters to function(s).</param>
	/// <returns>The value of last called function or void.</returns>
	return_type operator()(_Args... params) {
		return this->invoke(params...);
	}

#pragma /* Overloaded Operators */ endregion



private:
	std::vector<std_function_type> m_invokationList;


#pragma region Invocation

	// Executes all functions in invocation list if return type is void.
	void invoke_void(_Args... params) {
		for (uint32_t i = 0; i < m_invokationList.size(); ++i) {
			m_invokationList[i](params...);
		}
	}

	// Executes all functions in invocation list if return type isn't void.
	return_type invoke_nonvoid(_Args... params) {
		for (uint32_t i = 0; i < m_invokationList.size() - 1; ++i) {
			m_invokationList[i](params...);
		}
		uint32_t last = m_invokationList.size() - 1;
		return m_invokationList[last](params...);
	}

#pragma /* Invocation */ endregion
};

#endif // !DOC_DELEGATE_H_INCLUDED