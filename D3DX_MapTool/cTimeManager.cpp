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
	// �ʴ� ���ļ� ���
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_frequency);

	// ���ļ��� �ð� ���
	m_frequencyTime = 1.0 / m_frequency;

	// ���� ī��Ʈ ���
	QueryPerformanceCounter((LARGE_INTEGER*)&m_startCount);
	m_currentCount = m_startCount;
	m_previousCount = m_startCount;

	// �����Ӵ� �ð� ���
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