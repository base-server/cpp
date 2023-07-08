#include "../Main.h"
#include "test.h"
#include "gtest/gtest.h"
#include <chrono>
#include <cstdlib>
#include <future>
#include <thread>
#include <vector>

using namespace std;

static void test(int argc, char *argv[]) {
	const int pid = fork();
	ASSERT_NE(pid, -1);

	if (pid == 0) {
		EXPECT_TRUE(Main().Run(argc, argv));

		exit(testing::Test::HasFailure());
	}
	this_thread::sleep_for(chrono::seconds(1));

	EXPECT_FALSE(Main().Run(argc, argv));

	vector<future<void>> jobs = {};
	jobs.push_back(async(launch::async, []() { admin_command_test(); }));
	jobs.push_back(async(launch::async, []() { user_command_test(); }));
	for (auto &iter : jobs) {
		if (iter.valid()) {
			iter.get();
		}
	}
	jobs.clear();

	EXPECT_STREQ(send_command("stop", true).c_str(), "200 ok\r\n");

	EXPECT_TRUE(wait_process(pid));
}

TEST(MainTest, Run_1) {
	int argc = 2;
	char *argv[] = {(char *)"./MainTest", (char *)"-c",
					(char *)CONFIG_PATH.c_str()};

	EXPECT_FALSE(Main().Run(argc, argv));
}

TEST(MainTest, Run_2) {
	int argc = 3;
	char *argv[] = {(char *)"./MainTest", (char *)"-c",
					(char *)CONFIG_PATH.c_str()};

	test(argc, argv);
}
