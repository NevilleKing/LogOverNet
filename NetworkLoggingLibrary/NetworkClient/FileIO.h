#pragma once

#include <sys\stat.h>
#include <string>
#include <fstream>
#include "Severity.h"
#include <vector>
#include "LogMessage.h"

using std::ios;

class FileIO
{
public:
	FileIO(std::string filename);
	~FileIO();

	// Read log data from file
	void readLogFromFile();

	// Save log data to file
	void saveLogToFile(std::string ip, std::string timestamp, LOG_SEVERITY sev, std::string log_message);

	bool isOpen();

	std::vector<LogMessage> loadedMessages;

private:
	std::string _filename;
	
	bool newFile = false;

	std::fstream myFile;
};