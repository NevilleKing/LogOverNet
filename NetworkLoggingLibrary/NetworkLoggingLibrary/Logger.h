// Logger.h
// Defines functions and macros to be accessed from the code to be debugged

#ifndef __LOGOVERNET_LOGGER__
#define __LOGOVERNET_LOGGER__

#include <iostream>
#include "Severity.h"

namespace logovernet
{
	// main logging class
	class Logger
	{
	public:
		// constructors
		Logger(std::string ipAddress);
		~Logger() {};
		Logger(const Logger& obj) {};

		// send a message to the ip address with a specific
		// severity level (see Severity.h)
		void sendMessage(std::string msg, LOG_SEVERITY severity);

	private:
		std::string _ipAddress;
	};
}

#endif
