#pragma once
class cPyramid
{
public:
	cPyramid();
	~cPyramid();

private:
	std::vector<ST_PC_VERTEX>	m_vecVertex;	
	D3DXMATRIXA16				m_matR;
	D3DMATERIAL9				m_stMtl;

	D3DXVECTOR3					m_vPosition;
public:

	void Setup(D3DXCOLOR c, D3DXMATRIXA16& mat);	
	void Render();
	void SetPosition(D3DXVECTOR3 pos);


};

