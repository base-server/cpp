#pragma once

#include "ChildProcess.h"
#include "SocketServer.h"
#include <memory>

using namespace std;

class AdminChildProcess : public ChildProcess {
	private:
		unique_ptr<SocketServer> socketServer;

		virtual bool Initialize() override final;
		virtual bool Finalize() override final;

		virtual bool Job() override final;

	public:
		AdminChildProcess() = default;
		virtual ~AdminChildProcess() = default;
};
