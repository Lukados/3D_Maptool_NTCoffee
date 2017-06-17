#include "stdafx.h"
#include "cWeather.h"


cWeather::cWeather()
	: m_nMapSizeX(0), m_nMapSizeY(0), m_nMapSizeZ(0)
{
}


cWeather::~cWeather()
{
}


DWORD FtoDw(float f)
{
	return *((DWORD*)&f);
}


void cWeather::Setup(int mapSizeX, int mapSizeY, int mapSizeZ, int count)
{
	srand((unsigned)time(NULL));
	m_vecParticleVertex.clear();
	m_vecParticleVertex.resize(count);

	m_nMapSizeX = mapSizeX;
	m_nMapSizeY = mapSizeY;
	m_nMapSizeZ = mapSizeZ;

	for (int i = 0; i < m_vecParticleVertex.size(); ++i)
	{
		float randX = rand() % m_nMapSizeX;
		randX -= m_nMapSizeX / 2;

		float randY = rand() % m_nMapSizeY;
		randY -= m_nMapSizeY / 2;

		float randZ = rand() % m_nMapSizeZ;
		randZ -= m_nMapSizeZ / 2;

		m_vecParticleVertex[i].p = D3DXVECTOR3(randX, randY, randZ);

		D3DXVECTOR3	vAngle = D3DXVECTOR3(
			D3DXToRadian(rand() % 3600 / 10.0f),
			D3DXToRadian(rand() % 3600 / 10.0f),
			D3DXToRadian(rand() % 3600 / 10.0f));

		D3DXMATRIX	matRX, matRY, matRZ, matWorld;
		D3DXMatrixIsIdentity(&matWorld);
		D3DXMatrixRotationX(&matRX, vAngle.x);
		D3DXMatrixRotationY(&matRY, vAngle.y);
		D3DXMatrixRotationZ(&matRZ, vAngle.z);
		matWorld = matRX * matRY * matRZ;

		D3DXVec3TransformCoord(&m_vecParticleVertex[i].p, &m_vecParticleVertex[i].p, &matWorld);

		m_vecParticleVertex[i].c = D3DCOLOR_ARGB(255, 255, 255, 255);
	}
}

void cWeather::Update(float fMove, float fSpeed)
{
	int nAlpha = 0;
	int nDelta = 50;

	for (int i = 0; i < m_vecParticleVertex.size(); ++i)
	{
		m_vecParticleVertex[i].c = D3DCOLOR_ARGB(150, 255, 255, 255);	// alpha 값만 변경

		if (m_vecParticleVertex[i].p.y < -5) m_vecParticleVertex[i].p.y = 50 + rand() % 50;
		else m_vecParticleVertex[i].p.y -= fSpeed;

		// if (m_vecParticleVertex[i].p.y < 5)
		// {
		// 	nAlpha += nDelta;
		// 	m_vecParticleVertex[i].c = D3DCOLOR_ARGB(nAlpha, 255, 255, 255);
		// }

		if (m_vecParticleVertex[i].p.x < -m_nMapSizeX / 2) m_vecParticleVertex[i].p.x = m_nMapSizeX / 2;
		else m_vecParticleVertex[i].p.x -= fMove;
	}
}

void cWeather::Render(char* sFullPath)
{
	D3DXMATRIXA16	matWorld;
	D3DXMatrixIdentity(&matWorld);
	DEVICE->SetTransform(D3DTS_WORLD, &matWorld);

	DEVICE->SetRenderState(D3DRS_POINTSCALEENABLE, true);			// 점을 확대할지 말지 정하는것
	DEVICE->SetRenderState(D3DRS_POINTSIZE, FtoDw(5.0f));			// 포인트 사이즈
	DEVICE->SetRenderState(D3DRS_POINTSCALE_A, FtoDw(0.0f));
	DEVICE->SetRenderState(D3DRS_POINTSCALE_B, FtoDw(0.0f));
	DEVICE->SetRenderState(D3DRS_POINTSCALE_C, FtoDw(10.0f));		// 10.0f로 바꿔서 실험해보기

	DEVICE->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
	DEVICE->SetRenderState(D3DRS_POINTSIZE_MIN, FtoDw(0.0f));
	DEVICE->SetRenderState(D3DRS_POINTSIZE_MAX, FtoDw(100.0f));

	DEVICE->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	DEVICE->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	DEVICE->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	// 텍스쳐의 알파블렌딩 방식 결정
	DEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	DEVICE->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	DEVICE->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	DEVICE->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	DEVICE->SetRenderState(D3DRS_LIGHTING, false);
	DEVICE->SetRenderState(D3DRS_ZWRITEENABLE, false);

	DEVICE->SetFVF(ST_PC_VERTEX::FVF);
	DEVICE->SetTexture(0, TEXTURE->GetTexture(sFullPath));
	DEVICE->DrawPrimitiveUP(D3DPT_POINTLIST, m_vecParticleVertex.size(), &m_vecParticleVertex[0], sizeof(ST_PC_VERTEX));

	DEVICE->SetRenderState(D3DRS_ZWRITEENABLE, true);
	DEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	DEVICE->SetRenderState(D3DRS_LIGHTING, true);

	DEVICE->SetRenderState(D3DRS_POINTSPRITEENABLE, false);
	DEVICE->SetRenderState(D3DRS_POINTSCALEENABLE, false);
}

void cWeather::AddParticle(int count)
{
	for (int i = 0; i < count; i++)
	{
		ST_PC_VERTEX vertex;

		float randX = rand() % m_nMapSizeX;
		randX -= m_nMapSizeX / 2;

		float randY = rand() % m_nMapSizeY;
		randY -= m_nMapSizeY / 2;

		float randZ = rand() % m_nMapSizeZ;
		randZ -= m_nMapSizeZ / 2;

		vertex.p = D3DXVECTOR3(randX, randY, randZ);

		D3DXVECTOR3	vAngle = D3DXVECTOR3(
			D3DXToRadian(rand() % 3600 / 10.0f),
			D3DXToRadian(rand() % 3600 / 10.0f),
			D3DXToRadian(rand() % 3600 / 10.0f));

		D3DXMATRIX	matRX, matRY, matRZ, matWorld;
		D3DXMatrixIsIdentity(&matWorld);
		D3DXMatrixRotationX(&matRX, vAngle.x);
		D3DXMatrixRotationY(&matRY, vAngle.y);
		D3DXMatrixRotationZ(&matRZ, vAngle.z);
		matWorld = matRX * matRY * matRZ;

		D3DXVec3TransformCoord(&vertex.p, &vertex.p, &matWorld);

		vertex.c = D3DCOLOR_ARGB(150, 255, 255, 255);
		m_vecParticleVertex.push_back(vertex);
	}
}

void cWeather::DeleteParticle(int count)
{
	if (m_vecParticleVertex.size() < count + 1) return;

	for (int i = 0; i < count; i++)
	{
		m_vecParticleVertex.pop_back();
	}
}