#pragma once

#define TIME cTimeManager::GetInstance()

class cTimeManager
{
private:
	SINGLETON(cTimeManager);

private:
	LONGLONG m_frequency;		// 초당 주파수
	LONGLONG m_startCount;		// 시작 카운트
	LONGLONG m_currentCount;	// 현재 카운트
	LONGLONG m_previousCount;	// 이전 카운트

	float m_frequencyTime;		// 주파수당 시간
	float m_frameTime;			// 프레임당 시간
	float m_deltaTime;

	int m_fps;					// 초당 프레임 수
public:

	void Setup(int fps);
	bool Update();

	int FPS() { return m_fps; }
	float StartTime() { return m_startCount * m_frequencyTime; }
	float CurrentTime() { return m_currentCount * m_frequencyTime; }
	float DeltaTime() { return m_deltaTime; }
};

