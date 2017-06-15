#pragma once

class cObject;
class cMtlTex;
class cObjLoader;

class cConstruct : public cObject
{
private:
	D3DXMATRIXA16		m_matWorld;

	LPD3DXMESH			m_pObjMesh;
	vector<cMtlTex*>	m_vecObjMtlTex;

	SYNTHESIZE(int, m_nSObjID, SObjID);
	SYNTHESIZE(D3DXVECTOR3, m_vScale, Scale);
	SYNTHESIZE(float, m_fRotX, RotationX);
	SYNTHESIZE(float, m_fRotY, RotationY);
	SYNTHESIZE(float, m_fRotZ, RotationZ);
	SYNTHESIZE(D3DXVECTOR3, m_vPosition, Position);


public:
	cConstruct();
	~cConstruct();

	void Setup(char* szFolder, char* szFile, bool isChecked);
	void Update();
	void Render();
	void Render_Shadow();

	void Create(int sIndex);
	void Destroy();

	LPD3DXMESH GetMesh() { return m_pObjMesh; }

	D3DMATERIAL9 InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p);
};