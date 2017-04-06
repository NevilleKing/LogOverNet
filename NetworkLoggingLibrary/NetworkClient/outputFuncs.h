// outputFuncs.h
// Header files for output functions to be used by multiple classes

#pragma once

#include <iostream>
#include <sstream>
#include <ctime>

// output a message in the format:
// [ip] [dd/mm/yy HH:MM:SS] : message
void outputLogMessage(std::string ip, std::string message);
