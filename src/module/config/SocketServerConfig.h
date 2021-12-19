#pragma once

#include "Config.h"

class SocketServerConfig : public Config {
private:
	int iAdminPort;
	int iAdminTimeout;

	int iUserPort;
	int iUserTimeout;
	int iUserJobPoolSize;

	virtual bool InitializeDerived();
public:
	SocketServerConfig();
	virtual ~SocketServerConfig() = default;

	int GetAdminPort();
	int GetAdminTimeout();

	int GetUserPort();
	int GetUserTimeout();
	int GetUserJobPoolSize();
};
