// outputFuncs.h
// Header files for output functions to be used by multiple classes

#pragma once

#include <iostream>
#include <sstream>
#include <ctime>

#include <curses.h>
#include <panel.h>

#include <vector>
#include <map>

#include "Severity.h"
#include "LogMessage.h"

class LogOutput
{
public:
	enum LOG_WINDOWS {
		WIN_TOP = 0,
		WIN_BOTTOM = 2
	};

	// to hold currently view severity level (-1 => ALL)
	static LOG_SEVERITY currentSeverity;

	// output a message in the format:
	// [ip] [dd/mm/yy HH:MM:SS] : message
	static void outputLogMessage(std::string ip, std::string message, LOG_SEVERITY severity);

	static void initCurses();
	static void stopCurses();

	// Overwrite a window with a string
	static void updateWindow(LOG_WINDOWS window, std::string value);

	// get the int value of a key that has been pressed
	static int getKeyboardInput();

	// move main window up or down
	static void moveWindow(int moveAmount);

	// toggle the main window between log and variable view
	static void toggleWindow();

	// update the variable window
	static void updateVariableWindow(std::map<std::string, std::string>& variable_map);

	// filter log message based on severity level
	static void filterLogMessages(LOG_SEVERITY severity);
private:
	// the variable window is showing
	static bool variableView;

	// windows for curses
	static WINDOW* wins[4];

	// panels for middle section
	static PANEL* panels[4];

	// current scroll of window (for log messages)
	// refers to the topmost visible message
	static int currentLogPosition;

	// all log messages
	static std::vector<LogMessage> messages;
	
	// visible messages
	static unsigned int visibleMessages;

	static void redrawLogMessages(int offset);

	static void updateCursor();
};
