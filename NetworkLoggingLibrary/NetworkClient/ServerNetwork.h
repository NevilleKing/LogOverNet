// ServerNetwork.h
// Class for receiving messages from the logging library

#pragma once

#include <WinSock2.h>
#include <Windows.h>
#include <WS2tcpip.h>
#include <iostream>
#include <map>

#include "NetworkServices.h"
#include "Packet.h"

#pragma comment (lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "6881"

class ServerNetwork
{
public:
	// constructors
	ServerNetwork();
	~ServerNetwork();

	// Socket to listen for new connections
	SOCKET ListenSocket;
	SOCKET ClientSocket;

	// for error checking & return values
	int iResult;

	// accept new connection
	bool acceptNewClient(unsigned int& id);

	// close a connection with a client
	bool closeClientConnection(unsigned int& id);

	// receive incoming data
	int receiveData(unsigned int client_id, char* recvbuf);

	// table to keep track of each client
	std::map<unsigned int, SOCKET> sessions;
};
