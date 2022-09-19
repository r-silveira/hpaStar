#include "Timer.h"
#include "Log.h"

#include <ctime>
#include <windows.h>

#define GNU 0

Timer::Timer()
:timeElapsed(0)
,startTime(-1)
,stoped(true)
{
}

Timer::~Timer() {
}

void Timer::start() {
	if( stoped ) {
		startTime = getTime();
		stoped = false;
	}
}

void Timer::stop() {
	if( !stoped ) {
		timeElapsed += getTime() - startTime;
		stoped = true;
	}
}

void Timer::reset() {
	timeElapsed = 0;
}

double Timer::elapsed() const {
	if ( stoped ) {
        return timeElapsed;
    }
	else
		return timeElapsed + getTime() - startTime;
}

double Timer::getTime() const {
#if GNU
    return clock()/double(CLOCKS_PER_SEC);
#else
    static bool init = false;
    static LARGE_INTEGER gCountFrequency;
    if (!init) {
        QueryPerformanceFrequency(&gCountFrequency);
    }
    LARGE_INTEGER countTotalTime;
    QueryPerformanceCounter(&countTotalTime);
    return countTotalTime.QuadPart / double(gCountFrequency.QuadPart);
#endif
}
