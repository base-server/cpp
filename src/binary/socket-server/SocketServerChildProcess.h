#pragma once

#include "UserSocketServerJob.h"
#include "AdminSocketServerJob.h"

#include "ChildProcess.h"

class SocketServerChildProcess : public ChildProcess {
private:
	UserSocketServerJob userSocketServerJob;
	AdminSocketServerJob adminSocketServerJob;

	virtual bool InitializeDerived();
	virtual bool FinalizeDerived();
	virtual bool Job();
public:
	SocketServerChildProcess() = default;
	virtual ~SocketServerChildProcess() = default;
};
