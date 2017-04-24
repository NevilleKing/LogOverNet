// Severity.h
// Holds simple severity info

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
