#include <iostream>
#include "logovernet.h"
#include <vector>
#include <string>

#include <chrono>


typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::high_resolution_clock::time_point TimePoint;

using namespace logovernet;

int main(int argc, char *argv[])
{
	// setup logger with ip address
	Logger logger = Logger("127.0.0.1", DEFAULT_PORT);

	auto test = std::chrono::duration_cast<std::chrono::nanoseconds>(Clock::now().time_since_epoch()).count();

	logger.sendMessage("test", LOG_SEVERITY::LON_INFO);

	std::cout << test << std::endl;

	system("pause");

	return 0;
}