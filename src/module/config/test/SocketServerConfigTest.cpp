#include "../SocketServerConfig.h"
#include "test.h"
#include "gtest/gtest.h"
#include <memory>

using namespace std;

static unique_ptr<SocketServerConfig> get_config() {
	auto config = make_unique<SocketServerConfig>();

	EXPECT_TRUE(config->Initialize(CONFIG_PATH));

	return config;
}

TEST(SocketServerConfigTest, Initialize) {
	SocketServerConfig socketServerConfig;

	EXPECT_TRUE(socketServerConfig.Initialize(CONFIG_PATH));
	EXPECT_FALSE(socketServerConfig.Initialize(""));
}

TEST(SocketServerConfigTest, GetAdminPort) {
	EXPECT_EQ(get_config()->GetAdminPort(), 10000);
}

TEST(SocketServerConfigTest, GetAdminTimeout) {
	EXPECT_EQ(get_config()->GetAdminTimeout(), 2);
}

TEST(SocketServerConfigTest, GetUserPort) {
	EXPECT_EQ(get_config()->GetUserPort(), 10001);
}

TEST(SocketServerConfigTest, GetUserTimeout) {
	EXPECT_EQ(get_config()->GetUserTimeout(), 2);
}

TEST(SocketServerConfigTest, GetUserJobPoolSize) {
	EXPECT_EQ(get_config()->GetUserJobPoolSize(), 10);
}
