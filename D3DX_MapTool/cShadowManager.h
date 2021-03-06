#pragma once

#define SHADOW cShadowManager::GetInstance()

class cMtlTex;
class cConstruct;

class cShadowManager 
{
protected:
	vector<cConstruct*>      m_vecConstruct;
	D3DLIGHT9				 m_light;
	D3DXMATRIXA16            m_matWorld;
	float                    diffuseAlpha;
	D3DMATERIAL9			 m_mtrl;
	bool                     isView;
	LPD3DXMESH*              m_pMesh;
	D3DXMATRIXA16            m_cameraMatirx;
	D3DXVECTOR3              m_lightDir;

	D3DMATERIAL9 InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p);
public:
	SINGLETON(cShadowManager);

	void Setup(std::vector<cConstruct*> vecList);
	void SetupMap(std::vector<cConstruct*> vecList, LPD3DXMESH pMesh);
	void Update(std::vector<cConstruct*> vecList);
	void UpdateAlpha(float alpha);
	void Render();
	void Render2();
	void Destroy();
	void SetLight(D3DLIGHT9 light, D3DXVECTOR3 dir);
	void SetViewOk(bool b);
	void SetMatrix(D3DXMATRIXA16* mat);
};

