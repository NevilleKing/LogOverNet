// main.cpp
// Entry point for the application

#include "ServerHandler.h"
#include <string>

void ServerLoop();

ServerHandler* server;

int main(int argc, char *argv[])
{
	char* port = DEFAULT_PORT;

	if (argc > 2)
	{
		if (std::strcmp(argv[1], "--port") == 0)
		{
			port = argv[2];
		}
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
