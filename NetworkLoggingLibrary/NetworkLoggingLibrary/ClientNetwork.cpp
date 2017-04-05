#include "ClientNetwork.h"

namespace logovernet
{

	ClientNetwork::ClientNetwork(std::string ip_addr, std::string port_num)
	{
		// convert strings to c_str
		std::vector<char> ip(ip_addr.begin(), ip_addr.end());
		ip.push_back('\0');
		std::vector<char> port(port_num.begin(), port_num.end());
		port.push_back('\0');

		// Create WSADATA object
		WSADATA wsaData;

		// socket
		ConnectSocket = INVALID_SOCKET;

		// holds address info for socket to connect to
		struct addrinfo *result = NULL,
						*ptr = NULL,
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
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP; // tcp connection

		// resolve server address and port
		iResult = getaddrinfo(ip.data(), port.data(), &hints, &result);

		if (iResult != 0)
		{
			std::cout << "getaddrinfo failed with error: " << iResult << std::endl;
			WSACleanup();
			exit(1);
		}

		// Attempt to connect to an address until one succeeds
		for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
		{
			// Create a SOCKET for connecting to server
			ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

			if (ConnectSocket == INVALID_SOCKET)
			{
				std::cout << "socket failed with error: " << WSAGetLastError() << std::endl;
				WSACleanup();
				exit(1);
			}

			// Connect to the Server
			iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);

			if (iResult == SOCKET_ERROR)
			{
				closesocket(ConnectSocket);
				ConnectSocket = INVALID_SOCKET;
				std::cout << "Server is down. Could not connect." << std::endl;
			}
		}

		// No longer need address info for server
		freeaddrinfo(result);

		// if connection failed
		if (ConnectSocket == INVALID_SOCKET)
		{
			std::cout << "Unable to connect to server" << std::endl;
			WSACleanup();
			exit(1);
		}

		// Set the mode of socket to be non-blocking
		// This means the client won't wait on send/receive when there is no
		// data to send or receive
		u_long iMode = 1;

		iResult = ioctlsocket(ConnectSocket, FIONBIO, &iMode);
		if (iResult == SOCKET_ERROR)
		{
			std::cout << "ioctlsocket failed with error: " << iResult << std::endl;
			closesocket(ConnectSocket);
			WSACleanup();
			exit(1);
		}
	}

	ClientNetwork::~ClientNetwork()
	{
		// shutdown communication in both directions
		shutdown(ConnectSocket, SD_BOTH);
		
		// close the socket
		closesocket(ConnectSocket);
	}

}
