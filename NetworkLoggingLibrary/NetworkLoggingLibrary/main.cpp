#include <iostream>
#include "logovernet.h"
#include <vector>

using namespace logovernet;

int main(int argc, char *argv[])
{
	// setup logger with ip address
	Logger logger = Logger("");
	logger.sendMessage("testString", LOG_SEVERITY::ERROR);

	system("pause");

	return 0;
}