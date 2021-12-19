#include "FileLog.h"

#include "SocketServerChildProcess.h"

bool SocketServerChildProcess::InitializeDerived()
{
	DEBUG_G(__PRETTY_FUNCTION__);

	return true;
}

bool SocketServerChildProcess::FinalizeDerived()
{
	DEBUG_G(__PRETTY_FUNCTION__);

	if(this->adminSocketServerJob.Stop() == false) {
		ERROR_L_G("adminSocketServerJob stop fail");
	}

	if(this->userSocketServerJob.Stop() == false) {
		ERROR_L_G("userSocketServerJob stop fail");
	}

	return true;
}

bool SocketServerChildProcess::Job()
{
	DEBUG_G(__PRETTY_FUNCTION__);

	ThreadPool threadPool(2);

	auto adminSocketServerFunc = [&]() -> bool {
		return this->adminSocketServerJob.Start();
	};

	auto userSocketServerFunc = [&]() -> bool {
		return this->userSocketServerJob.Start();
	};

	vector<future<bool>> vecFuture;
	vecFuture.clear();

	vecFuture.push_back(threadPool.AddJob(adminSocketServerFunc));
	vecFuture.push_back(threadPool.AddJob(userSocketServerFunc));

	for(auto &iter : vecFuture) {
		if(iter.valid()) {
			iter.get();
		}
	}

	return true;
}
