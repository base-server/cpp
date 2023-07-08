#include "SocketServerConfig.h"

SocketServerConfig::SocketServerConfig()
	: Config("socket_server.config"), adminPort(0), adminTimeout(0),
	  userPort(0), userTimeout(0), userJobPoolSize(0) {}

bool SocketServerConfig::InitializeDerived() {
	this->adminPort = this->json->GetValue<int64_t>({"admin_port"});
	this->adminTimeout = this->json->GetValue<int64_t>({"admin_timeout"});

	this->userPort = this->json->GetValue<int64_t>({"user_port"});
	this->userTimeout = this->json->GetValue<int64_t>({"user_timeout"});
	this->userJobPoolSize =
		this->json->GetValue<int64_t>({"user_job_pool_size"});

	return true;
}

int SocketServerConfig::GetAdminPort() const { return this->adminPort; }

int SocketServerConfig::GetAdminTimeout() const { return this->adminTimeout; }

int SocketServerConfig::GetUserPort() const { return this->userPort; }

int SocketServerConfig::GetUserTimeout() const { return this->userTimeout; }

int SocketServerConfig::GetUserJobPoolSize() const {
	return this->userJobPoolSize;
}
