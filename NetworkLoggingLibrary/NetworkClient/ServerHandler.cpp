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
		client_id++;

	receiveFromClients();
}

void ServerHandler::receiveFromClients()
{
	// go through all clients
	std::map<unsigned int, ConnectedSocket>::iterator iter;

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

		outputMessage(iter->second.peer_ip, outputString);
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

void ServerHandler::outputMessage(std::string ip, std::string message)
{
	// create output stringstream
	std::stringstream output;

	// ip address
	output << "[" << ip << "] ";

	// time
	bool precDay = false, precMon = false, precHour =  false, precMin = false, precSec = false;

	time_t t = time(0);
	struct tm* now = localtime(&t);
	now->tm_year = (now->tm_year + 1900) - 2000;
	now->tm_mon++;

	if (snprintf(nullptr, 0, "%d", now->tm_mday) == 1)
		precDay = true;
	if (snprintf(nullptr, 0, "%d", now->tm_mon) == 1)
		precMon = true;
	if (snprintf(nullptr, 0, "%d", now->tm_hour) == 1)
		precHour = true;
	if (snprintf(nullptr, 0, "%d", now->tm_min) == 1)
		precMin = true;
	if (snprintf(nullptr, 0, "%d", now->tm_sec) == 1)
		precSec = true;
		
	output << "[" << (precDay ? std::string("0").append(std::to_string(now->tm_mday))  : std::to_string(now->tm_mday)) << "/"  <<
					 (precMon ? std::string("0").append(std::to_string(now->tm_mon))   : std::to_string(now->tm_mon))  << "/"  <<
					 now->tm_year    << " " <<
					 (precHour ? std::string("0").append(std::to_string(now->tm_hour)) : std::to_string(now->tm_hour)) << ":"  <<
			  		 (precMin  ? std::string("0").append(std::to_string(now->tm_min))  : std::to_string(now->tm_min))  << ":"  <<
			  		 (precSec  ? std::string("0").append(std::to_string(now->tm_sec))  : std::to_string(now->tm_sec))  << "] ";

	// message
	output << ": " << message;

	// output the entire string
	std::cout << output.str() << std::endl;
}
