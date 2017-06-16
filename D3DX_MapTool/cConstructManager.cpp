#include "stdafx.h"
#include "cConstructManager.h"


cConstructManager::cConstructManager()
{
}


cConstructManager::~cConstructManager()
{
}

void cConstructManager::AddConstruct(int nSID, LPD3DXMESH pMesh)
{
	if (m_mapMesh.find(nSID) != m_mapMesh.end()) return;
	m_mapMesh[nSID] = pMesh;
}

void cConstructManager::Destroy()
{
	for each(auto p in m_mapMesh)
	{
		SAFE_RELEASE(p.second);
	}
}

LPD3DXMESH cConstructManager::FindConstruct(int nSID)
{
	if (m_mapMesh.find(nSID) != m_mapMesh.end()) return m_mapMesh[nSID];
	return NULL;
}