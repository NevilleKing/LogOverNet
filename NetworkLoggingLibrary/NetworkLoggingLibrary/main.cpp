#include <iostream>
#include "logovernet.h"
#include <vector>
#include <string>

using namespace logovernet;

int main(int argc, char *argv[])
{
	// setup logger with ip address
	Logger logger = Logger("127.0.0.1", DEFAULT_PORT);

	GLOBAL_LOGGER = &logger;
	int i = 0;
	while (true)
	{
		logger.sendMessage("ABCDEFGHIJKLMNOPQRSTUVWXYZ+abcdefghijklmnopqrstuvwxyz+ABCDEFGHIJKLMNOPQRSTUVWXYZ+abcdefghijklmnopqrstuvwxyz+ABCDEFGHIJKLMNOPQRSTUVWXYZ+abcdefghijklmnopqrstuvwxyz+ABCDEFGHIJKLMNOPQRSTUVWXYZ+abcdefghijklmnopqrstuvwxyz+ABCDEFGHIJKLMNOPQRSTUVWXYZ+abcdefghijklmnopqrstuvwxyz+ABCDEFGHIJKLMNOPQRSTUVWXYZ+abcdefghijklmnopqrstuvwxyz+ABCDEFGHIJKLMNOPQRSTUVWXYZ+abcdefghijklmnopqrstuvwxyz+ABCDEFGHIJKLMNOPQRSTUVWXYZ+abcdefghijklmnopqrstuvwxyz+ABCDEFGHIJKLMNOPQRSTUVWXYZ+abcdefghijklmnopqrstuvwxyz+ABCDEFGHIJKLMNOPQRSTUVWXYZ+abcdefghijklmnopqrstuvwxyz", LOG_SEVERITY::LON_INFO);
		//logger.sendMessage("test" + std::to_string(++i), LON_INFO);
		//logger.sendMessage("test" + std::to_string(i), LON_ERROR);
		system("pause");
	}

	/* CAUSES ERROR
	for (int i = 0; i < 10; ++i)
	{
		logger.sendMessage("error" + std::to_string(i), LOG_SEVERITY::LON_ERROR);
		logger.sendMessage("debug" + std::to_string(i), LOG_SEVERITY::LON_DEBUG);
	}
	*/

	/*lon_bool b = false;
	
	b = !b;*/

	system("pause");

	return 0;
}