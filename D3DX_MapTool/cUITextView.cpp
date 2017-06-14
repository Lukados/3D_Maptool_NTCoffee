#include "stdafx.h"
#include "cUITextView.h"


cUITextView::cUITextView() : m_eFontType(E_FONT_DEFAULT)
{
}


cUITextView::~cUITextView()
{
}

void cUITextView::Setup_Text(ST_SIZE stSize, string text, eFONTTYPE fontType)
{
	m_sText = text;
	m_stSize = stSize;
	m_eFontType = fontType;
}

void cUITextView::Render(LPD3DXSPRITE pSprite)
{
	RECT rc;

	LPD3DXFONT pFont = FONT->GetFont(m_eFontType);

	SetRect(&rc, LeftTop().x, LeftTop().y, RightBottom().x, RightBottom().y);
	pFont->DrawText(NULL, m_sText.c_str(), m_sText.length(), &rc, DT_CENTER | DT_VCENTER, D3DCOLOR_XRGB(0, 0, 0));

	cUIObject::Render(pSprite);
}