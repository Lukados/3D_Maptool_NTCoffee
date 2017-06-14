#pragma once
#include "cUIObject.h"

class cObjectDB;
enum E_S_OBJECTID;

struct ST_SINDEX
{
	D3DXVECTOR3			vPosition;
	ST_SIZE				stSize;
	int					eCurrentSID;
	eUISTATE			eUIState;
	LPDIRECT3DTEXTURE9	texture;

	void SetTexture(LPDIRECT3DTEXTURE9 t) { texture = t; }
	void SetSID(int id) { eCurrentSID = id; }

	ST_SINDEX(D3DXVECTOR3 position, ST_SIZE size, E_S_OBJECTID sID, eUISTATE state)
	{
		vPosition = position;
		stSize = size;
		eCurrentSID = sID;
		eUIState = state;
	}
};

class cRadioButton : public cUIObject
{
	vector<ST_SINDEX*>					m_vecSIndex;
	map<eUISTATE, LPDIRECT3DTEXTURE9>	m_mapTexture;
	SYNTHESIZE(bool, m_isClicked, IsClicked);

public:
	cRadioButton();
	~cRadioButton();

	void Setup_RadioButton();
	void Render_ButtonUI(LPD3DXSPRITE pSprite);
	void Render_Sprite(LPD3DXSPRITE pSprite);


	virtual void Update(float deltaTime);
	virtual void Render(LPD3DXSPRITE pSprite);

	virtual void Destroy();

	void SetTexture(int index, LPDIRECT3DTEXTURE9 texture);
	void SetSID(int index, int eSID);
	void Add_RadioButton(D3DXVECTOR3 position, ST_SIZE size, E_S_OBJECTID sID, eUISTATE state, LPDIRECT3DTEXTURE9 t);

	vector<ST_SINDEX*>	GetVecSIndex() { return m_vecSIndex; }
	int GetSID();
};

