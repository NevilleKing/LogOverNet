# LogOverNet

![logovernet](https://user-images.githubusercontent.com/9254173/28210169-fce938b6-688d-11e7-9cb1-2aba7ce2b997.png)

A logging library for C++ which sends logging messages to a separate machine over the network. Only available for Windows, the library
was built from the ground up using the Windows Socket API (Winsock) for a 3rd year dissertation project for Games Computing
at the University of Lincoln.

By conducting a literature review, the following features were gathered based on current logging practices:
* Reliable transport protocol
* Easy to use interface and logging methods
* Automatic logging
* Verbosity level
* Log output with timestamp and ip address
* Limited external dependencies

## Installation

LogOverNet is only available for Windows. To attach the logging library in your own code, open up the sln file and build both projects.
Because of pdcurses, the server (NetworkClient) needs to be built in **x86 mode**. In your own project, link the generated lib file and
include all the header files within the `NetworkLoggingLibrary\NetworkLoggingLibrary` folder. See below for logging functions.

To start the server, simply run the generated `NetworkClient.exe` file on the same or different machine. The server can take 2 command
line parameters:

    NetworkClient.exe --port 1234 --file myfile.log

Make sure the port number is the same as in the code. By using `--file` you can have multiple log files and these can then be reloaded at a later time using the same command.

## Dependencies

The logging library doesn't require any external dependencies as all functionality is provided by Winsock. The library should work
with C++11 and higher.

The server requires pdcurses.dll which provides the console UI.

## Logging Functions

To see a demo, open up the main.cpp file within the NetworkLoggingLibrary project.
```cpp
#include "logovernet.h"
using namespace logovernet;
```
Include the header file within any file you are logging from.

```cpp
Logger logger = Logger("127.0.0.1", DEFAULT_PORT);
```
To start logging, create a Logger class instance with the destination ip and port number. `DEFAULT_PORT` corresponds to 6881.

```cpp
logger.sendMessage("info message", LOG_SEVERITY::LON_INFO);
```
To send log messages, use the sendMessage function. 4 severity levels are available: LON_DEBUG, LON_INFO, LON_WARNING and LON_ERROR.

```cpp
logger << "ABCDEFGHIJKLMNOPQRSTUVWXYZ" << "abcdefghijklmnopqrstuvwxyz" << 123456789 << LON_EOL();
```
You can also use C++ streams which use LON_INFO severity. Use LON_EOL() to send the log message.

```cpp
setGlobalLogger(logger);
for (lon_int i = 0; i < 5; ++i)
{
    std::cout << "i is " << i << std::endl;
}
```
You can log automatically when the value of a variable is changed. First set the global logging instance. Then replace the type with a 
logging variant (in this case replace int with lon_int).

```cpp
LonType<std::string> myString("myMessage");
```
If the type is not available, you can use the LonType template class with any base class.
