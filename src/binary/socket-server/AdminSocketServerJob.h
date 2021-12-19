#pragma once

#include "SocketServer.h"

class AdminSocketServerJob {
private:
	SocketServer socketServer;
public:
	AdminSocketServerJob() = default;
	virtual ~AdminSocketServerJob() = default;

	bool Start();
	bool Stop();
};
