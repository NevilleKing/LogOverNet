#include "ServerHandler.h"

// need to define static variables in cpp too
unsigned int ServerHandler::client_id;

ServerHandler::ServerHandler(char* port, std::string logFilename)
{
	// init the client id
	client_id = 0;

	// init curses
	LogOutput::initCurses();

	// Instructions window (bottom)
	_basicInstr = "UP/DOWN: Move log output | TAB: View/hide variable view | ";
	updateInstructionsWindow();

	// load from log file if possible
	myFile = new FileIO(logFilename);
	myFile->readLogFromFile();
	LogOutput::setOutputLogFile(*myFile);
	for (auto&& msg : myFile->loadedMessages)
		LogOutput::outputLogMessage(msg.ip, msg.message, msg.severity, msg.timestamp, true);

	// setup the server network object to listen
	network = new ServerNetwork(port);
}

ServerHandler::~ServerHandler()
{
	LogOutput::stopCurses();
	if (myFile != nullptr)
		delete myFile;
}

bool ServerHandler::update()
{
	// get new clients
	if (network->acceptNewClient(client_id))
		client_id++;

	receiveFromClients();

	// process keyboard input
	int ch;
	switch (ch = LogOutput::getKeyboardInput())
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
	// handle 0-9 (severity level change)
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		handleSeverityChange(ch - '0');
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
			// messages are split into 2 parts, split by '\0'
			// first is severity followed by the memory address
			// second is the message / variable value
			if ((currentLoop % 2) == 0)
			{
				++lastPos;
				if (currentLoop > 0 && retrPacket.memory_address.length() == 0) // is a normal message
					LogOutput::outputLogMessage(iter->second.peer_ip, retrPacket.data, retrPacket.severity);
				else if (currentLoop > 0) // is a variable value
					updateVariable(retrPacket.memory_address, retrPacket.data);
			}

			// find next '\0'
			int pos = (int)(pos_char - network_data) + 1;
			if (pos > (sizeof(network_data) / sizeof(*network_data))) break;
			
			std::string outputString(&network_data[lastPos], 0, pos - lastPos);

			if ((currentLoop % 2) == 0) // sev & mem addr
			{
				retrPacket.severity = static_cast<LOG_SEVERITY>(network_data[lastPos - 1]);
				retrPacket.memory_address = outputString;
			}
			else // value
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
	output << "0-" << (LOG_STRINGS_LENGTH > 10 ? 9 : LOG_STRINGS_LENGTH);
	output << ": Filter severity (currently " << (LogOutput::currentSeverity == -1 ? "ALL" : LOG_STRINGS[LogOutput::currentSeverity]) << ")";

	LogOutput::updateWindow(LogOutput::LOG_WINDOWS::WIN_BOTTOM, output.str());
}

void ServerHandler::handleSeverityChange(int numPressed)
{
	// align number with string array
	--numPressed;

	// boundary check
	if (numPressed >= -1 && numPressed < (int)LOG_STRINGS_LENGTH && LogOutput::currentSeverity != numPressed)
	{
		// change selected severity
		LogOutput::currentSeverity = (LOG_SEVERITY)numPressed;

		// update help info
		updateInstructionsWindow();

		// let output class handle filtering
		LogOutput::filterLogMessages(LogOutput::currentSeverity);
	}
}
