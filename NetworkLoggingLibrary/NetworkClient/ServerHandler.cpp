#include "ServerHandler.h"

// need to define static variables in cpp too
unsigned int ServerHandler::client_id;

ServerHandler::ServerHandler(char* port)
{
	// init the client id
	client_id = 0;

	// init curses
	LogOutput::initCurses();

	LogOutput::updateWindow(LogOutput::LOG_WINDOWS::WIN_BOTTOM, "UP/DOWN: Move log output | TAB: Switch between log and variable view");

	// setup the server network object to listen
	network = new ServerNetwork(port);
}

ServerHandler::~ServerHandler()
{
	LogOutput::stopCurses();
}

void ServerHandler::update()
{
	// get new clients
	if (network->acceptNewClient(client_id))
		client_id++;

	receiveFromClients();
}

void ServerHandler::receiveFromClients()
{
	// go through all clients
	std::map<unsigned int, ConnectedSocket>::iterator iter;

	for (iter = network->sessions.begin(); iter != network->sessions.end(); iter++)
	{
		memset(&network_data[0], 1, sizeof(network_data));

		// get data for that client
		int data_length = network->receiveData(iter->first, network_data);

		if (data_length <= 0)
		{
			if (data_length == 0) // gracefully closed
				_sessionsToBeRemoved.push_back(iter->first);
			// nothing received
			continue;
		}

		// loop through the network data as there may be multiple log messages to output
		int lastPos = 0;
		char* pos_char = strchr(network_data, '\0');

		do
		{
			int pos = (int)(pos_char - network_data) + 1;
			if (pos > (sizeof(network_data) / sizeof(*network_data))) break;
			std::string outputString(&network_data[lastPos], 0, pos - lastPos);

			LogOutput::outputLogMessage(iter->second.peer_ip, outputString);

			lastPos = pos;
			pos_char++;
		} while ((pos_char = strchr(pos_char, '\0')) && pos_char != NULL);
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
