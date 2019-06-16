#include "stdafx.h"

HiResTimer::HiResTimer()
{
}

HiResTimer::~HiResTimer()
{
}

bool HiResTimer::Init() {
	if (!QueryPerformanceFrequency(&m_ticksPerSecond)) {
		//未查询到系统的高精度定时器
		return false;
	}
	else {
		QueryPerformanceCounter(&m_startTime);//定时器给出来的计数值
	}
	return true;
}

float HiResTimer::GetElapsedSecond(unsigned long elapsedFrames) {
	static LARGE_INTEGER s_lastTime = m_startTime;
	LARGE_INTEGER currentTime;

	QueryPerformanceCounter(&currentTime);

	float seconds = ((float)currentTime.QuadPart - (float)s_lastTime.QuadPart) / (float)m_ticksPerSecond.QuadPart;
	s_lastTime = currentTime;

	return seconds;
}