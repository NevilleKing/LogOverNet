// NetworkServices.h
// Receiving messages over the network
#pragma once

#include <WinSock2.h>
#include <Windows.h>

class NetworkServices
{
public:
	static int receiveMessage(SOCKET curSocket, char* buffer, int bufSize);
};
