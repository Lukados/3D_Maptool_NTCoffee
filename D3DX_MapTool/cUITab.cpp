#include "stdafx.h"
#include "cUITab.h"
#include "cInputManager.h"
#include "cTextureManager.h"


cUITab::cUITab()
{
}


cUITab::~cUITab()
{
}

void cUITab::Setup_tab(int titleWidth, int titleHeight, string title, string path_idleState, string path_selected)
{
	m_stSize.nWidth = titleWidth;
	m_stSize.nHeight = titleHeight;
	m_sTitle = title;
	m_currentState = E_UISTATE_IDLE;

	m_mapColor[E_UISTATE_IDLE] = TEXTURE->GetTexture(path_idleState);
	m_mapColor[E_UISTATE_SELECTED] = TEXTURE->GetTexture(path_selected);
}

void cUITab::Update(float deltaTime)
{

	if (m_isHidden)
	{
		m_currentState = E_UISTATE_IDLE;
		return;
	}
	else
	{
		m_currentState = E_UISTATE_SELECTED;
	}

	cUIObject::Update(deltaTime);
}

void cUITab::Render(LPD3DXSPRITE pSprite)
{

	RECT rc;

	// >> Title ·»´õ ºÎºÐ
	pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
	pSprite->SetTransform(&m_matWorld);

	SetRect(&rc, 0, 0, m_stSize.nWidth, m_stSize.nHeight);
	pSprite->Draw(m_mapColor[m_currentState], &rc, &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 0, 0), D3DCOLOR_ARGB(255, 255, 255, 255));

	pSprite->End();
	// << 


	// >> ÇØ´ç ÅÇ¿¡ ¼ÓÇÑ ºÎºÐ ·»´õ
	if (!m_isHidden)
	{
		pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
		pSprite->SetTransform(&m_matWorld);

		SetRect(&rc, m_vPosition.x, m_vPosition.y, m_vPosition.x + m_stSize.nWidth, m_vPosition.y + m_stSize.nHeight);
		pSprite->Draw(m_mapColor[m_currentState], &rc, &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 0, 0), D3DCOLOR_ARGB(255, 255, 255, 255));
		pSprite->End();

		cUIObject::Render(pSprite);
	}	
	// << 

	// >> Title String ·»´õ ºÎºÐ
	LPD3DXFONT pFont = FONT->GetFont(E_FONT_DEFAULT);

	SetRect(&rc, LeftTop().x, LeftTop().y, RightBottom().x, RightBottom().y);

	pFont->DrawText(NULL, m_sTitle.c_str(), m_sTitle.length(), &rc, DT_CENTER | DT_VCENTER, D3DCOLOR_XRGB(0, 0, 0));
	//<<
}

