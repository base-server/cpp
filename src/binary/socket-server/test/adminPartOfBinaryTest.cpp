#include "test.h"

#include "gtest/gtest.h"

TEST(adminBinaryTest, invalid_command)
{
	EXPECT_STREQ(send_command("aaa", true).c_str(), "500 invalid command\r\n");
}

TEST(adminBinaryTest, log_reconfigure)
{
	EXPECT_STREQ(send_command("log_reconfigure", true).c_str(), "200 ok\r\n");
}

TEST(adminBinaryTest, quit)
{
	EXPECT_STREQ(send_command("quit", true).c_str(), "200 ok\r\n");
}

TEST(adminBinaryTest, help)
{
	const string strResult = string("") +
								"help : print command\r\n" +
								"log_reconfigure : log reconfigure\r\n" +
								"quit : connect end\r\n" +
								"stop : process stop\r\n" +
								"200 ok\r\n";

	EXPECT_STREQ(send_command("help", true).c_str(), strResult.c_str());
}
