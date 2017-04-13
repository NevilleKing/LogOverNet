// Packet.h
// Class to hold info which is serialized and sent over the network

#pragma once

#include <string>

#include "Severity.h"

#define MAX_PACKET_SIZE 1000000

struct Packet
{
	// Severity Level
	LOG_SEVERITY severity;

	// Memory address of variable (can be blank)
	std::string memory_address;

	// Message / Variable value
	std::string data;
};
