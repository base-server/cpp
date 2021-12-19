#include "test.h"

#include "../SocketServerConfig.h"

#include "gtest/gtest.h"

static SocketServerConfig get_config()
{
	SocketServerConfig socketServerConfig;

	EXPECT_TRUE(socketServerConfig.Initialize(GstrConfigPath));

	return socketServerConfig;
}

TEST(SocketServerConfigTest, Initialize)
{
	SocketServerConfig socketServerConfig;

	EXPECT_TRUE(socketServerConfig.Initialize(GstrConfigPath));
	EXPECT_FALSE(socketServerConfig.Initialize(""));
}

TEST(SocketServerConfigTest, GetAdminPort)
{
	EXPECT_EQ(get_config().GetAdminPort(), 10000);
}

TEST(SocketServerConfigTest, GetAdminTimeout)
{
	EXPECT_EQ(get_config().GetAdminTimeout(), 3);
}

TEST(SocketServerConfigTest, GetUserPort)
{
	EXPECT_EQ(get_config().GetUserPort(), 10001);
}

TEST(SocketServerConfigTest, GetUserTimeout)
{
	EXPECT_EQ(get_config().GetUserTimeout(), 3);
}

TEST(SocketServerConfigTest, GetUserJobPoolSize)
{
	EXPECT_EQ(get_config().GetUserJobPoolSize(), 10);
}
