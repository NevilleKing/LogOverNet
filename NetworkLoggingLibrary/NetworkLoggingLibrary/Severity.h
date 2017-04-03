// Severity.h
// Main header file to include in an application to send log messages over the network

#pragma once

namespace logovernet
{
	// the different levels of logging
	enum LOG_SEVERITY {
		DEBUG,
		INFO,
		WARNING,
		ERROR
	};
}
