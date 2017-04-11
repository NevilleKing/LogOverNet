#include "outputFuncs.h"

WINDOW* LogOutput::wins[3];
int LogOutput::currentLogPosition = 0;
std::vector<std::string> LogOutput::messages;

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
	output << ": " << message;

	// add to vector
	messages.push_back(output.str());

	// check if we need to move the window down (log is positioned at bottom of screen)
	if ((currentLogPosition + (LINES - 3)) == messages.size())
		++currentLogPosition;
	// check if we need to output
	if (messages.size() < (LINES - 4) || (currentLogPosition + (LINES - 3)) == (messages.size()+1))
	{
		std::string out = output.str();
		if (messages.size() != 1)
			out = "\n" + out;

		// output the entire string
		wprintw(wins[1], out.c_str());

		// refresh the window to display on screen
		wrefresh(wins[1]);
	}
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
	wins[1] = newwin(LINES - 4, COLS, 2, 0); // middle area
	wins[2] = newwin(1, COLS, LINES - 1, 0); // bottom

	// can scroll the main window
	scrollok(wins[1], TRUE);

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

void LogOutput::moveWindow(int moveAmount)
{
	// only need to update if the the user is able to scroll
	if (messages.size() >= (LINES - 4))
	{
		bool movef = false;
		if (moveAmount > 0 && ((currentLogPosition + (LINES - 4) + moveAmount) <= (currentLogPosition +1)))
			movef = true;
		if (moveAmount < 0 && (currentLogPosition + moveAmount) >= 0)
			movef = true;

		if (movef)
		{
			wscrl(wins[1], moveAmount);
			currentLogPosition += moveAmount;
			redrawLogMessages(moveAmount);
		}

		// update cursor
		if ((currentLogPosition + (LINES - 4)) == (currentLogPosition +1))
			curs_set(1);
		else
			curs_set(0);
	}
}

void LogOutput::redrawLogMessages(int offset)
{
	if (offset < 0)
	{
		offset = -offset;
		for (int i = currentLogPosition, j = 0; i < (currentLogPosition + offset); ++i, ++j)
			mvwprintw(wins[1], j, 0, messages[i].c_str());
		wrefresh(wins[1]);
	}
}
