#pragma once
#include "cUIObject.h"
class cUIButton :
	public cUIObject
{
	string m_sName;
	eUISTATE m_eCurrentState;
	map<int, LPDIRECT3DTEXTURE9> m_mapTexture; 
public:
	cUIButton();
	~cUIButton();

	void Setup_button(int width, int height, string name, string sPath_idle, string sPath_mouseover, string sPath_clicked);
	virtual void Update(float deltaTime);
	virtual void Render(LPD3DXSPRITE pSprite);
	virtual void Destroy();
	eUISTATE GetCurrentState() { return m_eCurrentState; }
};

