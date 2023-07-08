#include "Main.h"

int main(int argc, char *argv[]) {
	if (Main().Run(argc, argv) == false) {
		return -1;
	}

	return 0;
}
