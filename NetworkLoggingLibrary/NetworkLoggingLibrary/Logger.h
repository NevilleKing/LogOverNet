// Logger.h
// Defines functions and macros to be accessed from the code to be debugged

#pragma once

#include <string>
#include "Severity.h"
#include "ClientNetwork.h"
#include "Networking.h"
#include <vector>
#include "Packet.h"
#include <iostream>
#include <sstream>

namespace logovernet
{
	// end of line character (for << overload)
	struct LON_EOL {};

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

		// Overloads
		void sendMessage(int msg, LOG_SEVERITY severity);
		void sendMessage(bool msg, LOG_SEVERITY severity);
		void sendMessage(char msg, LOG_SEVERITY severity);
		void sendMessage(float msg, LOG_SEVERITY severity);
		void sendMessage(wchar_t msg, LOG_SEVERITY severity);

		// Allow user to use << operator
		template <typename T>
		Logger& operator<<(const T str)
		{
			oStream << str;
			return *this;
		}

		template<> Logger& operator<<(const LON_EOL str)
		{
			sendMessage(oStream.str(), LON_INFO);
			oStream.clear();
			return *this;
		}

		// send the value of a variable (including the mem address so
		// we can keep track of the value)
		void sendVariable(std::string variable, std::string memAddr);

	private:
		std::string _ipAddress;
		
		// Client Networking object which calls the Winsock API
		ClientNetwork* network;

		// basic function for sending data
		void sendData(const Packet& packet);

		// to hold current stream when using << operator
		std::stringstream oStream;
	};

	static Logger* GLOBAL_LOGGER = nullptr;
}
