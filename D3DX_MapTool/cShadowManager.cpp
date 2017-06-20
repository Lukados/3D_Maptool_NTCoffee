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
	m_vecConstruct = vecList;
	m_mtrl = InitMtrl(D3DCOLOR_XRGB(0, 0, 0), D3DCOLOR_XRGB(0, 0, 0), D3DCOLOR_XRGB(0, 0, 0), D3DCOLOR_XRGB(0, 0, 0), 0.0f);
	m_mtrl.Diffuse.a = diffuseAlpha;
}

void cShadowManager::SetupMap(std::vector<cConstruct*> vecList, LPD3DXMESH pMesh)
{
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
		DEVICE->SetRenderState(D3DRS_LIGHTING, true);
		//DEVICE->LightEnable(0, true);
		for (int i = 0; i < m_vecConstruct.size(); i++)
		{
			DEVICE->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
			DEVICE->SetRenderState(D3DRS_STENCILENABLE, TRUE);
			//false 는 이전 z버퍼에 저장된 값 무시하고 무조건 zbuffer 검사 통과해야됨
			DEVICE->SetRenderState(D3DRS_ZENABLE, TRUE);

			DEVICE->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);

			DEVICE->SetRenderState(D3DRS_STENCILREF, 0x0);
			DEVICE->SetRenderState(D3DRS_STENCILMASK, 0xffffffff);
			DEVICE->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);
			DEVICE->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR);
			DEVICE->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
			DEVICE->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);

			D3DXPLANE groundPlane(0.0f, -1.0f, 0.0f, m_vecConstruct[i]->GetPosition().y + 0.001f);

			D3DXVECTOR4 lightTest(-m_lightDir.x, -m_lightDir.y, -m_lightDir.z, 0.0f);

			D3DXMATRIX S;
			D3DXMatrixShadow(
				&S,
				&lightTest,
				&groundPlane);

			D3DXMATRIX T;
			D3DXMatrixTranslation(
				&T,
				m_vecConstruct[i]->GetPosition().x, m_vecConstruct[i]->GetPosition().y, m_vecConstruct[i]->GetPosition().z);

			D3DXMATRIXA16 matRY;
			D3DXMatrixRotationY(&matRY, m_vecConstruct[i]->GetRotationY());

			D3DXMATRIXA16 matS;
			D3DXMatrixScaling(&matS, m_vecConstruct[i]->GetScale().x, m_vecConstruct[i]->GetScale().y, m_vecConstruct[i]->GetScale().z);

			D3DXMATRIX W = matS * matRY * T * S;

			DEVICE->SetTransform(D3DTS_WORLD, &W);

			DEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			DEVICE->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			DEVICE->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

			DEVICE->SetRenderState(D3DRS_ZENABLE, TRUE);

			for (int j = 0; j < m_vecConstruct[i]->GetVecObjMtlTex().size(); j++)
			{
				DEVICE->SetMaterial(&m_mtrl);
				DEVICE->SetTexture(0, 0);
				m_vecConstruct[i]->GetObjMesh()->DrawSubset(j);
			}

			DEVICE->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
			DEVICE->SetRenderState(D3DRS_ZENABLE, TRUE);
			DEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
			DEVICE->SetRenderState(D3DRS_STENCILENABLE, FALSE);

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

void cShadowManager::SetLight(D3DLIGHT9 light, D3DXVECTOR3 dir)
{
	m_light = light;
	m_lightDir = dir;
}

void cShadowManager::SetViewOk(bool b)
{
	isView = b;
}

void cShadowManager::SetMatrix(D3DXMATRIXA16 * mat)
{
	m_cameraMatirx = *mat;
}
