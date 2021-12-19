#include "test.h"

#include "../SocketServerChildProcess.h"

#include "gtest/gtest.h"

#include "Singleton.h"
#include "EnvironmentVariable.h"

class SocketServerChildProcessTest : public ::testing::Test {
protected:
	void SetUp() override {
		extern int optind;
		optind = 1;

		int iArgc = 4;
		char *pcArgv[] = {(char *)"./SocketServerChildProcessTest", (char *)"-c", (char *)GstrConfigPath.c_str(), (char *)"-s"};

		EXPECT_TRUE(Singleton<EnvironmentVariable>::Instance().Initialize(iArgc, pcArgv));
	}

	void TearDown() override {}
};

static void admin_command_test()
{
	EXPECT_STREQ(send_command("log_reconfigure", true).c_str(), "200 ok\r\n");

	EXPECT_STREQ(send_command("quit", true).c_str(), "200 ok\r\n");

	const string strHelpResult = string("") +
								"help : print command\r\n" +
								"log_reconfigure : log reconfigure\r\n" +
								"quit : connect end\r\n" +
								"stop : process stop\r\n" +
								"200 ok\r\n";
	EXPECT_STREQ(send_command("help", true).c_str(), strHelpResult.c_str());
}

static void user_command_test()
{
	const string strCommand = "aaa";
	const string strResponse = "[response] " + strCommand + "\r\n";
	EXPECT_STREQ(send_command(strCommand, false).c_str(), strResponse.c_str());

	EXPECT_STREQ(send_command("quit", false).c_str(), "");
}

TEST_F(SocketServerChildProcessTest, Start)
{
	ThreadPool threadPool(1);
	SocketServerChildProcess socketServerChildProcess;

	future<bool> future = threadPool.AddJob([&]()->bool{return socketServerChildProcess.Start();});
	this_thread::sleep_for(chrono::seconds(1));

	admin_command_test();
	user_command_test();

	EXPECT_TRUE(socketServerChildProcess.Stop());

	EXPECT_TRUE(future.get());
}

TEST_F(SocketServerChildProcessTest, Stop)
{
	SocketServerChildProcess socketServerChildProcess;
	EXPECT_TRUE(socketServerChildProcess.Stop());
}

TEST_F(SocketServerChildProcessTest, SigTerm)
{
	const int iPid = fork();
	ASSERT_NE(iPid, -1);

	if(iPid == 0) {
		ThreadPool threadPool(1);
		SocketServerChildProcess socketServerChildProcess;

		future<bool> future = threadPool.AddJob([&]()->bool{return socketServerChildProcess.Start();});
		this_thread::sleep_for(chrono::seconds(1));

		EXPECT_EQ(raise(SIGTERM), 0);

		EXPECT_TRUE(future.get());

		exit(testing::Test::HasFailure());
	}

	int iStatus = -1;
	EXPECT_EQ(waitpid(iPid, &iStatus, 0), iPid);
	EXPECT_NE(WIFEXITED(iStatus), 0);
	EXPECT_EQ(WIFSIGNALED(iStatus), 0);
	EXPECT_EQ(WTERMSIG(iStatus), 0);
}
