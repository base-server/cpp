#include "test.h"

#include "../UserSocketServerJob.h"

#include "gtest/gtest.h"

#include "Singleton.h"
#include "EnvironmentVariable.h"

class UserSocketServerJobTest : public ::testing::Test {
protected:
	void SetUp() override {
		extern int optind;
		optind = 1;

		int iArgc = 4;
		char *pcArgv[] = {(char *)"./UserSocketServerJobTest", (char *)"-c", (char *)GstrConfigPath.c_str(), (char *)"-s"};

		EXPECT_TRUE(Singleton<EnvironmentVariable>::Instance().Initialize(iArgc, pcArgv));
	}

	void TearDown() override {}
};

static void command_test()
{
	const string strCommand = "aaa";
	const string strResponse = "[response] " + strCommand + "\r\n";
	EXPECT_STREQ(send_command(strCommand, false).c_str(), strResponse.c_str());

	EXPECT_STREQ(send_command("quit", false).c_str(), "");

	EXPECT_STREQ(send_command("timeout", false).c_str(), "");

	EXPECT_STREQ(send_command("too_long_line", false).c_str(), "500 too long line\r\n");
}

TEST_F(UserSocketServerJobTest, Start)
{
	ThreadPool threadPool(1);

	UserSocketServerJob userSocketServerJob;

	future<bool> future = threadPool.AddJob([&]()->bool{return userSocketServerJob.Start();});
	this_thread::sleep_for(chrono::seconds(1));

	command_test();

	EXPECT_TRUE(userSocketServerJob.Stop());

	EXPECT_TRUE(future.get());
}

TEST_F(UserSocketServerJobTest, Stop)
{
	UserSocketServerJob userSocketServerJob;
	EXPECT_TRUE(userSocketServerJob.Stop());
}
