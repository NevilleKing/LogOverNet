#include "Logger.h"

namespace logovernet
{
	Logger::Logger(std::string ipAddress)
	{
		_ipAddress = ipAddress;
		
		// setup the client network object
		network = new ClientNetwork();
	}

	Logger::~Logger()
	{
		if (network != nullptr)
			delete(network);
	}

	void Logger::sendMessage(std::string msg, LOG_SEVERITY severity)
	{
	}
}
