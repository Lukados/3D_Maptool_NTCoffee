#include "stdafx.h"
#include "cShadowManager.h"
#include "cConstruct.h"

cShadowManager::cShadowManager()
{
	
}


cShadowManager::~cShadowManager()
{
}

void cShadowManager::Setup(char * szFolder, char * szFile)
{
}

void cShadowManager::Setup(std::vector<cConstruct*> vList)
{
	
}

void cShadowManager::Update(std::vector<cConstruct*> vList)
{
	D3DXMATRIXA16	matWorld, matS, matRX, matRY, matRZ, matT;

	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixScaling(&matS, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixRotationX(&matRX, m_fRotX);
	D3DXMatrixRotationY(&matRY, m_fRotY);
	D3DXMatrixRotationZ(&matRZ, m_fRotZ);
	D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	matWorld = matS * matRX * matRY * matRZ* matT;

	DEVICE->SetTransform(D3DTS_WORLD, &matWorld);

	m_vecConstruct = vList;
}


void cShadowManager::Render()
{
}

void cShadowManager::ShadowRender()
{
}

void cShadowManager::Destroy()
{
}

D3DMATERIAL9 cShadowManager::InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p)
{
	return D3DMATERIAL9();
}
