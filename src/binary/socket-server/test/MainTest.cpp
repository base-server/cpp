#include "test.h"

#include "../Main.h"

#include "gtest/gtest.h"

#include "Singleton.h"
#include "EnvironmentVariable.h"

static void test(int iArgc, char *pcArgv[])
{
	const int iPid = fork();
	ASSERT_NE(iPid, -1);

	extern int optind;
	optind = 1;

	if(iPid == 0) {
		EXPECT_TRUE(Main().Run(iArgc, pcArgv));

		exit(testing::Test::HasFailure());
	}
	this_thread::sleep_for(chrono::seconds(2));

	EXPECT_FALSE(Main().Run(iArgc, pcArgv));

	EXPECT_STREQ(send_command("stop", true).c_str(), "200 ok\r\n");

	this_thread::sleep_for(chrono::seconds(2));
}

TEST(MainTest, Run)
{
	EXPECT_FALSE(Main().Run(0, nullptr));
}

TEST(MainTest, StandAlone)
{
	int iArgc = 4;
	char *pcArgv[] = {(char *)"./MainTest", (char *)"-c", (char *)GstrConfigPath.c_str(), (char *)"-s"};

	test(iArgc, pcArgv);
}

TEST(MainTest, NonStandAlone)
{
	int iArgc = 3;
	char *pcArgv[] = {(char *)"./MainTest", (char *)"-c", (char *)GstrConfigPath.c_str()};

	test(iArgc, pcArgv);
}
