// ServerHandler.h
// Handles the loop and update of clients waiting to connect

#pragma once

#include "outputFuncs.h"
#include "ServerNetwork.h"
#include <vector>
#include "Packet.h"

class ServerHandler
{
public:
	ServerHandler(char* port);
	~ServerHandler();

	// update function
	bool update();

	// receive messages from clients
	void receiveFromClients();

private:
	// add or update value of variable
	void updateVariable(std::string memAddr, std::string value);

	// id for the clients connecting
	static unsigned int client_id;

	// Server Object
	ServerNetwork* network;

	// data buffer
	char network_data[MAX_PACKET_SIZE];

	// to store sessions to be removed
	std::vector<unsigned int> _sessionsToBeRemoved;

	// map to store current variable values
	std::map<std::string, std::string> _variableMap;

	// update instructions
	void updateInstructionsWindow();

	// to hold currently view severity level (-1 => ALL)
	LOG_SEVERITY _currentSeverity = (LOG_SEVERITY)-1;

	// Basic instructions (without severity level)
	std::string _basicInstr;
};

