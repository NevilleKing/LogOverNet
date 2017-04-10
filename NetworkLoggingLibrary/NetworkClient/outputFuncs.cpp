#include "outputFuncs.h"

WINDOW* LogOutput::wins[3];
int LogOutput::padPosition = 0;
int LogOutput::totalLogMessages = 0;

void LogOutput::outputLogMessage(std::string ip, std::string message)
{
	// create output stringstream
	std::stringstream output;

	if (totalLogMessages != 0)
	output << "\n";

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
	output << ": " << message;

	// output the entire string
	wprintw(wins[1], output.str().c_str());

	// update the amount of log messages
	++totalLogMessages;

	// move the pad down automatically the pad is currently positioned at the bottom
	int i = LINES - 4;
	if (totalLogMessages >= (LINES - 4) && (padPosition + (LINES - 4)) == totalLogMessages)
		++padPosition;

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

	// create windows
	wins[0] = newwin(2, COLS, 0, 0); // top
	wins[1] = newpad(PAD_HEIGHT, COLS); // middle area (pad because it's scrollable)
	wins[2] = newwin(1, COLS, LINES - 1, 0); // bottom

	keypad(wins[1], TRUE); // alow keyboard input handling
	nodelay(wins[1], TRUE);
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

	// print to window
	mvwprintw(w, 0, 0, value.c_str());

	// refresh the window
	wrefresh(w);
}

int LogOutput::getKeyboardInput()
{
	return wgetch(wins[1]);
}
