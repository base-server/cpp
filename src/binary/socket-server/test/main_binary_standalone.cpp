#include <thread>
using namespace std;

#include "test.h"

#include "gtest/gtest.h"

int main(int iArgc, char *pcArgv[])
{
	testing::InitGoogleTest(&iArgc, pcArgv);

	return main_sub(true);
}
