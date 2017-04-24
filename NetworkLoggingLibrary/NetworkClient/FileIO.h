// FileIO.h
// Used for retreiving and saving logs to file

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
	// creates/opens file
	FileIO(std::string filename);
	// closes file (more efficient to keep open
	// as we are likely to do lots of writing)
	~FileIO();

	// Read log data from file
	void readLogFromFile();

	// Save log data to file
	void saveLogToFile(std::string ip, std::string timestamp, LOG_SEVERITY sev, std::string log_message);

	// check if the file is open
	bool isOpen();

	// flush the file if needed (write)
	void flushFile();

	// the most recent batch of messages loaded
	std::vector<LogMessage> loadedMessages;

private:
	std::string _filename;
	
	// we don't need to read if it's a new file
	bool newFile = false;

	std::fstream myFile;

	bool needsFlush = false;
};