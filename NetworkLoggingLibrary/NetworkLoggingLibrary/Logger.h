// Logger.h
// Defines functions and macros to be accessed from the code to be debugged

#pragma once

#include <string>
#include "Severity.h"
#include "ClientNetwork.h"
#include "Networking.h"
#include <vector>
#include "Packet.h"

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

		// send the value of a variable (including the mem address so
		// we can keep track of the value)
		void sendVariable(std::string variable, std::string memAddr);

	private:
		std::string _ipAddress;
		
		// Client Networking object which calls the Winsock API
		ClientNetwork* network;

		// basic function for sending data
		void sendData(const Packet& packet);
	};

	static Logger* GLOBAL_LOGGER = nullptr;
}
