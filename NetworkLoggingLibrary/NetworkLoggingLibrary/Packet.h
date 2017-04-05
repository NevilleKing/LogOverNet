// Packet.h
// Class to hold info which is serialized and sent over the network

#pragma once

#include <string>

#define MAX_PACKET_SIZE 1000000

namespace logovernet
{
	class Packet
	{
	public:

		std::string packet_msg;

		void serialize(char* data)
		{
			memcpy(data, this, sizeof(Packet));
		}

		void deserialize(char* data)
		{
			memcpy(this, data, sizeof(Packet));
		}
	};
}
