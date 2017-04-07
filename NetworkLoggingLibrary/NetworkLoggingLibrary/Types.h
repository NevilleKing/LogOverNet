// Types.h
// Defines logovernet's types. These are used to automatically log
// any changes to a variable

#pragma once

#include <iostream>
#include <type_traits>

namespace logovernet
{
	// helper for copy constructor below
	template<bool B, typename T = void> using disable_if = std::enable_if<!B, T>;

	template <typename T>
	class LonType : public T
	{
	public:
		// ref: http://stackoverflow.com/questions/13937873/how-can-i-prevent-a-variadic-constructor-from-being-preferred-to-the-copy-constr
		// Disable the main constructor if the first argument is of 'LonType'
		// This means the copy constructor is properly called below
		template<typename Arg, typename ... Args, typename = typename
			disable_if<
				sizeof...(Args) == 0 && // no other arguments except the first
				std::is_same<typename
					std::remove_reference<Arg>::type,
					LonType
				>::value
			>::type
		>
		LonType(Arg&& arg, Args&& ... args) : T(std::forward<Arg>(arg), std::forward<Args>(args) ...)
		{
			std::cout << "created" << std::endl;
		}

		// copy constructor
		LonType(const LonType& obj) : T(obj)
		{
			std::cout << "copied" << std::endl;
		}

		// destructor
		~LonType()
		{
			std::cout << "destroyed" << std::endl;
		}

		// Overloaded Operators ===

		// Assignment
		LonType& operator=(const LonType & other)
		{
			T::operator=( other );
			std::cout << "assignment operator" << std::endl;
			return *this;
		}

		// Increment
		LonType& operator++()
		{
			T::operator++();
			std::cout << "increment operator" << std::endl;
			return *this;
		}
		LonType operator++(int)
		{
			T::operator++(int);
			std::cout << "increment operator" << std::endl;
			return *this;
		}

		// Decrement
		LonType& operator--()
		{
			T::operator--();
			std::cout << "decrement operator" << std::endl;
			return *this;
		}
		LonType operator--(int)
		{
			T::operator--(int);
			std::cout << "decrement operator" << std::endl;
			return *this;
		}

		// Compound assignment operators
		LonType& operator+=(const LonType & other)
		{
			T::operator+=(other);
			std::cout << "+= operator" << std::endl;
			return *this;
		}
		LonType& operator-=(const LonType & other)
		{
			T::operator-=(other);
			std::cout << "-= operator" << std::endl;
			return *this;
		}
		LonType& operator*=(const LonType & other)
		{
			T::operator*=(other);
			std::cout << "*= operator" << std::endl;
			return *this;
		}
		LonType& operator/=(const LonType & other)
		{
			T::operator/=(other);
			std::cout << "/= operator" << std::endl;
			return *this;
		}
		LonType& operator%=(const LonType & other)
		{
			T::operator%=(other);
			std::cout << "%= operator" << std::endl;
			return *this;
		}
		LonType& operator&=(const LonType & other)
		{
			T::operator&=(other);
			std::cout << "&= operator" << std::endl;
			return *this;
		}
		LonType& operator|=(const LonType & other)
		{
			T::operator|=(other);
			std::cout << "|= operator" << std::endl;
			return *this;
		}
		LonType& operator^=(const LonType & other)
		{
			T::operator^=(other);
			std::cout << "^= operator" << std::endl;
			return *this;
		}
		LonType& operator<<=(const LonType & other)
		{
			T::operator<==(other);
			std::cout << "<== operator" << std::endl;
			return *this;
		}
		LonType& operator>>=(const LonType & other)
		{
			T::operator>>=(other);
			std::cout << ">>= operator" << std::endl;
			return *this;
		}
	};

}
