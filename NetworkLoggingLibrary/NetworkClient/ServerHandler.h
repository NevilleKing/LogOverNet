// ServerHandler.h
// Handles the loop and update of clients waiting to connect

#pragma once

#include "ServerNetwork.h"

class ServerHandler
{
public:
	ServerHandler();
	~ServerHandler();

	// update function
	void update();

private:
	// id for the clients connecting
	static unsigned int client_id;

	// Server Object
	ServerNetwork* network;
};

