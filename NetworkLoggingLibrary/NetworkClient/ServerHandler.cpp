#include "ServerHandler.h"

// need to define static variables in cpp too
unsigned int ServerHandler::client_id;

ServerHandler::ServerHandler(char* port)
{
	// init the client id
	client_id = 0;

	// init curses
	LogOutput::initCurses();

	// Instructions window (bottom)
	_basicInstr = "UP/DOWN: Move log output | TAB: View/hide variable view | ";
	updateInstructionsWindow();

	// setup the server network object to listen
	network = new ServerNetwork(port);
}

ServerHandler::~ServerHandler()
{
	LogOutput::stopCurses();
}

bool ServerHandler::update()
{
	// get new clients
	if (network->acceptNewClient(client_id))
		client_id++;

	receiveFromClients();

	// process keyboard input
	switch (LogOutput::getKeyboardInput())
	{
	case 27: // ESC or ALT
		if (LogOutput::getKeyboardInput() == -1) // ESC
		{
			return false;
		}
		break;
	case KEY_UP: // move the logs up
		LogOutput::moveWindow(-1);
		break;
	case KEY_DOWN: // move the logs down
		LogOutput::moveWindow(1);
		break;
	case 9: // tab
		LogOutput::toggleWindow();
		LogOutput::updateVariableWindow(_variableMap);
		break;
	}

	return true;
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

		Packet retrPacket;
		int currentLoop = 0;
		do
		{
			if ((currentLoop % 2) == 0)
			{
				++lastPos;
				if (currentLoop > 0 && retrPacket.memory_address.length() == 0)
					LogOutput::outputLogMessage(iter->second.peer_ip, retrPacket.data, retrPacket.severity);
				else if (currentLoop > 0)
					updateVariable(retrPacket.memory_address, retrPacket.data);
			}

			int pos = (int)(pos_char - network_data) + 1;
			if (pos > (sizeof(network_data) / sizeof(*network_data))) break;
			
			std::string outputString(&network_data[lastPos], 0, pos - lastPos);

			if ((currentLoop % 2) == 0)
			{
				retrPacket.severity = static_cast<LOG_SEVERITY>(network_data[lastPos - 1]);
				retrPacket.memory_address = outputString;
			}
			else
				retrPacket.data = outputString;

			lastPos = pos;
			pos_char++;
			++currentLoop;
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

void ServerHandler::updateVariable(std::string memAddr, std::string value)
{
	bool e = false;
	if (memAddr[0] == '-') { e = true; memAddr = memAddr.substr(1); }
	if (_variableMap.find(memAddr) == _variableMap.end())
		_variableMap.emplace(memAddr, value); // not found, insert
	else
	{
		if (e)
			_variableMap.erase(memAddr);
		else
			_variableMap[memAddr] = value; // found value, update
	}

	// update the variable window
	LogOutput::updateVariableWindow(_variableMap);
}

void ServerHandler::updateInstructionsWindow()
{
	std::stringstream output;
	output << _basicInstr;
	int len = (sizeof(LOG_STRINGS) / sizeof(LOG_STRINGS[0]));
	output << "0-" << (len > 10 ? 9 : len - 1);
	output << ": Filter severity (currently " << (_currentSeverity == -1 ? "ALL" : LOG_STRINGS[_currentSeverity]) << ")";

	LogOutput::updateWindow(LogOutput::LOG_WINDOWS::WIN_BOTTOM, output.str());
}
