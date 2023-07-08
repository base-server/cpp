#pragma once

#include "Config.h"

class SocketServerConfig : public Config {
	private:
		int adminPort;
		int adminTimeout;

		int userPort;
		int userTimeout;
		int userJobPoolSize;

		virtual bool InitializeDerived() final;

	public:
		SocketServerConfig();
		virtual ~SocketServerConfig() = default;

		int GetAdminPort() const;
		int GetAdminTimeout() const;

		int GetUserPort() const;
		int GetUserTimeout() const;
		int GetUserJobPoolSize() const;
};
