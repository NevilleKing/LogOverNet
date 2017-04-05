// main.cpp
// Entry point for the application

#include "ServerHandler.h"

void ServerLoop();

ServerHandler* server;

int main(int argc, char *argv[])
{
	char* port;
	if (argc > 2)
	{
		if (argv[1] == "--port")
		{
			port = argv[2];
		}
	}
	else
	{
		port = DEFAULT_PORT;
	}

	server = new ServerHandler(port);

	ServerLoop();

	return 0;
}

// main loop
void ServerLoop()
{
	while (true)
	{
		// call the update function every loop
		// this checks to see if there is new clients / messages waiting
		server->update();
	}
}
