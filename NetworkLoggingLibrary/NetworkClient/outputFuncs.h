// outputFuncs.h
// Header files for output functions to be used by multiple classes

#pragma once

#include <iostream>
#include <sstream>
#include <ctime>

#include <curses.h>

#define PAD_HEIGHT 9001

class LogOutput
{
public:
	enum LOG_WINDOWS {
		WIN_TOP = 0,
		WIN_BOTTOM = 2
	};

	// output a message in the format:
	// [ip] [dd/mm/yy HH:MM:SS] : message
	static void outputLogMessage(std::string ip, std::string message);

	static void initCurses();
	static void stopCurses();

	static void updateWindow(LOG_WINDOWS window, std::string value);

private:
	// windows for curses
	static WINDOW* wins[3];

	// current scroll of pad (for log messages)
	static int padPosition;
};
