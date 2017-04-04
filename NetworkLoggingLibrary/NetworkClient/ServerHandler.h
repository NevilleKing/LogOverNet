// ServerHandler.h
// Handles the loop and update of clients waiting to connect

#pragma once

#include "ServerNetwork.h"
#include "Packet.h"

class ServerHandler
{
public:
	ServerHandler();
	~ServerHandler();

	// update function
	void update();

	// receive messages from clients
	void receiveFromClients();

private:
	// id for the clients connecting
	static unsigned int client_id;

	// Server Object
	ServerNetwork* network;

	// data buffer
	char network_data[MAX_PACKET_SIZE];
};

