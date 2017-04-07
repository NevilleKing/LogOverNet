// Types.h
// Defines logovernet's types. These are used to automatically log
// any changes to a variable

#pragma once

#include <iostream>

namespace logovernet
{

	template <typename T>
	class LonType : public T
	{
	public:
		template<typename ... Args>
		LonType(Args&& ... args) : T(std::forward<Args>(args) ...)
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
	};

}
