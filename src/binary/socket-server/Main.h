#pragma once

class Main {
private:

public:
	Main() = default;
	virtual ~Main() = default;

	bool Run(int iArgc, char *pcArgv[]);
};
