#include "FileIO.h"

FileIO::FileIO(std::string filename) : _filename(filename)
{
	// check if file exists
	struct stat buffer;
	if (stat(_filename.c_str(), &buffer) != 0)
		newFile = true;

	// open file for reading/writing
	myFile.open(filename, ios::in | ios::app);
}

FileIO::~FileIO()
{
	myFile.close();
}

void FileIO::readLogFromFile()
{
	// no need to run this if the file didn't exist before
	if (newFile)
		return;

	loadedMessages.clear();

	// loop through the lines of the file
	std::string line;
	std::vector<std::string> strings;

	while (std::getline(myFile, line))
	{
		// multi-line check
		if (line.at(1) != ';')
		{
			// print previous line
			if (strings.size() == 4)
				loadedMessages.push_back({ strings[0], strings[1], (LOG_SEVERITY)std::stoi(strings[2]), strings[3] });
			strings.clear();
		}

		// ';' is the delimiter, loop through getting the next one
		int offset = 0;
		size_t i;
		int curLoop = 0;
		while (true)
		{
			i = line.find(';', offset);
			std::string substr = (curLoop == 3) ? line.substr(offset) : line.substr(offset, i - offset);
			if (strings.size() < 4)
				strings.push_back(substr);
			else if (curLoop == 3)
				strings[3].append(substr);
			if (i == std::string::npos || curLoop == 3) break;
			offset = i + 1;
			++curLoop;
		}
	}

	if (strings.size() == 4)
		loadedMessages.push_back({ strings[0], strings[1], (LOG_SEVERITY)std::stoi(strings[2]), strings[3] });

	// clear error state
	myFile.clear();
}

void FileIO::saveLogToFile(std::string ip, std::string timestamp, LOG_SEVERITY sev, std::string log_message)
{
	if (!isOpen())
		return;

	myFile << ip << ";" << timestamp << ";" << (int)sev << ";" << log_message << "\n";
	myFile.flush(); // flush - just in case there is a crash - makes sure it writes straight away
}

bool FileIO::isOpen()
{
	return myFile.is_open();
}
