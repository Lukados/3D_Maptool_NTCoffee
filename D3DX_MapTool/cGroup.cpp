#include "stdafx.h"
#include "cGroup.h"


cGroup::cGroup()
	: m_pMtlTex(NULL)
{
}

cGroup::~cGroup()
{
	SAFE_RELEASE(m_pMtlTex);
}

void cGroup::Render()
{
	if (m_pMtlTex)
	{
		DEVICE->SetTexture(0, m_pMtlTex->GetTexture());
		DEVICE->SetMaterial(&m_pMtlTex->GetMaterial());
	}

	DEVICE->SetFVF(ST_PNT_VERTEX::FVF);
	DEVICE->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
		m_vecVertex.size() / 3,
		&m_vecVertex[0],
		sizeof(ST_PNT_VERTEX));
}