#include "outputFuncs.h"

WINDOW* LogOutput::wins[3];
int LogOutput::padPosition = 0;

void LogOutput::outputLogMessage(std::string ip, std::string message)
{
	// create output stringstream
	std::stringstream output;

	// ip address
	if (ip != "")
		output << "[" << ip << "] ";

	// time
	bool precDay = false, precMon = false, precHour = false, precMin = false, precSec = false;

	time_t t = time(0);
	struct tm* now = localtime(&t);
	now->tm_year = (now->tm_year + 1900) - 2000;
	now->tm_mon++;

	if (snprintf(nullptr, 0, "%d", now->tm_mday) == 1)
	precDay = true;
	if (snprintf(nullptr, 0, "%d", now->tm_mon) == 1)
	precMon = true;
	if (snprintf(nullptr, 0, "%d", now->tm_hour) == 1)
	precHour = true;
	if (snprintf(nullptr, 0, "%d", now->tm_min) == 1)
	precMin = true;
	if (snprintf(nullptr, 0, "%d", now->tm_sec) == 1)
	precSec = true;

	output << "[" << (precDay ? std::string("0").append(std::to_string(now->tm_mday)) : std::to_string(now->tm_mday)) << "/" <<
	(precMon ? std::string("0").append(std::to_string(now->tm_mon)) : std::to_string(now->tm_mon)) << "/" <<
	now->tm_year << " " <<
	(precHour ? std::string("0").append(std::to_string(now->tm_hour)) : std::to_string(now->tm_hour)) << ":" <<
	(precMin ? std::string("0").append(std::to_string(now->tm_min)) : std::to_string(now->tm_min)) << ":" <<
	(precSec ? std::string("0").append(std::to_string(now->tm_sec)) : std::to_string(now->tm_sec)) << "] ";

	// message
	output << ": " << message << "\n";

	// output the entire string
	wprintw(wins[1], output.str().c_str());

	// refresh the pad (window)
	prefresh(wins[1], padPosition, 0, 2, 0, LINES - 4, COLS);
}

void LogOutput::initCurses()
{
	// setup curses
	initscr();
	cbreak();
	noecho(); // don't show ouput
	//curs_set(0); // don't show cursor
	keypad(stdscr, TRUE); // alow keyboard input handling

	// create windows
	wins[0] = newwin(2, COLS, 0, 0); // top
	wins[1] = newpad(PAD_HEIGHT, COLS); // middle area (pad because it's scrollable)
	wins[2] = newwin(2, COLS, LINES - 2, 0); // bottom
}

void LogOutput::stopCurses()
{
	endwin(); // end curses mode
}

void LogOutput::updateWindow(LOG_WINDOWS window, std::string value)
{
	WINDOW* w = wins[window];

	// clear window first
	wclear(w);

	// if the window is the top it doesn't need an offset,
	// the bottom needs a 1 offset
	int ypos = window == LOG_WINDOWS::WIN_TOP ? 0 : 1;

	// print to window
	mvwprintw(w, ypos, 0, value.c_str());

	// refresh the window
	wrefresh(w);
}
