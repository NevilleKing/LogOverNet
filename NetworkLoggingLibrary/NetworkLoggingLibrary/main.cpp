#include <iostream>
#include "logovernet.h"
#include <vector>

using namespace logovernet;

int main(int argc, char *argv[])
{
	// setup logger with ip address
	Logger logger = Logger("127.0.0.1", DEFAULT_PORT);
	logger.sendMessage("testString", LOG_SEVERITY::LON_ERROR);

	system("pause");

	return 0;
}