# LogOverNet

A logging library for C++ which sends logging messages to a seperate machine over the network. Only available for Windows, the library
was built from the ground up using the Windows Socket API (Winsock) for the 3rd year dissertation project for Games Computing
at the University of Lincoln.

By conducting a literature review, the following features were gathered based on current logging practices:
* Reliable transport protocol
* Easy to use interface and logging methods
* Automatic logging
* Verbosity level
* Log output with timestamp and ip address
* Limited external dependencies

