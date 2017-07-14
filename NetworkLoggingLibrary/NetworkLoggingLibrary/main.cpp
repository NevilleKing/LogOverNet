#include <iostream>
#include "logovernet.h"
#include <vector>
#include <string>

using namespace logovernet;

int main(int argc, char *argv[])
{
	// setup logger with ip address
	Logger logger = Logger("127.0.0.1", DEFAULT_PORT);

	logger.sendMessage("info message", LOG_SEVERITY::LON_INFO);
	logger.sendMessage(std::string("error!"), LOG_SEVERITY::LON_ERROR);
	logger.sendMessage(true, LOG_SEVERITY::LON_WARNING);

	system("pause");

	setGlobalLogger(logger);
	for (lon_int i = 0; i < 5; ++i)
	{
		std::cout << "i is " << i << std::endl;
		system("pause");
	}

	LonType<std::string> myString("myMessage");
	std::cout << myString << std::endl;

	system("pause");

	for (int i = 0; i < 10; i++)
	{
		logger << "ABCDEFGHIJKLMNOPQRSTUVWXYZ" << "abcdefghijklmnopqrstuvwxyz" << 123456789 
			   << "ABCDEFGHIJKLMNOPQRSTUVWXYZ" << "abcdefghijklmnopqrstuvwxyz" << 123456789 
			   << "ABCDEFGHIJKLMNOPQRSTUVWXYZ" << "abcdefghijklmnopqrstuvwxyz" << 123456789 << LON_EOL();
	}

	return 0;
}