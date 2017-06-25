#pragma once

#define TEXTURE cTextureManager::GetInstance()

class cTextureManager
{

private:
	SINGLETON(cTextureManager);

	map <string, LPDIRECT3DTEXTURE9> m_mapTexture;
	map<string, D3DXIMAGE_INFO> m_mapImageInfo;

public:
	LPDIRECT3DTEXTURE9 GetTexture(char* szFullPath);
	LPDIRECT3DTEXTURE9 GetTexture(std::string& sFullPath);
	void Destroy();

	LPDIRECT3DTEXTURE9	GetTexture(char* szFullPath, D3DXIMAGE_INFO* pImageInfo);
	LPDIRECT3DTEXTURE9	GetTexture(string szFullPath, OUT D3DXIMAGE_INFO& pImageInfo);
};

