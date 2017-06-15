#pragma once

#define CONSTRUCT	cConstructManager::GetInstance()

class cConstructManager
{
	SINGLETON(cConstructManager);

private:
	map<int, LPD3DXMESH> m_mapMesh;

public:
	void AddConstruct(int nSID, LPD3DXMESH pMesh);
	LPD3DXMESH FindConstruct(int nSID);
	void Destroy();
};

