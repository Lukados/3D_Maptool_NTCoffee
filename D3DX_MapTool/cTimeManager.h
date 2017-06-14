#pragma once

#define TIME cTimeManager::GetInstance()

class cTimeManager
{
private:
	SINGLETON(cTimeManager);

private:
	LONGLONG m_frequency;		// �ʴ� ���ļ�
	LONGLONG m_startCount;		// ���� ī��Ʈ
	LONGLONG m_currentCount;	// ���� ī��Ʈ
	LONGLONG m_previousCount;	// ���� ī��Ʈ

	float m_frequencyTime;		// ���ļ��� �ð�
	float m_frameTime;			// �����Ӵ� �ð�
	float m_deltaTime;

	int m_fps;					// �ʴ� ������ ��
public:

	void Setup(int fps);
	bool Update();

	int FPS() { return m_fps; }
	float StartTime() { return m_startCount * m_frequencyTime; }
	float CurrentTime() { return m_currentCount * m_frequencyTime; }
	float DeltaTime() { return m_deltaTime; }
};

