// Logger.h
// Defines functions and macros to be accessed from the code to be debugged

#pragma once

#include <string>
#include "Severity.h"
#include "ClientNetwork.h"
#include "Networking.h"
#include <vector>

namespace logovernet
{
	// main logging class
	class Logger
	{
	public:
		// constructors
		Logger(std::string ipAddress, std::string portNumber);
		~Logger();
		Logger(const Logger& obj) {};

		// send a message to the ip address with a specific
		// severity level (see Severity.h)
		void sendMessage(std::string msg, LOG_SEVERITY severity);

	private:
		std::string _ipAddress;
		
		// Client Networking object which calls the Winsock API
		ClientNetwork* network;
	};

	static Logger* GLOBAL_LOGGER = nullptr;
}
