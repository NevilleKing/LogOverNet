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
		Packet p = { severity, "", msg };
		sendData(p);
	}

	void Logger::sendVariable(std::string variable, std::string memAddr)
	{
		Packet p = { LOG_SEVERITY::LON_INFO, memAddr, variable };
		sendData(p);
	}

	void Logger::sendData(const Packet & packet)
	{
		std::vector<char> cStr;
		cStr.push_back(packet.severity);
		cStr.insert(cStr.end(), packet.memory_address.begin(), packet.memory_address.end());
		// NUL termination for string
		// This will allow us to know where the end of the string is on the other end
		cStr.push_back('\0');
		cStr.insert(cStr.end(), packet.data.begin(), packet.data.end());
		cStr.push_back('\0');

		const unsigned int packet_size = cStr.size() * sizeof(char);

		Network::sendMsg(network->ConnectSocket, cStr.data(), packet_size);
	}
}
