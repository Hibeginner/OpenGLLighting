#include "stdafx.h"

HiResTimer::HiResTimer()
{
}

HiResTimer::~HiResTimer()
{
}

bool HiResTimer::Init() {
	if (!QueryPerformanceFrequency(&m_ticksPerSecond)) {
		//δ��ѯ��ϵͳ�ĸ߾��ȶ�ʱ��
		return false;
	}
	else {
		QueryPerformanceCounter(&m_startTime);//��ʱ���������ļ���ֵ
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