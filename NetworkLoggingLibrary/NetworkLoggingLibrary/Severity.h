// Severity.h
// Main header file to include in an application to send log messages over the network

#ifndef __LOGOVERNET_SEVERITY__
#define __LOGOVERNET_SEVERITY__

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

#endif
