#include "FileIO.h"

FileIO::FileIO(std::string filename) : _filename(filename)
{
	// check if file exists
	struct stat buffer;
	if (stat(_filename.c_str(), &buffer) != 0)
		newFile = true;

	// open file for reading/writing
	myFile.open(filename, ios::in | ios::out | ios::app);
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

	// loop through the lines of the file
	std::string line;
	while (std::getline(myFile, line))
	{
		// ';' is the delimiter, loop through getting the next one
		int offset = 0;
		std::vector<std::string> strings;
		size_t i;
		while (true)
		{
			i = line.find(';', offset);
			strings.push_back(line.substr(offset, i - offset));
			if (i == std::string::npos) break;
			offset = i + 1;
		}
	}
}

void FileIO::saveLogToFile(std::string ip, std::string timestamp, LOG_SEVERITY sev, std::string log_message)
{
	if (!isOpen())
		return;

	myFile << ip << ";" << timestamp << ";" << (int)sev << ";" << log_message << "\n";
	myFile.flush();
}

bool FileIO::isOpen()
{
	return myFile.is_open();
}
