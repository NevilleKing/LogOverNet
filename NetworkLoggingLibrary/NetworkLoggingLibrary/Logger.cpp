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
		// create packet
		const unsigned int packet_size = sizeof(Packet);
		char packet_data[packet_size];

		Packet packet;
		packet.packet_msg = msg.c_str();

		packet.serialize(packet_data);

		Network::sendMsg(network->ConnectSocket, packet_data, packet_size);
	}
}
