#pragma once
class cUIObject
{
protected:
	vector<cUIObject*> m_vecChild;
	D3DXMATRIXA16 m_matWorld;
	SYNTHESIZE(D3DXVECTOR3, m_vPosition, Position);
	SYNTHESIZE(cUIObject*, m_pParent, Parent);
	SYNTHESIZE(ST_SIZE, m_stSize, Size);
	SYNTHESIZE(eUIOBJECT, m_uiTag, Tag);
	SYNTHESIZE(bool, m_isHidden, Hidden);

public:
	cUIObject();
	~cUIObject();

	virtual void Setup(D3DXVECTOR3 pos, eUIOBJECT tag);
	virtual void Update(float deltaTime);
	virtual void Render(LPD3DXSPRITE pSprite);
	virtual void Destroy();
	virtual void AddChild(cUIObject* pChild);
	virtual void SetHiddenAll(bool isHidden);
	virtual D3DXVECTOR2 LeftTop();
	virtual D3DXVECTOR2 LeftVCenter();
	virtual D3DXVECTOR2 RightBottom();
	virtual D3DXVECTOR2 RightVCenter();
};

