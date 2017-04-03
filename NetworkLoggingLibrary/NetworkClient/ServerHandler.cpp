#include "ServerHandler.h"

// need to define static variables in cpp too
unsigned int ServerHandler::client_id;

ServerHandler::ServerHandler()
{
	// init the client id
	client_id = 0;

	// setup the server network object to listen
	network = new ServerNetwork();
}

void ServerHandler::update()
{
	// get new clients
	if (network->acceptNewClient(client_id))
	{
		std::cout << "Client " << client_id <<
			" has been connected to the server." << std::endl;

		client_id++;
	}
}
