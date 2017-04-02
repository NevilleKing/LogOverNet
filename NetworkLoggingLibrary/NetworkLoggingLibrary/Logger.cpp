#include "Logger.h"

namespace logovernet
{
	Logger::Logger(std::string ipAddress)
	{
		_ipAddress = ipAddress;
		
	}
	void Logger::sendMessage(std::string msg, LOG_SEVERITY severity)
	{
	}
}
