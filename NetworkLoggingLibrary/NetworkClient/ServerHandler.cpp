#include "ServerHandler.h"

// need to define static variables in cpp too
unsigned int ServerHandler::client_id;

ServerHandler::ServerHandler(char* port)
{
	// init the client id
	client_id = 0;

	// setup the server network object to listen
	network = new ServerNetwork(port);
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

	receiveFromClients();
}

void ServerHandler::receiveFromClients()
{
	// go through all clients
	std::map<unsigned int, SOCKET>::iterator iter;

	for (iter = network->sessions.begin(); iter != network->sessions.end(); iter++)
	{
		// get data for that client
		int data_length = network->receiveData(iter->first, network_data);

		if (data_length <= 0)
		{
			if (data_length == 0) // gracefully closed
				_sessionsToBeRemoved.push_back(iter->first);
			// nothing received
			continue;
		}

		//int i = 0;
		//while (i < (unsigned int)data_length)
		//{

		char* pos_char = strchr(network_data, '\0');
		int pos = (int)(pos_char - network_data);
		std::string outputString(network_data, 0, pos);

		std::cout << "Message received from #" << iter->first <<
			": " << outputString << std::endl;
		//find}
	}

	// remove closed sessions
	if (_sessionsToBeRemoved.size() > 0)
	{
		std::vector<unsigned int>::iterator iter;
		for (iter = _sessionsToBeRemoved.begin(); iter != _sessionsToBeRemoved.end(); iter++)
		{
			network->closeClientConnection(*iter);
		}
		_sessionsToBeRemoved.clear();
	}
}
