// Types.h
// Defines logovernet's types. These are used to automatically log
// any changes to a variable

#pragma once

#include <iostream>
#include <type_traits>
#include <sstream>

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
			LogMessage("Constructed. Initial value:");
		}

		LonType() : T()
		{
			LogMessage("Constructed. Initial value:");
		}

		// copy constructor
		LonType(const LonType& obj) : T(obj)
		{
			LogMessage("Copied", false);
		}

		// destructor
		~LonType()
		{
			LogMessage("Destroyed", false);
		}

		// Overloaded Operators ===

		// Assignment
		LonType& operator=(const LonType & other)
		{
			T::operator=( other );
			LogMessage("New value assigned. New value:");
			return *this;
		}

		// Increment
		LonType& operator++()
		{
			T::operator++();
			LogMessage("++ operator called. New value:");
			return *this;
		}
		LonType operator++(int)
		{
			T::operator++(0);
			LogMessage("++ operator called. New value:");
			return *this;
		}

		// Decrement
		LonType& operator--()
		{
			T::operator--();
			LogMessage("-- operator called. New value:");
			return *this;
		}
		LonType operator--(int)
		{
			T::operator--(0);
			LogMessage("-- operator called. New value:");
			return *this;
		}

		// Compound assignment operators
		LonType& operator+=(const LonType & other)
		{
			T::operator+=(other);
			LogMessage("+= operator called. New value:");
			return *this;
		}
		LonType& operator-=(const LonType & other)
		{
			T::operator-=(other);
			LogMessage("-= operator called. New value:");
			return *this;
		}
		LonType& operator*=(const LonType & other)
		{
			T::operator*=(other);
			LogMessage("*= operator called. New value:");
			return *this;
		}
		LonType& operator/=(const LonType & other)
		{
			T::operator/=(other);
			LogMessage("/= operator called. New value:");
			return *this;
		}
		LonType& operator%=(const LonType & other)
		{
			T::operator%=(other);
			LogMessage("%= operator called. New value:");
			return *this;
		}
		LonType& operator&=(const LonType & other)
		{
			T::operator&=(other);
			LogMessage("&= operator called. New value:");
			return *this;
		}
		LonType& operator|=(const LonType & other)
		{
			T::operator|=(other);
			LogMessage("|= operator called. New value:");
			return *this;
		}
		LonType& operator^=(const LonType & other)
		{
			T::operator^=(other);
			LogMessage("^= operator called. New value:");
			return *this;
		}
		LonType& operator<<=(const LonType & other)
		{
			T::operator<<=(other);
			LogMessage("<<== operator called. New value:");
			return *this;
		}
		LonType& operator>>=(const LonType & other)
		{
			T::operator>>=(other);
			LogMessage(">>= operator called. New value:");
			return *this;
		}

	private:
		// log message to the GLOBAL_LOGGER
		void LogMessage(std::string msg, bool addValue = true)
		{
			if (GLOBAL_LOGGER != nullptr)
			{
				std::stringstream ss;
				ss << "[" << this << "] " << msg;
				if (addValue)
					ss << " " << *this;
				GLOBAL_LOGGER->sendMessage(ss.str(), LOG_SEVERITY::LON_INFO);
			}
		}
	};

	template <typename L>
	class LonBasicType
	{
	private:
		L _val;
	public:
		LonBasicType() {};
		LonBasicType(L val) : _val(val) {};

		// casting
		operator L() const { return _val; }

		// operator overloads
		LonBasicType& operator=(const LonBasicType & other)
		{
			_val = other._val;
			return *this;
		}
		LonBasicType operator+(const LonBasicType & other)
		{
			LonBasicType temp(other);
			temp += other._val;
			return temp;
		}
		LonBasicType operator-(const LonBasicType & other)
		{
			LonBasicType temp(other);
			temp -= other._val;
			return temp;
		}
		LonBasicType operator*(const LonBasicType & other)
		{
			LonBasicType temp(other);
			temp *= other._val;
			return temp;
		}
		LonBasicType operator/(const LonBasicType & other)
		{
			LonBasicType temp(other);
			temp /= other._val;
			return temp;
		}
		LonBasicType operator%(const LonBasicType & other)
		{
			LonBasicType temp(other);
			temp %= other._val;
			return temp;
		}
		LonBasicType& operator++()
		{
			++_val;
			return *this;
		}
		LonBasicType operator++(int)
		{
			LonBasicType temp(_val);
			++_val;
			return  temp;
		}
		LonBasicType& operator--()
		{
			--_val;
			return *this;
		}
		LonBasicType operator--(int)
		{
			LonBasicType temp(_val);
			--_val;
			return  temp;
		}
		LonBasicType operator&&(const LonBasicType & other)
		{
			LonBasicType temp(_val);
			temp._val = temp._val && other._val;
			return temp;
		}
		LonBasicType operator||(const LonBasicType & other)
		{
			LonBasicType temp(_val);
			temp._val = temp._val || other._val;
			return temp;
		}
		LonBasicType operator~()
		{
			LonBasicType temp(_val);
			temp._val = ~temp._val;
			return temp;
		}
		LonBasicType operator&(const LonBasicType & other)
		{
			LonBasicType temp(_val);
			temp._val = temp._val & other._val;
			return temp;
		}
		LonBasicType operator|(const LonBasicType & other)
		{
			LonBasicType temp(_val);
			temp._val = temp._val | other._val;
			return temp;
		}
		LonBasicType operator^(const LonBasicType & other)
		{
			LonBasicType temp(_val);
			temp._val = temp._val ^ other._val;
			return temp;
		}
		LonBasicType operator<<(const LonBasicType & other)
		{
			LonBasicType temp(_val);
			temp._val = temp._val << other._val;
			return temp;
		}
		LonBasicType operator>>(const LonBasicType & other)
		{
			LonBasicType temp(_val);
			temp._val = temp._val >> other._val;
			return temp;
		}
		LonBasicType& operator+=(const LonBasicType & other)
		{
			_val += other._val;
			return *this;
		}
		LonBasicType& operator-=(const LonBasicType & other)
		{
			_val -= other._val;
			return *this;
		}
		LonBasicType& operator*=(const LonBasicType & other)
		{
			_val *= other._val;
			return *this;
		}
		LonBasicType& operator/=(const LonBasicType & other)
		{
			_val /= other._val;
			return *this;
		}
		LonBasicType& operator%=(const LonBasicType & other)
		{
			_val %= other._val;
			return *this;
		}
		LonBasicType& operator&=(const LonBasicType & other)
		{
			_val &= other._val;
			return *this;
		}
		LonBasicType& operator|=(const LonBasicType & other)
		{
			_val |= other._val;
			return *this;
		}
		LonBasicType& operator^=(const LonBasicType & other)
		{
			_val ^= other._val;
			return *this;
		}
		LonBasicType& operator<<=(const LonBasicType & other)
		{
			_val <<= other._val;
			return *this;
		}
		LonBasicType& operator>>=(const LonBasicType & other)
		{
			_val >>= other._val;
			return *this;
		}
	};

	// typedefs for basic types
	typedef LonType<LonBasicType<bool>>    lon_bool;
	typedef LonType<LonBasicType<char>>    lon_char;
	typedef LonType<LonBasicType<int>>     lon_int;
	typedef LonType<LonBasicType<float>>   lon_float;
	typedef LonType<LonBasicType<double>>  lon_double;
	typedef LonType<LonBasicType<wchar_t>> lon_wchar_t;

}
