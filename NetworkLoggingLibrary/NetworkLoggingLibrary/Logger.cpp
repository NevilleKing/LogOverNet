#include "Logger.h"

namespace logovernet
{
	Logger::Logger(std::string ipAddress, std::string portNumber)
	{
		_ipAddress = ipAddress;
		
		// setup the client network object
		network = new ClientNetwork(ipAddress, portNumber);
	}

	Logger::~Logger()
	{
		if (network != nullptr)
			delete(network);
	}

	void Logger::sendMessage(std::string msg, LOG_SEVERITY severity)
	{
		std::vector<char> cStr(msg.begin(), msg.end());
		// NUL termination for string
		// This will allow us to know where the end of the string is on the other end
		cStr.push_back('\0');

		
		const unsigned int packet_size = cStr.size() * sizeof(char);

		Network::sendMsg(network->ConnectSocket, cStr.data(), packet_size);
	}
}
