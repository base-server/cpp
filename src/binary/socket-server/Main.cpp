#include "Main.h"
#include "AdminChildProcess.h"
#include "EnvironmentVariable.h"
#include "ParentProcess.h"
#include "Singleton.h"
#include "UserChildProcess.h"
#include <cstdio>
#include <memory>
#include <vector>

using namespace std;

bool Main::Run(int argc, char *argv[]) {
	vector<unique_ptr<ChildProcess>> processes = {};
	processes.push_back(make_unique<AdminChildProcess>());
	processes.push_back(make_unique<UserChildProcess>());

	if (Singleton<EnvironmentVariable>::Instance().Initialize(argc, argv) ==
		false) {
		printf("%s\n",
			   Singleton<EnvironmentVariable>::Instance().Usage().c_str());
		return false;
	}

	const auto standAlone =
		Singleton<EnvironmentVariable>::Instance().GetStandAlone();
	const auto binaryName =
		Singleton<EnvironmentVariable>::Instance().GetBinaryName();
	if (ParentProcess(standAlone, binaryName, processes).Start() == false) {
		printf("ParentProcess Start fail");
		return false;
	}

	return true;
}
