#include "FileLog.h"
#include "JsonManager.h"

#include "SocketServerConfig.h"

SocketServerConfig::SocketServerConfig()
	: Config("socket_server.config"),
		iAdminPort(0),
		iAdminTimeout(0),
		iUserPort(0),
		iUserTimeout(0),
		iUserJobPoolSize(0)
{
	DEBUG_G(__PRETTY_FUNCTION__);
}

bool SocketServerConfig::InitializeDerived()
{
	DEBUG_G(__PRETTY_FUNCTION__);

	this->iAdminPort = this->jsonManager.GetValue<int>({"admin_port"});
	this->iAdminTimeout = this->jsonManager.GetValue<int>({"admin_timeout"});

	this->iUserPort = this->jsonManager.GetValue<int>({"user_port"});
	this->iUserTimeout = this->jsonManager.GetValue<int>({"user_timeout"});
	this->iUserJobPoolSize = this->jsonManager.GetValue<int>({"user_job_pool_size"});

	return true;
}

int SocketServerConfig::GetAdminPort()
{
	return this->iAdminPort;
}

int SocketServerConfig::GetAdminTimeout()
{
	return this->iAdminTimeout;
}

int SocketServerConfig::GetUserPort()
{
	return this->iUserPort;
}

int SocketServerConfig::GetUserTimeout()
{
	return this->iUserTimeout;
}

int SocketServerConfig::GetUserJobPoolSize()
{
	return this->iUserJobPoolSize;
}
