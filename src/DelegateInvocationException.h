/*****************************************************************//**
 * \file   DelegateInvocationException.h
 * \brief  The definition of exception for Delegates.
 * 
 * \author DOCtorActoAntohich
 * \date   December 2020
 *********************************************************************/


#ifndef DOC_DELEGATE_INVOCATION_EXCEPTION_H_INCLUDED
#define DOC_DELEGATE_INVOCATION_EXCEPTION_H_INCLUDED

#include <exception>
#include <string>

// Actually nothing to see here.

/// <summary>
/// An exception used when delegate fails to invoke methods when they are not provided.
/// </summary>
class DelegateInvocationException : public std::exception {
public:
	/// <summary>
	/// Generate an exception with given message.
	/// </summary>
	/// <param name="str">A message describing an error.</param>
	DelegateInvocationException(std::string str) : m_message(str) {}

	/// <summary>
	/// See what went wrong.
	/// </summary>
	/// <returns>A message describing an error</returns>
	const char* what() const noexcept {
		return m_message.c_str();
	}

private:
	std::string m_message;
};

#endif // !DOC_DELEGATE_INVOCATION_EXCEPTION_H_INCLUDED