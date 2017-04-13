// Packet.h
// Class to hold info which is serialized and sent over the network

#pragma once

#include <string>

#define MAX_PACKET_SIZE 1000000

enum LOG_SEVERITY {
	LON_DEBUG,
	LON_INFO,
	LON_WARNING,
	LON_ERROR
};

const std::string LOG_STRINGS[4]
{
	"DEBUG",
	"INFO",
	"WARNING",
	"ERROR"
};

struct Packet
{
	// Severity Level
	LOG_SEVERITY severity;

	// Memory address of variable (can be blank)
	std::string memory_address;

	// Message / Variable value
	std::string data;
};
