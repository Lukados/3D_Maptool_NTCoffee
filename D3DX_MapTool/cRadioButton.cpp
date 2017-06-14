#include "stdafx.h"
#include "cRadioButton.h"
#include "cObjectDB.h"


cRadioButton::cRadioButton()
{
}

cRadioButton::~cRadioButton()
{
}

void cRadioButton::Setup_RadioButton()
{
	// 맵 변수의 상태값에 따라 텍스쳐값 가지고 오기
	m_mapTexture[E_UISTATE_IDLE] = TEXTURE->GetTexture("image/rect/white.png");
	m_mapTexture[E_UISTATE_CLICKED] = TEXTURE->GetTexture("image/rect/yellow.png");
}

void cRadioButton::Update(float deltaTime)
{
	m_isClicked = false;
	// 버튼이 눌려졌을 때 상태값 변경
	for (int i = 0; i < m_vecSIndex.size(); i++)
	{
		if (INPUT->IsMouseDown(MOUSE_LEFT) &&
			INPUT->IsCollided(D3DXVECTOR2(m_vecSIndex[i]->vPosition.x, m_vecSIndex[i]->vPosition.y),
				D3DXVECTOR2(m_vecSIndex[i]->vPosition.x + m_vecSIndex[i]->stSize.nWidth, m_vecSIndex[i]->vPosition.y + m_vecSIndex[i]->stSize.nWidth)))
		{
			if (m_vecSIndex[i]->eUIState == E_UISTATE_IDLE)
			{
				for (int k = 0; k < m_vecSIndex.size(); k++)
				{
					m_vecSIndex[k]->eUIState = E_UISTATE_IDLE;
				}
				m_vecSIndex[i]->eUIState = E_UISTATE_CLICKED;
				m_isClicked = true;
				break;
			}

			else if (m_vecSIndex[i]->eUIState == E_UISTATE_CLICKED)
			{
				m_vecSIndex[i]->eUIState = E_UISTATE_IDLE;
				m_isClicked = false;
				break;
			}
		}
	}

	cUIObject::Update(deltaTime);
}

void cRadioButton::Render(LPD3DXSPRITE pSprite)
{
	Render_ButtonUI(pSprite);
	Render_Sprite(pSprite);

	cUIObject::Render(pSprite);
}

void cRadioButton::Destroy()
{
	// texture가 있을 때 삭제
	for each(auto p in m_mapTexture)
	{
		SAFE_RELEASE(p.second);
	}
	for each(auto p in m_vecSIndex)
	{
		SAFE_DELETE(p);
	}

	delete this;
}

void cRadioButton::SetTexture(int index, LPDIRECT3DTEXTURE9 texture)
{
	// vector의 해당 인덱스 값에 있는 텍스쳐 변경
	m_vecSIndex[index]->SetTexture(texture);
}

void cRadioButton::SetSID(int index, int eSID)
{
	m_vecSIndex[index]->SetSID(eSID);
}

void cRadioButton::Add_RadioButton(D3DXVECTOR3 position, ST_SIZE size, E_S_OBJECTID sID, eUISTATE state, LPDIRECT3DTEXTURE9 t)
{
	// RadioButton의 구조체 값 삽입
	ST_SINDEX* st = new ST_SINDEX(position, size, sID, state);
	st->SetTexture(t);
	st->vPosition = position + m_vPosition;

	m_vecSIndex.push_back(st);
}

int cRadioButton::GetSID()
{
	for (int i = 0; i < m_vecSIndex.size(); i++)
	{
		if (m_vecSIndex[i]->eUIState == E_UISTATE_CLICKED)
		{
			return m_vecSIndex[i]->eCurrentSID;
		}
	}
	return -1;
}

void cRadioButton::Render_ButtonUI(LPD3DXSPRITE pSprite)
{
	RECT rc;

	pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

	for (int i = 0; i < m_vecSIndex.size(); i++)
	{
		D3DXMATRIXA16 matS, matT, matWorld;
		D3DXMatrixIdentity(&matS);
		D3DXMatrixIdentity(&matT);
		D3DXMatrixIdentity(&matWorld);

		D3DXMatrixScaling(&matS, 0.9f, 0.9f, 0.9f);
		matT._41 += m_vecSIndex[i]->vPosition.x;
		matT._42 += m_vecSIndex[i]->vPosition.y;
		matT._43 += m_vecSIndex[i]->vPosition.z;

		matWorld = matS * matT;
		pSprite->SetTransform(&matWorld);

		SetRect(&rc, 0, 0, m_vecSIndex[i]->stSize.nWidth, m_vecSIndex[i]->stSize.nHeight);
		pSprite->Draw(m_mapTexture[m_vecSIndex[i]->eUIState], &rc, &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 0, 0), D3DCOLOR_RGBA(255, 255, 255, 255));
	}

	pSprite->End();
}

void cRadioButton::Render_Sprite(LPD3DXSPRITE pSprite)
{
	RECT rc;

	pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

	for (int i = 0; i < m_vecSIndex.size(); i++)
	{
		D3DXMATRIXA16 matS, matT, matWorld;
		D3DXMatrixIdentity(&matS);
		D3DXMatrixIdentity(&matT);
		D3DXMatrixIdentity(&matWorld);

		D3DXMatrixScaling(&matS, 0.9f, 0.9f, 0.9f);
		matT._41 += m_vecSIndex[i]->vPosition.x;
		matT._42 += m_vecSIndex[i]->vPosition.y;
		matT._43 += m_vecSIndex[i]->vPosition.z;

		matWorld = matS * matT;
		pSprite->SetTransform(&matWorld);

		int u = OBJECTDB->GetMapObject(m_vecSIndex[i]->eCurrentSID)->fU * 8;
		int v = OBJECTDB->GetMapObject(m_vecSIndex[i]->eCurrentSID)->fV * 8;
		SetRect(&rc, 128 * u, 128 * v, 128 * (u + 1), 128 * (v + 1));
		pSprite->Draw(m_vecSIndex[i]->texture, &rc, &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 0, 0), D3DCOLOR_RGBA(255, 255, 255, 255));
	}

	pSprite->End();
}
