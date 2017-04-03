// Networking.h
// Wraps the networking function calls, using the winsocket API

#pragma once

#include <WinSock2.h>
#include <Windows.h>

namespace logovernet
{
	class Network
	{
	public:
		// function to send messages using a specific socket
		static int sendMsg(SOCKET socket, char* msg, int msgSize);
	};
}
