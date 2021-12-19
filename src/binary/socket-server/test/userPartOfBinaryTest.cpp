#include "test.h"

#include "gtest/gtest.h"

static void test(const string &strCommand)
{
	const string strResponse = "[response] " + strCommand + "\r\n";

	EXPECT_STREQ(send_command(strCommand, false).c_str(), strResponse.c_str());
}

TEST(userBinaryTest, command)
{
	test("aaa");
	test("bbb");
	test("ccc");
}

TEST(userBinaryTest, quit)
{
	EXPECT_STREQ(send_command("quit", false).c_str(), "");
}
