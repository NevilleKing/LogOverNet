#pragma once

#include <string>

enum LOG_SEVERITY {
	LON_DEBUG,
	LON_INFO,
	LON_WARNING,
	LON_ERROR
};

const std::string LOG_STRINGS[4]
{
	"DEBUG",
	"INFO",
	"WARNING",
	"ERROR"
};

const unsigned int LOG_STRINGS_LENGTH = (sizeof(LOG_STRINGS) / sizeof(LOG_STRINGS[0]));
