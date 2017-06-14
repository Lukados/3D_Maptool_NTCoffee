#pragma once

#define FONT cFontManager::GetInstance()

class cFontManager
{
	SINGLETON(cFontManager);
	map<eFONTTYPE, LPD3DXFONT>	m_mapFont;

public:
	LPD3DXFONT GetFont(eFONTTYPE e);
	void Destroy();
};

