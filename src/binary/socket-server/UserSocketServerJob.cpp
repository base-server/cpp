#include <cstring>
using namespace std;

#include "trim.h"
#include "FileLog.h"
#include "SocketServerConfig.h"
#include "EnvironmentVariable.h"

#include "UserSocketServerJob.h"

bool UserSocketServerJob::Start()
{
	DEBUG_G(__PRETTY_FUNCTION__);

	auto job = [](const SocketClient &socketClient) {
		const string strLogPrefix = "[" + socketClient.GetPeerAddress() + "]" +
									"[" + to_string(socketClient.GetPeerPort()) + "]" +
									" user socket";

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

			if(strCommand == "quit") {
				break;
			}

			socketClient.Write("[response] " + strCommand + "\r\n");
		}

		INFO_G("%s end", strLogPrefix.c_str());
	};

	SocketServerConfig socketServerConfig;
	if(socketServerConfig.Initialize(Singleton<EnvironmentVariable>::Instance().GetConfigPath()) == false) {
		ERROR_L_G("SocketServerConfig Initialize fail");
		return false;
	}

	return this->socketServer.Start(socketServerConfig.GetUserPort(), socketServerConfig.GetUserTimeout(), socketServerConfig.GetUserJobPoolSize(), job);
}

bool UserSocketServerJob::Stop()
{
	DEBUG_G(__PRETTY_FUNCTION__);

	if(this->socketServer.Stop() == false) {
		ERROR_L_G("socketServer stop fail");
	}

	return true;
}
