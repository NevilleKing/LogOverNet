// LogMessage.h
// Class for passing around and storing log messages

#pragma once

#include "Severity.h"
#include <string>

class LogMessage
{
public:
	LOG_SEVERITY severity;
	std::string message;
	bool visible = true;
	
	std::string ip;
	std::string timestamp;

	LogMessage(LOG_SEVERITY sev, std::string msg)
		: severity(sev), message(msg) {};

	LogMessage(std::string IP, std::string time, LOG_SEVERITY sev, std::string msg)
		: severity(sev), message(msg), ip(IP), timestamp(time) {};
};
