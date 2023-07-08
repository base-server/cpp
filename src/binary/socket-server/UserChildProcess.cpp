#include "UserChildProcess.h"
#include "EnvironmentVariable.h"
#include "SocketServerConfig.h"
#include "define.h"
#include "trim.h"
#include <cstring>
#include <memory>
#include <string>
#include <thread>

using namespace std;

bool UserChildProcess::Initialize() {
	DEBUG(__PRETTY_FUNCTION__);

	this->socketServer = make_unique<SocketServer>();

	if (log_initialize() == false) {
		return false;
	}

	return true;
}

bool UserChildProcess::Finalize() {
	DEBUG(__PRETTY_FUNCTION__);

	if (this->socketServer->Stop() == false) {
		ERROR("SocketServer stop fail");
		return false;
	}

	this->socketServer.reset(nullptr);

	return true;
}

bool UserChildProcess::Job() {
	DEBUG(__PRETTY_FUNCTION__);

	auto job = [](const SocketClient &socketClient) {
		const string prefixLog = "[" + socketClient.GetPeerAddress() + "]" +
								 "[" + to_string(socketClient.GetPeerPort()) +
								 "]" + " user socket";

		INFO("{} start", prefixLog.c_str());

		const string greeting = "=== greeting ===\r\n";
		if (socketClient.Write(greeting) == false) {
			ERROR("{} write fail - message : ({}), error : ({})({})",
				  prefixLog.c_str(), greeting, errno, strerror(errno));
			return;
		}

		while (true) {
			bool end = false;
			const auto result = socketClient.Read(1024, end);

			if (get<0>(result) == false) {
				if (errno == ETIMEDOUT) {
					INFO("{} timeout", prefixLog.c_str());
				} else {
					ERROR("{} read fail - error : ({})({})", prefixLog.c_str(),
						  errno, strerror(errno));
				}

				break;
			}

			if (end == false) {
				socketClient.ReadGarbage();
				socketClient.Write("500 too long line\r\n");
				continue;
			}

			const string command = trim_copy(get<1>(result));

			INFO("{} command : ({})", prefixLog.c_str(), command.c_str());

			const string response = command == "quit"
										? "200 ok\r\n"
										: "[response] " + command + "\r\n";
			if (socketClient.Write(response) == false) {
				ERROR("{} write fail - message : ({}), error : ({})({})",
					  prefixLog.c_str(), response, errno, strerror(errno));
			}

			if (command == "quit") {
				break;
			}
		}

		INFO("{} end", prefixLog.c_str());
	};

	SocketServerConfig socketServerConfig;
	if (socketServerConfig.Initialize(
			Singleton<EnvironmentVariable>::Instance().GetConfigPath()) ==
		false) {
		ERROR("SocketServerConfig Initialize fail - error : ({})({})", errno,
			  strerror(errno));
		return false;
	}

	if (this->socketServer->Start(socketServerConfig.GetUserPort(),
								  socketServerConfig.GetUserTimeout(),
								  socketServerConfig.GetUserJobPoolSize(),
								  job) == false) {
		ERROR("SocketServer Start fail - error : ({})({})", errno,
			  strerror(errno));
		return false;
	}

	while (this->GetCondition()) {
		this_thread::sleep_for(1ms);
	}

	return true;
}
