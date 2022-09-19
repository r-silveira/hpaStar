/* -* c++ *- PathPlanning - 2007 Renato Silveira
 *	
 *	Log.h
 *
 *
 *				created 27/06/2008
 */

#ifndef LOG_H
#define LOG_H

#include <string>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

/** For benchmark log macros dump null comand */
#ifdef __RUN_BENCHMARK__
#endif

/** Array of logs */
class Log;
typedef vector<Log*> LogArray;

/**
 * Responsible to log messages on the application.
 * Default output is stdout.
 *
 * \author Giovane Kuhn <grkuhn at gmail dot com>
 * \date 2007-04-21
 */
class Log {
    public:

        /** Levels of log */
        enum LogLevel {
            LOG_NONE  = 0,
            LOG_FATAL = 1,  // Very severe errors that will probably lead the application to abort
            LOG_ERROR = 2,  // Errors that might still allow application to continue running
            LOG_WARN  = 3,  // Potentially harmful situations
            LOG_INFO  = 4,  // Messages that highlight the progress of the application at coarse-grained level
            LOG_DEBUG = 5,  // Fine-grained informational events that are most useful to debug an application
            LOG_TRACE = 6,   // Finer-grained informational events than the DEBUG
            LOG_ALL   = 7
        };

        static const string gLevelText[];

        virtual ~Log() { }

        static inline void trace(const string &message) { log(LOG_TRACE, message); }
        static inline void debug(const string &message) { log(LOG_DEBUG, message); }
        inline static void info(const string &message) { log(LOG_INFO, message); }
        inline static void warn(const string &message) { log(LOG_WARN, message); }
        inline static void error(const string &message) { log(LOG_ERROR, message); }
        inline static void fatal(const string &message) { log(LOG_FATAL, message); }
        inline static void log(const LogLevel level, const string &message) {
            if (level <= Log::gLevel) {
                gLogger->print(level, message);
            }
        }

        #define LOG(level, message) \
            if (level <= Log::getLogLevel()) { \
                ostringstream ret; \
                ret << message; \
                Log::getLogger()->print(level, ret.str()); \
            } \
			else \
				(void)0 

		#define LOG_NONE(message)  LOG(Log::LOG_NONE, message)
        #define LOG_FATAL(message) LOG(Log::LOG_FATAL, message)
        #define LOG_ERROR(message) LOG(Log::LOG_ERROR, message)
        #define LOG_WARN(message)  LOG(Log::LOG_WARN, message)
        #define LOG_INFO(message)  LOG(Log::LOG_INFO, message)
        #define LOG_DEBUG(message) LOG(Log::LOG_DEBUG, message)
        #define LOG_TRACE(message) LOG(Log::LOG_TRACE, message)

        /** Change current logger and free the old one. */
        static inline void setLogger(Log *logger_) { gLogger = logger_; }
        static inline Log* getLogger() { return gLogger; }

        static inline void setLogLevel(const LogLevel level_) { gLevel = level_; }
        static inline LogLevel getLogLevel() { return gLevel; }

        virtual void print(const LogLevel level, const string &message) { throw; }

    protected:
        Log() { }

    private:

        /** Instance to current logger */
        static Log *gLogger;

        /** Current log level */
        static LogLevel gLevel;
};

/**
 * Output logging messages to the air.
 *
 * \author Giovane Kuhn <grkuhn at gmail dot com>
 * \date 2007-04-21
 */
class LogNull: public Log {
    public:
        LogNull() { }
        virtual ~LogNull() { }
        inline void print(const LogLevel level, const string &message) { }

    protected:
    private:
};


/**
 * Output logging messages to the specified stream.
 *
 * \author Giovane Kuhn <grkuhn at gmail dot com>
 * \date 2007-04-21
 */
class LogStream: public Log {
    public:
        LogStream(ostream *stream_) { stream = stream_; }
        virtual ~LogStream() { }
        inline void print(const LogLevel level, const string &message) {
            *stream << "[" << Log::gLevelText[level] << "] " << message << std::endl;
        }

    protected:
    private:
        /** Stream to output messages */
        ostream *stream;
};

/**
 * Output logging messages to the chain of logging.
 *
 * \author Giovane Kuhn <grkuhn at gmail dot com>
 * \date 2007-09-06
 */
class LogChain: public Log {
    public:
        LogChain() { }
        virtual ~LogChain() { }

        void addLog(Log *log) { chain.push_back(log); }
        LogArray* getChain() { return &chain; }

        inline void print(const LogLevel level, const string &message) {
            for (LogArray::iterator iter=chain.begin(); iter!=chain.end(); iter++) {
                (*iter)->print(level, message);
            }
        }

    protected:
    private:
        /** Chain of loggings */
        LogArray chain;
};


#endif // LOG_H
