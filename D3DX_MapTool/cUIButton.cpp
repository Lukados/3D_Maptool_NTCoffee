#include "stdafx.h"
#include "cUIButton.h"
#include "cInputManager.h"


cUIButton::cUIButton() : m_eCurrentState(E_UISTATE_IDLE)
{
}


cUIButton::~cUIButton()
{
}

void cUIButton::Setup_button(string sPath_idle, string sPath_mouseover, string sPath_clicked)
{
	D3DXIMAGE_INFO imageInfo;

	m_mapTexture[E_UISTATE_IDLE] = TEXTURE->GetTexture(sPath_idle, imageInfo);
	m_mapTexture[E_UISTATE_MOUSEOVER] = TEXTURE->GetTexture(sPath_mouseover, imageInfo);
	m_mapTexture[E_UISTATE_CLICKED] = TEXTURE->GetTexture(sPath_clicked, imageInfo);

	m_stSize.nWidth = imageInfo.Width;
	m_stSize.nHeight = imageInfo.Height;
}

void cUIButton::Update(float deltaTime)
{
	m_eCurrentState = E_UISTATE_IDLE;

	// >> Idle -> MouseOver
	if (INPUT->IsCollided(D3DXVECTOR2(m_vPosition.x, m_vPosition.y), D3DXVECTOR2(m_vPosition.x + m_stSize.nWidth, m_vPosition.y + m_stSize.nHeight)))
		m_eCurrentState = E_UISTATE_MOUSEOVER;
	// <<

	// >> MouseOver -> Clicked
	if (INPUT->IsMouseDown(MOUSE_LEFT) &&
		INPUT->IsCollided(D3DXVECTOR2(m_vPosition.x, m_vPosition.y), D3DXVECTOR2(m_vPosition.x + m_stSize.nWidth, m_vPosition.y + m_stSize.nHeight)))
		m_eCurrentState = E_UISTATE_CLICKED;
	// << 

	//if (INPUT->IsMouseUp(MOUSE_LEFT) &&
	//	m_eCurrentState == E_UISTATE_CLICKED)
	//{
	//
	//}

	cUIObject::Update(deltaTime);
}

void cUIButton::Render(LPD3DXSPRITE pSprite)
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

	pFont->DrawText(NULL, m_sName.c_str(), m_sName.length(), &rc, DT_CENTER | DT_VCENTER, D3DCOLOR_XRGB(0, 0, 0));
	//<<

	cUIObject::Render(pSprite);
}

void cUIButton::Destroy()
{
	for each(auto p in m_mapTexture)
	{
		SAFE_RELEASE(p.second);
	}

	delete this;
}
