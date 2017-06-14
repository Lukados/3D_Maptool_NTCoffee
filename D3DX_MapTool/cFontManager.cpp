#include "stdafx.h"
#include "cFontManager.h"


cFontManager::cFontManager()
{
}


cFontManager::~cFontManager()
{
}

LPD3DXFONT cFontManager::GetFont(eFONTTYPE e)
{
	if (m_mapFont.find(e) != m_mapFont.end()) return m_mapFont[e];

	D3DXFONT_DESC fd;
	ZeroMemory(&fd, sizeof(D3DXFONT_DESC));

	if (e == E_FONT_DEFAULT)
	{
		fd.Height = 16;
		fd.Width = 8;
		fd.Weight = FW_BOLD;
		fd.Italic = false;
		fd.CharSet = DEFAULT_CHARSET;
		fd.OutputPrecision = OUT_DEFAULT_PRECIS;
		fd.PitchAndFamily = FF_DONTCARE;
		strcpy_s(fd.FaceName, "πŸ≈¡√º");
	}
	//else if (e == E_QUEST)
	//{
	//	fd.Height = 50;
	//	fd.Width = 25;
	//	fd.Weight = FW_MEDIUM;
	//	fd.Italic = false;
	//	fd.CharSet = DEFAULT_CHARSET;
	//	fd.OutputPrecision = OUT_DEFAULT_PRECIS;
	//	fd.PitchAndFamily = FF_DONTCARE;
	//	AddFontResource("font/umberto.ttf");
	//	strcpy_s(fd.FaceName, "umberto");
	//}

	D3DXCreateFontIndirect(DEVICE, &fd, &m_mapFont[e]);
	return m_mapFont[e];
}

void cFontManager::Destroy()
{
	for each(auto it in m_mapFont)
	{
		SAFE_RELEASE(it.second);
	}
}