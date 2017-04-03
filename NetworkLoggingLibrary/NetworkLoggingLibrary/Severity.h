// Severity.h
// Main header file to include in an application to send log messages over the network

#pragma once

namespace logovernet
{
	// the different levels of logging
	// LON - Log Over Net - stops clashes with defines
	// from windows header files
	enum LOG_SEVERITY {
		LON_DEBUG,
		LON_INFO,
		LON_WARNING,
		LON_ERROR
	};
}
