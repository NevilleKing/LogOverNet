#include "ServerNetwork.h"

ServerNetwork::ServerNetwork(char* port_num)
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
	iResult = getaddrinfo(NULL, port_num, &hints, &result);

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

	// Get the current ip address
	char hostName[255];
	bool success = false;
	if (gethostname(hostName, 255) != SOCKET_ERROR)
	{
		struct hostent *host_entry;
		host_entry = gethostbyname(hostName);
		if (host_entry != nullptr)
		{
			char* localIP = inet_ntoa(*(struct in_addr*)*host_entry->h_addr_list);
			if (localIP != NULL)
			{
				std::stringstream ss;
				ss << "LogOverNet Server | IP: " << localIP << " | Port: " << port_num;
				LogOutput::updateWindow(LogOutput::LOG_WINDOWS::WIN_TOP, ss.str());
				success = true;
			}
		}
	}
	
	if (!success) // unable to get ip address info
	{
		std::stringstream ss;
		ss << "LogOverNet Server | Port: " << port_num;
		LogOutput::updateWindow(LogOutput::LOG_WINDOWS::WIN_TOP, ss.str());
	}

	LogOutput::outputLogMessage("", "== Server Started ==");
}

bool ServerNetwork::acceptNewClient(unsigned int & id)
{
	// if client waiting, accept the conection and save the socket
	ClientSocket = accept(ListenSocket, NULL, NULL);

	if (ClientSocket != INVALID_SOCKET)
	{
		// get the ip address so we can output
		std::string peer_ip = getIPfromSocket(ClientSocket);

		// insert new client into session id table
		sessions.insert(std::pair<unsigned int, ConnectedSocket>(id, {ClientSocket, peer_ip}));

		LogOutput::outputLogMessage(peer_ip, "==== Client " + peer_ip + " connected ====");

		return true;
	}

	return false;
}

bool ServerNetwork::closeClientConnection(unsigned int & id)
{
	// check that the id is contained in the sessions table
	auto session = sessions.find(id);
	if (session != sessions.end())
	{
		std::string peer_ip = sessions[id].peer_ip;
		iResult = closesocket(session->second.socket);
		if (iResult != SOCKET_ERROR)
		{
			if (peer_ip == "")
				LogOutput::outputLogMessage(peer_ip, "Connection closed with client.");
			else
				LogOutput::outputLogMessage(peer_ip, "==== Connection closed with " + peer_ip + " ====");

			return true;
		}
	}
	return false;
}

int ServerNetwork::receiveData(unsigned int client_id, char * recvbuf)
{
	if (sessions.find(client_id) != sessions.end())
	{
		SOCKET currentSocket = sessions[client_id].socket;
		iResult = NetworkServices::receiveMessage(currentSocket, recvbuf,
			MAX_PACKET_SIZE);
		return iResult;
	}
	return 0;
}

std::string ServerNetwork::getIPfromSocket(SOCKET s)
{
	struct sockaddr_storage addr;
	socklen_t len = sizeof(addr);

	if (getpeername(s, (struct sockaddr*)&addr, &len) != SOCKET_ERROR)
	{
		char ipstr[INET6_ADDRSTRLEN];
		struct sockaddr_in *s_in = (struct sockaddr_in*)&addr;
		inet_ntop(AF_INET, &s_in->sin_addr, ipstr, sizeof(ipstr));
		return std::string(ipstr);
	}
	else // unable to get ip address info
	{
		return std::string();
	}
}
