#include "stdafx.h"
#include "cConstruct.h"
#include "cObject.h"
#include "cMtlTex.h"
#include "cObjLoader.h"

cConstruct::cConstruct()
	: m_pObjMesh(NULL)
	, m_vScale(0.5f, 0.5f, 0.5f)
	, m_fRotX(0.0f), m_fRotY(0.0f), m_fRotZ(0.0f)
	, m_vPosition(0.0f, 0.0f, 0.0f)
{
}

cConstruct::cConstruct(const cConstruct& pConstruct)
{

}

cConstruct::~cConstruct()
{
	SAFE_RELEASE(m_pObjMesh);
}

void cConstruct::Setup(char* szFolder, char* szFile, bool isChecked)
{
	cObjLoader	l;

	if(isChecked == true)			m_pObjMesh = l.LoadMesh(m_vecObjMtlTex, szFolder, szFile, true);
	else if (isChecked == false)	m_pObjMesh = l.LoadMesh(m_vecObjMtlTex, szFolder, szFile);
}

void cConstruct::Update()
{
	D3DXMATRIXA16	matS, matRX, matRY, matRZ, matT;

	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&matS);
	D3DXMatrixIdentity(&matRX);
	D3DXMatrixIdentity(&matRY);
	D3DXMatrixIdentity(&matRZ);
	D3DXMatrixIdentity(&matT);

	D3DXMatrixScaling(&matS, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixRotationX(&matRX,  m_fRotX);
	D3DXMatrixRotationY(&matRY,  m_fRotY);
	D3DXMatrixRotationZ(&matRZ, m_fRotZ);
	D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	m_matWorld = matS * matRX * matRY * matRZ* matT;

	DEVICE->SetTransform(D3DTS_WORLD, &m_matWorld);
}

void cConstruct::Render()
{
	DEVICE->SetRenderState(D3DRS_LIGHTING, true);
	DEVICE->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	if (m_nSObjID >= E_S_OBJECTID_H_DW_START && m_nSObjID <= E_S_OBJECTID_H_DW_END ||
		m_nSObjID >= E_S_OBJECTID_P_DW_START && m_nSObjID <= E_S_OBJECTID_P_ETC_END)
	{
		DEVICE->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		DEVICE->SetRenderState(D3DRS_ALPHAREF, 0x00000088);
		DEVICE->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		DEVICE->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	}

	for (size_t i = 0; i < m_vecObjMtlTex.size(); i++)
	{
		DEVICE->SetTransform(D3DTS_WORLD, &m_matWorld);
		DEVICE->SetMaterial(&m_vecObjMtlTex[i]->GetMaterial());
		DEVICE->SetTexture(0, m_vecObjMtlTex[i]->GetTexture());

		m_pObjMesh->DrawSubset(i);
	}	

	DEVICE->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	DEVICE->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	DEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	DEVICE->SetRenderState(D3DRS_LIGHTING, false);
}

void cConstruct::Create(int sIndex)
{
	char* folder = OBJECTDB->GetMapObject(sIndex)->szFolder;
	char* file = OBJECTDB->GetMapObject(sIndex)->szFile;

	m_nSObjID = sIndex;

	if (sIndex >= E_S_OBJECTID_P_DW_START && sIndex <= E_S_OBJECTID_P_ETC_END || 
		sIndex >= E_S_OBJECTID_N_H_START && sIndex <= E_S_OBJECTID_N_O_END)
		Setup(folder, file, false);
	else Setup(folder, file, true);

	Update();
}

void cConstruct::Destroy()
{
	SAFE_RELEASE(m_pObjMesh);
}

D3DMATERIAL9 cConstruct::InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p)
{
	D3DMATERIAL9 mtrl;
	mtrl.Ambient = a;
	mtrl.Diffuse = d;
	mtrl.Specular = s;
	mtrl.Emissive = e;
	mtrl.Power = p;
	return mtrl;
}