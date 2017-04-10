// ServerHandler.h
// Handles the loop and update of clients waiting to connect

#pragma once

#include "outputFuncs.h"
#include "ServerNetwork.h"
#include <vector>

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
	// id for the clients connecting
	static unsigned int client_id;

	// Server Object
	ServerNetwork* network;

	// data buffer
	char network_data[MAX_PACKET_SIZE];

	// to store sessions to be removed
	std::vector<unsigned int> _sessionsToBeRemoved;

};

