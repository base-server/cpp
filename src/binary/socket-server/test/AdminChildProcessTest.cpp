#include "../AdminChildProcess.h"
#include "EnvironmentVariable.h"
#include "Singleton.h"
#include "ThreadPool.h"
#include "test.h"
#include "gtest/gtest.h"
#include <chrono>
#include <thread>

using namespace std;

class AdminChildProcessTest : public ::testing::Test {
	protected:
		void SetUp() override {
			int argc = 3;
			char *argv[] = {(char *)"./AdminChildProcessTest", (char *)"-c",
							(char *)CONFIG_PATH.c_str()};

			ASSERT_TRUE(Singleton<EnvironmentVariable>::Instance().Initialize(
				argc, argv));
		}

		void TearDown() override {}
};

TEST_F(AdminChildProcessTest, total) {
	AdminChildProcess process;

	ThreadPool threadPool(1);
	auto job = threadPool.AddJob([&process]() { return process.Start(); });
	this_thread::sleep_for(chrono::seconds(1));

	admin_command_test();

	EXPECT_TRUE(process.Stop());

	EXPECT_TRUE(job.get());
}
