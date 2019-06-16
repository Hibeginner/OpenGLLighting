#ifndef  __TIMER_H_INCLUDED__
#define __TIMER_H_INCLUDED__

#include <Windows.h>

class  HiResTimer
{
public:
	 HiResTimer();
	~ HiResTimer();

	bool Init();

	float GetElapsedSecond(unsigned long elapsedFrames = 1);
private:
	LARGE_INTEGER m_startTime;
	LARGE_INTEGER m_ticksPerSecond;
};
 
#endif // ! __TIMER_H_INCLUDED__
