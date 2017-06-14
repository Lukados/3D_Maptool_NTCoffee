#pragma once
#include "cUIObject.h"
class cUITextView :
	public cUIObject
{
	string m_sText;
	eFONTTYPE m_eFontType;

public:
	cUITextView();
	~cUITextView();

	void Setup_Text(ST_SIZE stSize, string text, eFONTTYPE fontType = E_FONT_DEFAULT);
	virtual void Render(LPD3DXSPRITE pSprite);
	void SetText(string text) { m_sText = text; }
};

