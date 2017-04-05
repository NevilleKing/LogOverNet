// ClientNetwork.h
// Client network class

#pragma once
#include <WinSock2.h>
#include <Windows.h>
#include "Networking.h"
#include <WS2tcpip.h>
#include <iostream>
#include <vector>

// Defines
// TODO: Replace these later on
// Buffer size
#define DEFAULT_BUFLEN 512
// port to connect sockets through
#define DEFAULT_PORT "6881"
// Need to link some libraries
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

namespace logovernet
{
	class ClientNetwork
	{
	public:

		// for error checking
		int iResult;

		// socket for client to connect to server
		SOCKET ConnectSocket;

		// constructors
		ClientNetwork(std::string ip_addr, std::string port_num);
		~ClientNetwork();
	};
}
