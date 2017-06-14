#pragma once
#define SHADOW cShadowManager::GetInstance()

class cObject;
class cMtlTex;
class cObjLoader;
class cConstruct;

class cShadowManager : public cObject
{
protected:
	D3DXVECTOR4         light;
	D3DXMATRIXA16       matWorld;
	LPD3DXMESH			m_pObjMesh;
	vector<cMtlTex*>	m_vecObjMtlTex;

	std::vector<cConstruct*> m_vecConstruct;

	SYNTHESIZE(D3DXVECTOR3, m_vScale, Scale);
	SYNTHESIZE(float, m_fRotX, RotationX);
	SYNTHESIZE(float, m_fRotY, RotationY);
	SYNTHESIZE(float, m_fRotZ, RotationZ);
	SYNTHESIZE(D3DXVECTOR3, m_vPosition, Position);

public:
	SINGLETON(cShadowManager);

	void Setup(char* szFolder, char* szFile);
	void Setup(std::vector<cConstruct*> vList);
	void Update(std::vector<cConstruct*> vList);
	void Render();
	void ShadowRender();
	void Destroy();

	D3DMATERIAL9 InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p);
};

