#include "test.h"

#include "../AdminSocketServerJob.h"

#include "gtest/gtest.h"

#include "Singleton.h"
#include "EnvironmentVariable.h"

class AdminSocketServerJobTest : public ::testing::Test {
protected:
	void SetUp() override {
		extern int optind;
		optind = 1;

		int iArgc = 4;
		char *pcArgv[] = {(char *)"./AdminSocketServerJobTest", (char *)"-c", (char *)GstrConfigPath.c_str(), (char *)"-s"};

		EXPECT_TRUE(Singleton<EnvironmentVariable>::Instance().Initialize(iArgc, pcArgv));
	}

	void TearDown() override {}
};

static void command_test()
{
	const string strHelpResult = string("") +
								"help : print command\r\n" +
								"log_reconfigure : log reconfigure\r\n" +
								"quit : connect end\r\n" +
								"stop : process stop\r\n" +
								"200 ok\r\n";
	EXPECT_STREQ(send_command("help", true).c_str(), strHelpResult.c_str());

	EXPECT_STREQ(send_command("log_reconfigure", true).c_str(), "200 ok\r\n");

	EXPECT_STREQ(send_command("quit", true).c_str(), "200 ok\r\n");

	EXPECT_STREQ(send_command("aaa", true).c_str(), "500 invalid command\r\n");

	EXPECT_STREQ(send_command("timeout", true).c_str(), "");

	EXPECT_STREQ(send_command("too_long_line", true).c_str(), "500 too long line\r\n");
}

TEST_F(AdminSocketServerJobTest, command_stop)
{
	const int iPid = fork();
	ASSERT_NE(iPid, -1);

	if(iPid == 0) {
		static AdminSocketServerJob adminSocketServerJob;

		Singleton<EnvironmentVariable>::Instance().SetCondition(true);
		sigset(SIGTERM, [](int iSig){
							Singleton<EnvironmentVariable>::Instance().SetCondition(false);
							adminSocketServerJob.Stop();
						});

		EXPECT_TRUE(adminSocketServerJob.Start());

		exit(testing::Test::HasFailure());
	}
	this_thread::sleep_for(chrono::seconds(1));

	EXPECT_STREQ(send_command("stop", true).c_str(), "200 ok\r\n");

	this_thread::sleep_for(chrono::seconds(1));
}

TEST_F(AdminSocketServerJobTest, Start)
{
	ThreadPool threadPool(1);

	AdminSocketServerJob adminSocketServerJob;

	future<bool> future = threadPool.AddJob([&]()->bool{return adminSocketServerJob.Start();});
	this_thread::sleep_for(chrono::seconds(1));

	command_test();

	EXPECT_TRUE(adminSocketServerJob.Stop());

	EXPECT_TRUE(future.get());
}

TEST_F(AdminSocketServerJobTest, Stop)
{
	AdminSocketServerJob adminSocketServerJob;
	EXPECT_TRUE(adminSocketServerJob.Stop());
}
