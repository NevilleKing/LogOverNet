#include "ServerNetwork.h"

ServerNetwork::ServerNetwork()
{
	// Create WSADATA object
	WSADATA wsaData;

	// initailise the socket
	ListenSocket = INVALID_SOCKET;

	// address info for the server to listen to
	struct addrinfo *result = NULL,
					hints;

	// Initialise Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		std::cout << "WSAStartup failed with error: " << iResult << std::endl;
		exit(1);
	}

	// set address info
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol - IPPROTO_TCP; // tcp connection
	hints.ai_flags = AI_PASSIVE;

	// Resolve the server address and port
	// We don't need an ip address as it's on the local machine
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);

	if (iResult != 0)
	{
		std::cout << "geraddrinfo failed with error: " << iResult << std::endl;
		WSACleanup();
		exit(1);
	}

	// Create a SOCKET for connecting to server
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	if (ListenSocket == INVALID_SOCKET)
	{
		std::cout << "Socket failed with error: " << WSAGetLastError() << std::endl;
		freeaddrinfo(result);
		WSACleanup();
		exit(1);
	}

	// Set the mode of socket to be non-blocking
	u_long iMode = 1;
	iResult = ioctlsocket(ListenSocket, FIONBIO, &iMode);

	if (iResult == SOCKET_ERROR)
	{
		std::cout << "ioctlsocket failed with error: " << WSAGetLastError() << std::endl;
		closesocket(ListenSocket);
		WSACleanup();
		exit(1);
	}

	// Setup the TCP listening socket
	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);

	if (iResult == SOCKET_ERROR)
	{
		std::cout << "bind failed with error: " << WSAGetLastError() << std::endl;
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		exit(1);
	}

	// no longer need address info
	freeaddrinfo(result);

	// start listening for new clients attempting to connect
	iResult = listen(ListenSocket, SOMAXCONN);

	if (iResult == SOCKET_ERROR)
	{
		std::cout << "listen failed with error: " << WSAGetLastError() << std::endl;
		closesocket(ListenSocket);
		WSACleanup();
		exit(1);
	}
}

bool ServerNetwork::acceptNewClient(unsigned int & id)
{
	// if client waiting, accept the conection and save the socket
	ClientSocket = accept(ListenSocket, NULL, NULL);

	if (ClientSocket != INVALID_SOCKET)
	{
		// insert new client into session id table
		sessions.insert(std::pair<unsigned int, SOCKET>(id, ClientSocket));

		return true;
	}

	return false;
}