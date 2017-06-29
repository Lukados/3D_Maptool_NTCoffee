#pragma once
#include "cIMap.h"

class cMtlTex;

class cHeightMap :
	public cIMap
{
	SYNTHESIZE(LPD3DXMESH,	m_pMesh, Mesh);
	SYNTHESIZE(vector<ST_PNT_VERTEX>, m_vecVertex, VecVertex);
	SYNTHESIZE(vector<cMtlTex*>	,	m_vecMtlTex, VecMtlTex);
	SYNTHESIZE(vector<DWORD>, m_vecIndex, VecIndex);

	SYNTHESIZE(float, m_nCellSpace, CellSpace);
	SYNTHESIZE(int	, m_nCellPerRow, CellPerRow);
	
	SYNTHESIZE(int,	m_nBrushSize_Outside, BrushOutside);
	SYNTHESIZE(int, m_nBrushSize_Inside, BrushInside);
	SYNTHESIZE(int, m_nBrushDepth_OutSide, BrushDepthOutside);
	SYNTHESIZE(int, m_nBrushDepth_InSide, BrushDepthInside);
	SYNTHESIZE(int, m_nBrushSharpness, BrushSharpness);
	SYNTHESIZE(float, m_fFixedHeight, FixedHeight);
	int						m_nFaceIndex;
	SYNTHESIZE(D3DXVECTOR3, m_vCursorPos, CursorPosition);
	vector<DWORD>			m_vecFace_Inside;
	vector<DWORD>			m_vecFace_Outside;
	LPDIRECT3DTEXTURE9		m_pTexture_Brush_Inside;
	LPDIRECT3DTEXTURE9		m_pTexture_Brush_Outside;
	 
	SYNTHESIZE(bool, m_isCursorOn, IsCursorOn);
	SYNTHESIZE(int, m_nOption, Option);				// Def Brush = 1, Fixed Brush = 2,
public:
	cHeightMap();
	~cHeightMap();

	void Setup(int cellPerRow, float cellSpace);
	void Setup(int cellPerRow, float cellSpace, vector<ST_PNT_VERTEX> vecVertex, vector<DWORD> vecIndex);
	void Update();
	virtual bool GetHeight(IN float x, OUT float&y, IN float z) override;
	virtual void Render() override;
	void RenderBrush();
	void PickingCursor();
	void SetMapHeight_Inside();
	void SetMapHeight_Outside();
	void GetYInFormula(IN D3DXVECTOR3& vertex, OUT float& addingY);
	void SetNoramlVector(IN OUT ST_PNT_VERTEX& v0, IN OUT ST_PNT_VERTEX& v1, IN OUT ST_PNT_VERTEX& v2);
	LPD3DXMESH* m_sMesh;
	LPD3DXMESH GetMesh();
	vector<ST_PNT_VERTEX>* m_sVeretx;
	vector<ST_PNT_VERTEX> GetVertex();	
};

