#pragma once

#include "SocketServer.h"

class UserSocketServerJob {
private:
	SocketServer socketServer;
public:
	UserSocketServerJob() = default;
	virtual ~UserSocketServerJob() = default;

	bool Start();
	bool Stop();
};
