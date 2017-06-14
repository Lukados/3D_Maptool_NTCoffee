#include "stdafx.h"
#include "cUIScrollbar.h"


cUIScrollbar::cUIScrollbar() : m_pBarTexture(NULL), m_pLineTexture(NULL), m_vBarPos(0,0), m_eCurrentState(E_UISTATE_IDLE), m_nValue(0)
{
}


cUIScrollbar::~cUIScrollbar()
{
}

void cUIScrollbar::Setup_scroll(int width, int height, string name, string path_bar, int minValue, int maxValue)
{
	m_stSize.nWidth = width;
	m_stSize.nHeight = height;
	m_sName = name;
	m_pBarTexture = TEXTURE->GetTexture(path_bar);
	m_pLineTexture = TEXTURE->GetTexture("image/rect/darkgray.png");
	m_nMinValue = minValue;
	m_nMaxValue = maxValue;
}

void cUIScrollbar::Setup_bar(int barW, int barH)
{
	m_stBarSize.nWidth = barW;
	m_stBarSize.nHeight = barH;
	m_vBarPos = D3DXVECTOR2(m_vPosition.x + m_stSize.nWidth * 0.5f, m_vPosition.y + m_stSize.nHeight * 0.5f);
}

void cUIScrollbar::Update(float deltaTime)
{
	// >> 버튼 눌릴 시, 눌린 상태로 전환
	D3DXVECTOR2 lt_bar = D3DXVECTOR2(m_vBarPos.x - m_stBarSize.nWidth * 0.5f, m_vBarPos.y - m_stBarSize.nHeight * 0.5f);
	D3DXVECTOR2 rb_bar = D3DXVECTOR2(m_vBarPos.x + m_stBarSize.nWidth * 0.5f, m_vBarPos.y + m_stBarSize.nHeight * 0.5f);
	if (INPUT->IsCollided(lt_bar, rb_bar) && 
		INPUT->IsMouseDown(MOUSE_LEFT))
	{
		m_eCurrentState = E_UISTATE_PRESSED;
	}
	// << 

	// >> 눌린 상태에서 드래그 시, 막대 이동
	if (INPUT->IsMouseDrag(MOUSE_LEFT)
		&& m_eCurrentState == E_UISTATE_PRESSED)
	{
		m_vBarPos.x += INPUT->GetMouseDelta().x;
		m_vBarPos.x = MATH->Clamp(LeftVCenter().x, RightVCenter().x, m_vBarPos.x);
	}
	// <<

	// >> 마우스 업에서 IdleState로
	if (INPUT->IsMouseUp(MOUSE_LEFT))
	{
		m_eCurrentState = E_UISTATE_IDLE;
	}
	// << 
	
	// >> 현재 값 저장
	m_nValue = m_nMinValue + (m_nMaxValue - m_nMinValue) * (m_vBarPos.x - m_vPosition.x) / m_stSize.nWidth;
	m_nValue = MATH->Clamp(m_nMinValue, m_nMaxValue, m_nValue);
	// <<

	cUIObject::Update(deltaTime);
}

void cUIScrollbar::Render(LPD3DXSPRITE pSprite)
{
	if (m_isHidden) return;

	RECT rc;

	// >> Line 렌더 부분
	pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

	D3DXMATRIXA16 matLine;
	D3DXMatrixTranslation(&matLine, LeftVCenter().x, LeftVCenter().y, 0);
	pSprite->SetTransform(&matLine);
	SetRect(&rc, 0, 0, m_stSize.nWidth, 2);
	pSprite->Draw(m_pLineTexture, &rc, &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 0, 0), D3DCOLOR_ARGB(255, 255, 255, 255));

	D3DXMATRIXA16 matBar;
	D3DXMatrixTranslation(&matBar, m_vBarPos.x - m_stBarSize.nWidth * 0.5f, m_vBarPos.y - m_stBarSize.nHeight * 0.5f, 0);
	pSprite->SetTransform(&matBar);
	SetRect(&rc, 0, 0, m_stBarSize.nWidth, m_stBarSize.nHeight);
	pSprite->Draw(m_pBarTexture, &rc, &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 0, 0), D3DCOLOR_ARGB(255, 255, 255, 255));
	
	pSprite->End();
	// << 

	// >> Name String 렌더 부분
	LPD3DXFONT pFont = FONT->GetFont(E_FONT_DEFAULT);
	
	SetRect(&rc, LeftTop().x - 100, LeftTop().y, LeftTop().x - 20, RightBottom().y);	
	pFont->DrawText(NULL, m_sName.c_str(), m_sName.length(), &rc, DT_RIGHT | DT_VCENTER, D3DCOLOR_XRGB(0, 0, 0));

	SetRect(&rc, RightBottom().x + 20, LeftTop().y, RightBottom().x + 50, RightBottom().y);
	pFont->DrawText(NULL, to_string(m_nValue).c_str(), to_string(m_nValue).length(), &rc, DT_LEFT | DT_VCENTER, D3DCOLOR_XRGB(0, 0, 0));
	// <<

	cUIObject::Render(pSprite);
}

void cUIScrollbar::Destroy()
{
	SAFE_RELEASE(m_pBarTexture);
	SAFE_RELEASE(m_pLineTexture);
}
