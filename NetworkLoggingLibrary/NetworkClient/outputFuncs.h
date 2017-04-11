// outputFuncs.h
// Header files for output functions to be used by multiple classes

#pragma once

#include <iostream>
#include <sstream>
#include <ctime>

#include <curses.h>

#include <vector>

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

	// Overwrite a window with a string
	static void updateWindow(LOG_WINDOWS window, std::string value);

	// get the int value of a key that has been pressed
	static int getKeyboardInput();

	// move main window up or down
	static void moveWindow(int moveAmount);

private:
	// windows for curses
	static WINDOW* wins[3];

	// current scroll of window (for log messages)
	// refers to the topmost visible message
	static int currentLogPosition;

	// all log messages
	static std::vector<std::string> messages;

	static void redrawLogMessages(int offset);
};
