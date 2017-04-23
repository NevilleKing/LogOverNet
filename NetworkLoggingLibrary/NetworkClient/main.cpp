// main.cpp
// Entry point for the application

#include "ServerHandler.h"
#include <string>

void ServerLoop();

ServerHandler* server;

int main(int argc, char *argv[])
{
	char* port = DEFAULT_PORT;
	std::string filename = "log.txt";

	if (argc > 2)
	{
		if (std::strcmp(argv[1], "--port") == 0)
		{
			port = argv[2];
		}
		for (int i = 1; i < argc; ++i)
		{
			if (std::strcmp(argv[i], "--port") == 0)
			{
				port = argv[i + 1];
			}
			else if (std::strcmp(argv[i], "--file") == 0)
			{
				filename = argv[i + 1];
			}
		}
	}

	server = new ServerHandler(port, filename);

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
		if (!server->update())
			break;
	}
}
