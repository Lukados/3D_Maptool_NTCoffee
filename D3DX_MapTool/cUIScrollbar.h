#pragma once

#include "cUIObject.h"

// 사용 순서 : cUIObject::Setup -> Setup_scroll -> Addchilde -> Setup_bar
class cUIScrollbar : public cUIObject
{
	int					m_nMinValue;
	int					m_nMaxValue;
	SYNTHESIZE(int, m_nValue, Value);
	D3DXVECTOR2			m_vBarPos;
	string				m_sName;
	ST_SIZE				m_stBarSize;
	eUISTATE			m_eCurrentState;
	LPDIRECT3DTEXTURE9	m_pBarTexture;
	LPDIRECT3DTEXTURE9	m_pLineTexture;
public:
	cUIScrollbar();
	~cUIScrollbar();

	void Setup_scroll(int width, int height, string name, string path_bar, int minValue, int maxValue);
	void Setup_bar(int barW, int barH);
	virtual void Update(float deltaTime);
	virtual void Render(LPD3DXSPRITE pSprite);
	virtual void Destroy();
};

