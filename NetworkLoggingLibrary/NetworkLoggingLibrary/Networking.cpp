#include "Networking.h"

namespace logovernet
{
	int Network::sendMsg(SOCKET socket, char * msg, int msgSize)
	{
		return send(socket, msg, msgSize, 0);
	}
}
