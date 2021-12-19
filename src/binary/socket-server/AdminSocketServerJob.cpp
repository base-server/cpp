#include <csignal>
#include <cstring>
using namespace std;

#include "trim.h"
#include "FileLog.h"
#include "SocketServerConfig.h"
#include "EnvironmentVariable.h"

#include "AdminSocketServerJob.h"

bool AdminSocketServerJob::Start()
{
	DEBUG_G(__PRETTY_FUNCTION__);

	auto job = [](const SocketClient &socketClient) {
		struct Command {
			string strName;
			string strDescription;
			function<bool(const SocketClient &socketClient, bool &bContinue)> funcJob;
		};

		const map<string, Command> mapCommand = {
				{"log_reconfigure",
					{
						"log_reconfigure",
						"log reconfigure",
						[](const SocketClient &socketClient, bool &bContinue) -> bool {
							if(Singleton<EnvironmentVariable>::Instance().InitializeLog() == false) {
								ERROR_L_G("InitializeLog fail");
								return false;
							}

							return true;
						}
					}
				},
				{"stop",
					{
						"stop",
						"process stop",
						[](const SocketClient &socketClient, bool &bContinue) -> bool {
							bContinue = false;

							raise(SIGTERM);

							while(Singleton<EnvironmentVariable>::Instance().GetCondition()) {
							}

							return true;
						}
					}
				},
				{"quit",
					{
						"quit",
						"connect end",
						[](const SocketClient &socketClient, bool &bContinue) -> bool {
							bContinue = false;
							return true;
						}
					}
				},
				{"help",
					{
						"help",
						"print command",
						nullptr,
					}
				},
			};

		const string strLogPrefix = "[" + socketClient.GetPeerAddress() + "]" +
									"[" + to_string(socketClient.GetPeerPort()) + "]" +
									" admin socket";

		INFO_G("%s start", strLogPrefix.c_str());

		socketClient.Write("=== greeting ===\r\n");

		while(true) {
			bool bEnd = false;
			string strCommand = "";

			if(socketClient.Read(strCommand, 1024, bEnd) == false) {
				if(errno == ETIMEDOUT) {
					INFO_G("%s timeout", strLogPrefix.c_str());
				} else {
					ERROR_L_G("%s read fail - error : (%s)", strLogPrefix.c_str(), strerror(errno));
				}

				break;
			}
			if(bEnd == false) {
				socketClient.ReadGarbage();
				socketClient.Write("500 too long line\r\n");
				continue;
			}

			trim(strCommand);

			INFO_G("%s command : (%s)", strLogPrefix.c_str(), strCommand.c_str());

			if(mapCommand.find(strCommand) == mapCommand.end()) {
				socketClient.Write("500 invalid command\r\n");
				continue;
			}

			bool bResult = false;
			bool bContinue = true;
			string strPrefixWrite = "";

			for(const auto &iter : mapCommand) {
				if(strCommand == "help") {
					bResult = true;
					strPrefixWrite += iter.second.strName + " : " + iter.second.strDescription + "\r\n";
				} else if(strCommand == iter.first) {
					bResult = iter.second.funcJob(socketClient, bContinue);

					break;
				}
			}

			if(bResult) {
				socketClient.Write(strPrefixWrite + "200 ok\r\n");
			} else {
				socketClient.Write("500 fail\r\n");
			}

			if(bContinue == false) {
				break;
			}
		}

		INFO_G("%s end", strLogPrefix.c_str());
	};

	SocketServerConfig socketServerConfig;
	if(socketServerConfig.Initialize(Singleton<EnvironmentVariable>::Instance().GetConfigPath()) == false) {
		ERROR_L_G("SocketServerConfig Initialize fail");
		return false;
	}

	return this->socketServer.Start(socketServerConfig.GetAdminPort(), socketServerConfig.GetAdminTimeout(), 1, job);
}

bool AdminSocketServerJob::Stop()
{
	DEBUG_G(__PRETTY_FUNCTION__);

	if(this->socketServer.Stop() == false) {
		ERROR_L_G("socketServer stop fail");
	}

	return true;
}
