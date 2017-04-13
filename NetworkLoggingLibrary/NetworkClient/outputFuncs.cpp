#include "outputFuncs.h"

bool LogOutput::variableView = false;
WINDOW* LogOutput::wins[4];
PANEL* LogOutput::panels[4];
int LogOutput::currentLogPosition = 0;
std::vector<std::string> LogOutput::messages;

void LogOutput::outputLogMessage(std::string ip, std::string message, LOG_SEVERITY severity)
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

	// Severity Level
	output << "[" << LOG_STRINGS[severity] << "] ";

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
		update_panels();
		doupdate();
	}
}

void LogOutput::initCurses()
{
	// setup curses
	initscr();
	cbreak();
	noecho(); // don't show ouput

	// create windows
	wins[0] = newwin(2, COLS, 0, 0); // top
	wins[1] = newwin(LINES - 4, COLS, 2, 0); // middle area
	wins[2] = newwin(1, COLS, LINES - 1, 0); // bottom
	wins[3] = newwin(LINES - 10, COLS - 10, 5, 5); // variable window

	// can scroll the main window
	scrollok(wins[1], TRUE);

	keypad(wins[1], TRUE); // alow keyboard input handling
	nodelay(wins[1], TRUE);

	// create panels
	panels[0] = new_panel(wins[0]);
	panels[1] = new_panel(wins[1]);
	panels[2] = new_panel(wins[2]);
	panels[3] = new_panel(wins[3]);

	// hide the top panel until the user presses TAB
	hide_panel(panels[3]);

	update_panels();
	doupdate();
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
	update_panels();
	doupdate();
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
		if (moveAmount > 0 && ((currentLogPosition + (LINES - 4) + moveAmount) <= messages.size()))
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
		updateCursor();
	}
}

void LogOutput::toggleWindow()
{
	if (variableView)
		hide_panel(panels[3]);
	else
		show_panel(panels[3]);
	update_panels();
	doupdate();
	variableView = !variableView;
	updateCursor();
}

void LogOutput::updateVariableWindow(std::map<std::string, std::string>& variable_map)
{
	if (variableView)
	{
		WINDOW* vWin = wins[3];

		// clear the window
		wclear(vWin);

		// add the border
		box(vWin, 0, 0);

		// add the headers
		int yPos = 3, xPos = ((COLS - 10) - 84) / 2;
		mvwprintw(vWin, 1, 2, "Variable View");
		mvwprintw(vWin, yPos++, xPos, "Memory Address\tCurrent Value");
		mvwprintw(vWin, yPos++, xPos, "-----------------------------------------------------------------------------------");

		// iterate through the map of variables
		for (auto iter = variable_map.begin(); iter != variable_map.end(); ++iter, ++yPos)
		{
			// loop through to add spaces to get the correct formatting
			std::string spaces("");
			for (int i = iter->first.length(); i < 19; i++)
				spaces += " ";
			// print the variable
			mvwprintw(vWin, yPos, xPos, "%s%s%s", iter->first.c_str(), spaces.c_str(), iter->second.c_str());
		}

		// update window
		update_panels();
		doupdate();
	}
}

void LogOutput::redrawLogMessages(int offset)
{
	if (offset < 0)
	{
		offset = -offset;
		for (int i = currentLogPosition, j = 0; i < (currentLogPosition + offset); ++i, ++j)
			mvwprintw(wins[1], j, 0, messages[i].c_str());
		update_panels();
		doupdate();
	}
	else if (offset > 0)
	{
		int bottomPos = (LINES - 4);
		for (int i = (currentLogPosition-offset)+bottomPos, j = bottomPos - offset; i < (currentLogPosition + bottomPos); ++i, ++j)
			mvwprintw(wins[1], j, 0, messages[i].c_str());
		update_panels();
		doupdate();
	}
}

void LogOutput::updateCursor()
{
	if (!variableView)
	{
		if (messages.size() < (LINES - 4) || (currentLogPosition + (LINES - 4)) == messages.size())
			curs_set(1);
		else
			curs_set(0);
	}
	else
		curs_set(0);
}
