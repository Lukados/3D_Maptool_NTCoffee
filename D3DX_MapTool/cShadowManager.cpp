#include "stdafx.h"
#include "cShadowManager.h"
#include "cConstruct.h"

cShadowManager::cShadowManager() :
	diffuseAlpha(0.5),
	isView(false),
	m_pMesh(NULL)
{
	
}


cShadowManager::~cShadowManager()
{
}

D3DMATERIAL9 cShadowManager::InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p)
{
	D3DMATERIAL9 mtrl;
	mtrl.Ambient = a;
	mtrl.Diffuse = d;
	mtrl.Specular = s;
	mtrl.Emissive = e;
	mtrl.Power = p;
	return mtrl;
}

void cShadowManager::Setup(std::vector<cConstruct*> vecList)
{
	SetLight();
	m_vecConstruct = vecList;
	m_mtrl = InitMtrl(D3DCOLOR_XRGB(0, 0, 0), D3DCOLOR_XRGB(0, 0, 0), D3DCOLOR_XRGB(0, 0, 0), D3DCOLOR_XRGB(0, 0, 0), 0.0f);
	m_mtrl.Diffuse.a = diffuseAlpha;
}

void cShadowManager::SetupMap(std::vector<cConstruct*> vecList, LPD3DXMESH pMesh)
{
	SetLight();
	m_vecConstruct = vecList;
	m_mtrl = InitMtrl(D3DCOLOR_XRGB(0, 0, 0), D3DCOLOR_XRGB(0, 0, 0), D3DCOLOR_XRGB(0, 0, 0), D3DCOLOR_XRGB(0, 0, 0), 0.0f);
	m_mtrl.Diffuse.a = diffuseAlpha;
	m_pMesh = &pMesh;
}

void cShadowManager::Update(std::vector<cConstruct*> vecList)
{
	m_vecConstruct = vecList;
}

void cShadowManager::UpdateAlpha(float alpha)
{
	m_mtrl.Diffuse.a = alpha;
}

void cShadowManager::Render()
{
	if (isView)
	{
		DEVICE->Clear(NULL, NULL, D3DCLEAR_STENCIL, D3DCOLOR_XRGB(47, 121, 112), 1.0f, 0);
		DEVICE->SetRenderState(D3DRS_LIGHTING, true);
		for (int i = 0; i < m_vecConstruct.size(); i++)
		{
			DEVICE->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
			DEVICE->SetRenderState(D3DRS_STENCILENABLE, TRUE);

			DEVICE->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);

			DEVICE->SetRenderState(D3DRS_STENCILREF, 0x0);
			DEVICE->SetRenderState(D3DRS_STENCILMASK, 0xffffffff);
			DEVICE->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);

			DEVICE->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR);
			DEVICE->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
			DEVICE->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);

			D3DXPLANE groundPlane(0.0f, m_vecConstruct[i]->GetPosition().y -1.0f , 0.0f, 0.0f);

			D3DXVECTOR4 lightTest(-m_light.Direction.x, -m_light.Direction.y, -m_light.Direction.z, 0.0f);

			D3DXMATRIX S;
			D3DXMatrixShadow(
				&S,
				&lightTest,
				&groundPlane);

			D3DXMATRIX T;
			D3DXMatrixTranslation(
				&T,
				m_vecConstruct[i]->GetPosition().x, m_vecConstruct[i]->GetPosition().y, m_vecConstruct[i]->GetPosition().z);
	

			D3DXMATRIXA16 matS;
			D3DXMatrixScaling(&matS, m_vecConstruct[i]->GetScale().x, m_vecConstruct[i]->GetScale().y, m_vecConstruct[i]->GetScale().z);

			D3DXMATRIX W = matS *T * S;

			DEVICE->SetTransform(D3DTS_WORLD, &W);

			DEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			DEVICE->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			DEVICE->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


			DEVICE->SetRenderState(D3DRS_ZENABLE, FALSE);

			for (int j = 0; j < m_vecConstruct[i]->GetMtl().size(); j++)
			{
				DEVICE->SetMaterial(&m_mtrl);
				DEVICE->SetTexture(0, 0);
				m_vecConstruct[i]->GetMesh()->DrawSubset(j);
			}

			DEVICE->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);

			DEVICE->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
			DEVICE->SetRenderState(D3DRS_ZENABLE, TRUE);
			DEVICE->SetRenderState(D3DRS_STENCILENABLE, FALSE);
			DEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
			DEVICE->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
			DEVICE->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
		}
		DEVICE->SetRenderState(D3DRS_LIGHTING, false);

	}
}

void cShadowManager::Render2()
{
	if (isView)
	{
		DEVICE->Clear(NULL, NULL, D3DCLEAR_STENCIL, D3DCOLOR_XRGB(47, 121, 112), 1.0f, 0);
		DEVICE->SetRenderState(D3DRS_LIGHTING, true);
		DEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	}
}

void cShadowManager::Destroy()
{
}

void cShadowManager::SetLight()
{
	ZeroMemory(&m_light, sizeof(D3DLIGHT9));

	m_light.Type = D3DLIGHT_DIRECTIONAL;
	m_light.Ambient = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);            // 주변에 영향을 받는 것들은 색을 띄게 만듬
	m_light.Diffuse = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	m_light.Specular = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);

	D3DXVECTOR3   vDir(1.0f, 1.0f, 1.0f);
	D3DXVec3Normalize(&vDir, &vDir);
	m_light.Direction = vDir;
	DEVICE->SetLight(0, &m_light);
	DEVICE->LightEnable(0, true);
}

void cShadowManager::SetViewOk(bool b)
{
	isView = b;
}
