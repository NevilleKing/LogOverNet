// ServerHandler.h
// Handles the loop and update of clients waiting to connect

#pragma once

#include "outputFuncs.h"
#include "ServerNetwork.h"
#include <vector>
#include "Packet.h"
#include "FileIO.h"

class ServerHandler
{
public:
	ServerHandler(char* port, std::string logFilename);
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

	// Log file to store and load log data from
	FileIO* myFile;

	// to store sessions to be removed
	std::vector<unsigned int> _sessionsToBeRemoved;

	// map to store current variable values
	std::map<std::string, std::string> _variableMap;

	// update instructions
	void updateInstructionsWindow();

	// Basic instructions (without severity level)
	std::string _basicInstr;

	// Severity change (i.e. 0-9 pressed)
	void handleSeverityChange(int numPressed);
};

