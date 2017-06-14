#include "stdafx.h"
#include "cPyramid.h"


cPyramid::cPyramid() : m_vPosition(0.0f, 0.0f, 0.0f)
{
}


cPyramid::~cPyramid()
{
}

void cPyramid::Setup(D3DXCOLOR c, D3DXMATRIXA16& mat)
{
	m_matR = mat;

	ST_PC_VERTEX v;

	v.c = c;


	v.p = D3DXVECTOR3(0, 0, 0); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(1, -1, -1); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(-1, -1, -1); m_vecVertex.push_back(v);

	v.p = D3DXVECTOR3(0, 0, 0); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(1, -1, 1); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(1, -1, -1); m_vecVertex.push_back(v);

	v.p = D3DXVECTOR3(0, 0, 0); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(-1, -1, 1); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(1, -1, 1); m_vecVertex.push_back(v);

	v.p = D3DXVECTOR3(0, 0, 0); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(-1, -1, -1); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(-1, -1, 1); m_vecVertex.push_back(v);

	v.p = D3DXVECTOR3(-1, -1, -1); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(1, -1, -1); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(-1, -1, 1); m_vecVertex.push_back(v);

	v.p = D3DXVECTOR3(1, -1, -1); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(1, -1, 1); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(-1, -1, 1); m_vecVertex.push_back(v);
}

void cPyramid::Render()
{
	D3DXMATRIXA16 matWorld, matS, matT;
	D3DXMatrixScaling(&matS, 0.1f, 2.0f, 0.1f);
	D3DXMatrixIdentity(&matT);
	D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);

	matWorld = matS * m_matR * matT;

	DEVICE->SetRenderState(D3DRS_LIGHTING, false);
	DEVICE->SetTexture(0, NULL);
	DEVICE->SetTransform(D3DTS_WORLD, &matWorld);
	DEVICE->SetFVF(ST_PC_VERTEX::FVF);	
	DEVICE->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
		m_vecVertex.size() / 3,
		&m_vecVertex[0],
		sizeof(ST_PC_VERTEX));
}

void cPyramid::SetPosition(D3DXVECTOR3 pos)
{
	m_vPosition = pos;
}