#include "stdafx.h"
#include "cTimeManager.h"


cTimeManager::cTimeManager()
{
}


cTimeManager::~cTimeManager()
{
}

void cTimeManager::Setup(int fps)
{
	// 초당 주파수 계산
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_frequency);

	// 주파수당 시간 계산
	m_frequencyTime = 1.0 / m_frequency;

	// 시작 카운트 계산
	QueryPerformanceCounter((LARGE_INTEGER*)&m_startCount);
	m_currentCount = m_startCount;
	m_previousCount = m_startCount;

	// 프레임당 시간 계산
	m_fps = fps;
	m_frameTime = 1.0 / fps;
}

bool cTimeManager::Update()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&m_currentCount);

	m_deltaTime = (m_currentCount - m_previousCount) * m_frequencyTime;

	if (m_deltaTime > m_frameTime)
	{
		m_previousCount = m_currentCount;
		return true;
	}
	return false;
}