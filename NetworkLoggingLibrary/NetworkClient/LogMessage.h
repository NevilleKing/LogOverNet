#pragma once

#include "Severity.h"
#include <string>

class LogMessage
{
public:
	LOG_SEVERITY severity;
	std::string message;
	bool visible = true;

	LogMessage(LOG_SEVERITY sev, std::string msg)
		: severity(sev), message(msg) {};
};