#pragma once

class cWeather
{
	SYNTHESIZE(vector<ST_PC_VERTEX>, m_vecParticleVertex, VerParticleVertex);
	int m_nMapSizeX, m_nMapSizeY, m_nMapSizeZ;

	float m_fMove, m_fSpeed;
	int m_nCount;

public:
	cWeather();
	~cWeather();

	void Setup(int posX, int posY, int posZ, int count);
	void Update(float fMove, float fSpeed);
	void Render(char* sFullPath);

	void AddParticle(int count);
	void DeleteParticle(int count);
};