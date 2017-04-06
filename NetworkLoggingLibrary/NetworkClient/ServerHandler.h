// ServerHandler.h
// Handles the loop and update of clients waiting to connect

#pragma once

#include "ServerNetwork.h"
#include "Packet.h"
#include <vector>
#include <iostream>
#include <sstream>
#include <ctime>

class ServerHandler
{
public:
	ServerHandler(char* port);
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

	// to store sessions to be removed
	std::vector<unsigned int> _sessionsToBeRemoved;

	// output a message in the format:
	// [ip] [dd/mm/yy HH:MM:SS] : message
	void outputMessage(std::string ip, std::string message);
};

