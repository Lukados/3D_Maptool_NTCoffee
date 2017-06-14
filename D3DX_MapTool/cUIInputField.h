#pragma once
#include "cUIObject.h"
class cUIInputField :
	public cUIObject
{
	SYNTHESIZE(string, m_sText, Text);
	eUISTATE m_eCurrentState;
	map<int, LPDIRECT3DTEXTURE9> m_mapTexture;
	SYNTHESIZE(bool,  m_isOn, IsOn);
public:
	cUIInputField();
	~cUIInputField();

	void Setup_field(int width, int height, string sPath_idle, string sPath_selected);
	virtual void Update(float deltaTime);
	virtual void Render(LPD3DXSPRITE pSprite);
	virtual void Destroy();
	eUISTATE GetCurrentState() { return m_eCurrentState; }
};

