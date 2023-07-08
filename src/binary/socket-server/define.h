#pragma once

#include "CommonConfig.h"
#include "FileLog.h"
#include "Singleton.h"
#include <cstring>
#include <format>

using namespace std;

#define DEBUG(args...) Singleton<FileLog>::Instance().Debug(format(args))
#define INFO(args...) Singleton<FileLog>::Instance().Info(format(args))
#define WARNING(args...) Singleton<FileLog>::Instance().Warning(format(args))
#define ERROR(args...) Singleton<FileLog>::Instance().Error(format(args))
#define CRITICAL(args...) Singleton<FileLog>::Instance().Critical(format(args))

static inline bool log_initialize();

static inline bool log_initialize() {
	CommonConfig commonConfig;
	if (commonConfig.Initialize(
			Singleton<EnvironmentVariable>::Instance().GetConfigPath()) ==
		false) {
		ERROR("CommonConfig Initialize fail - errno : ({})({})", errno,
			  strerror(errno));
		return false;
	}

	if (Singleton<FileLog>::Instance().Initialize(
			commonConfig.GetLogLevel(), commonConfig.GetLogOutputPath(),
			commonConfig.GetLogFileName(), commonConfig.GetLogLinePrint(),
			commonConfig.GetLogThreadMode()) == false) {
		ERROR("FileLog Initialize fail - errno : ({})({})", errno,
			  strerror(errno));
		return false;
	}

	return true;
}
