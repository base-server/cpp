#include "AdminChildProcess.h"
#include "EnvironmentVariable.h"
#include "SocketServerConfig.h"
#include "define.h"
#include "trim.h"
#include <csignal>
#include <cstring>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <thread>

using namespace std;

bool AdminChildProcess::Initialize() {
	DEBUG(__PRETTY_FUNCTION__);

	this->socketServer = make_unique<SocketServer>();

	if (log_initialize() == false) {
		return false;
	}

	return true;
}

bool AdminChildProcess::Finalize() {
	DEBUG(__PRETTY_FUNCTION__);

	if (this->socketServer->Stop() == false) {
		ERROR("SocketServer stop fail");
		return false;
	}

	this->socketServer.reset(nullptr);

	return true;
}

bool AdminChildProcess::Job() {
	DEBUG(__PRETTY_FUNCTION__);

	auto job = [](const SocketClient &socketClient) {
		struct Command {
				string name;
				string description;
				function<bool(const SocketClient &socketClient,
							  bool &isContinue)>
					funcJob;
		};

		const map<string, Command> commandInfo = {
			{"stop",
			 {"stop", "process stop",
			  [](const SocketClient &socketClient, bool &isContinue) {
				  isContinue = false;

				  kill(getppid(), SIGTERM);

				  return true;
			  }}},
			{"quit",
			 {"quit", "connection end",
			  [](const SocketClient &socketClient, bool &isContinue) {
				  isContinue = false;
				  return true;
			  }}},
			{"help",
			 {
				 "help",
				 "print command",
				 nullptr,
			 }},
		};

		const string prefixLog = "[" + socketClient.GetPeerAddress() + "]" +
								 "[" + to_string(socketClient.GetPeerPort()) +
								 "]" + " admin socket";

		INFO("{} start", prefixLog);

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
					ERROR("{} read fail - error : ({})", prefixLog.c_str(),
						  strerror(errno));
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

			if (commandInfo.find(command) == commandInfo.end()) {
				socketClient.Write("500 invalid command\r\n");
				continue;
			}

			bool success = false;
			bool isContinue = true;
			string prefixResponse = "";

			for (const auto &iter : commandInfo) {
				if (command == "help") {
					success = true;
					prefixResponse += iter.second.name + " : " +
									  iter.second.description + "\r\n";
				} else if (command == iter.first) {
					success = iter.second.funcJob(socketClient, isContinue);

					break;
				}
			}

			const string response =
				success ? prefixResponse + "200 ok\r\n" : "500 fail\r\n";
			if (socketClient.Write(response) == false) {
				ERROR("{} write fail - message : ({}), error : ({})({})",
					  prefixLog.c_str(), response, errno, strerror(errno));
				continue;
			}

			if (isContinue == false) {
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

	if (this->socketServer->Start(socketServerConfig.GetAdminPort(),
								  socketServerConfig.GetAdminTimeout(), 1,
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
