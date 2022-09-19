#include "Log.h"

const string Log::gLevelText[Log::LOG_ALL+1] = {
    "",
    "FATAL",
    "ERROR",
    "WARN ",
    "INFO ",
    "DEBUG",
    "TRACE",
    "ALL  "
};

Log *Log::gLogger = static_cast<Log*>(new LogNull());

#if defined(__LOG_NONE__)
	Log::LogLevel Log::gLevel = Log::LOG_NONE;
#elif defined(__LOG_FATAL__)
	Log::LogLevel Log::gLevel = Log::LOG_FATAL;
#elif defined(__LOG_ERROR__)
	Log::LogLevel Log::gLevel = Log::LOG_ERROR;
#elif defined(__LOG_WARN__)
	Log::LogLevel Log::gLevel = Log::LOG_WARN;
#elif defined(__LOG_INFO__)
	Log::LogLevel Log::gLevel = Log::LOG_INFO;
#elif defined(__LOG_DEBUG__)
	Log::LogLevel Log::gLevel = Log::LOG_DEBUG;
#elif defined(__LOG_TRACE__)
	Log::LogLevel Log::gLevel = Log::LOG_TRACE;
#else
	Log::LogLevel Log::gLevel = Log::LOG_ALL;
#endif
