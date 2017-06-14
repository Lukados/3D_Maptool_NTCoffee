#include "stdafx.h"
#include "cUIInputField.h"


cUIInputField::cUIInputField() : m_isOn(false), m_eCurrentState(E_UISTATE_IDLE)
{
}


cUIInputField::~cUIInputField()
{
}

void cUIInputField::Setup_field(int width, int height, string sPath_idle, string sPath_selected)
{
	m_stSize.nWidth = width;
	m_stSize.nHeight = height;
	m_mapTexture[E_UISTATE_IDLE] = TEXTURE->GetTexture(sPath_idle);
	m_mapTexture[E_UISTATE_SELECTED] = TEXTURE->GetTexture(sPath_selected);
}

void cUIInputField::Update(float deltaTime)
{
	switch (m_eCurrentState)
	{
	case E_UISTATE_IDLE:
		if (INPUT->IsMouseDown(MOUSE_LEFT)&&
			INPUT->IsCollided(D3DXVECTOR2(m_vPosition.x, m_vPosition.y), D3DXVECTOR2(m_vPosition.x + m_stSize.nWidth, m_vPosition.y + m_stSize.nHeight)))
		{
			m_eCurrentState = E_UISTATE_SELECTED;
			m_isOn = true;
		}
		break;
	case E_UISTATE_SELECTED:
		if ((INPUT->IsMouseDown(MOUSE_LEFT) &&
			!INPUT->IsCollided(D3DXVECTOR2(m_vPosition.x, m_vPosition.y), D3DXVECTOR2(m_vPosition.x + m_stSize.nWidth, m_vPosition.y + m_stSize.nHeight))) ||
			INPUT->IsMouseDown(MOUSE_RIGHT))
		{
			m_eCurrentState = E_UISTATE_IDLE;
			m_isOn = false;
		}
		if (INPUT->IsKeyDown(VK_RETURN))
		{
			m_eCurrentState = E_UISTATE_IDLE;
			m_isOn = false;
		}
		else if (INPUT->IsKeyDown(VK_BACK))
		{
			if(m_sText.size() > 0) m_sText.pop_back();
		}
		else
		{
			char c = INPUT->GetKeyDown();
			if(c != NULL) m_sText.push_back(c);
		}
		break;
	}

}

void cUIInputField::Render(LPD3DXSPRITE pSprite)
{
	if (m_isHidden) return;

	RECT rc;

	// >> Title ·»´õ ºÎºÐ
	pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
	pSprite->SetTransform(&m_matWorld);

	SetRect(&rc, 0, 0, m_stSize.nWidth, m_stSize.nHeight);
	pSprite->Draw(m_mapTexture[m_eCurrentState], &rc, &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 0, 0), D3DCOLOR_ARGB(255, 255, 255, 255));

	pSprite->End();
	// << 

	// >> Name String ·»´õ ºÎºÐ
	LPD3DXFONT pFont = FONT->GetFont(E_FONT_DEFAULT);

	SetRect(&rc, LeftTop().x, LeftTop().y, RightBottom().x, RightBottom().y);

	pFont->DrawText(NULL, m_sText.c_str(), m_sText.length(), &rc, DT_CENTER | DT_VCENTER, D3DCOLOR_XRGB(0, 0, 0));
	//<<

	cUIObject::Render(pSprite);
}

void cUIInputField::Destroy()
{
	for each(auto p in m_mapTexture)
	{
		SAFE_RELEASE(p.second);
	}

	delete this;
}
