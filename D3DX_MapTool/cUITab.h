#pragma once

#include "cUIObject.h"

class cUITab : public cUIObject
{
	string m_sTitle;
	map<int, LPDIRECT3DTEXTURE9> m_mapColor;
	int m_currentState;


public:
	cUITab();
	~cUITab();

	void Setup_tab(int titleWidth, int menuHeight, string title, string path_idleState, string path_selected);
	virtual void Update(float deltaTime);
	virtual void Render(LPD3DXSPRITE pSprite);
};

