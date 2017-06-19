#include "stdafx.h"
#include "cHeightMap.h"
#include "cMtlTex.h"


cHeightMap::cHeightMap() : m_pMesh(NULL), m_nCellSpace(0.0f), m_nBrushSize_Outside(0), m_nBrushSize_Inside(0), m_fFixedHeight(0), m_nFaceIndex(-1),
m_vCursorPos(0,0,0), m_nBrushDepth_OutSide(0), m_nBrushDepth_InSide(0), m_nBrushSharpness(0), m_pTexture_Brush_Inside(NULL), m_pTexture_Brush_Outside(NULL),
m_isCursorOn(true), m_nOption(0), m_sMesh(NULL)
{
}


cHeightMap::~cHeightMap()
{
	SAFE_RELEASE(m_pMesh);
	for each(auto p in m_vecMtlTex)
	{
		SAFE_RELEASE(p);
	}
	SAFE_RELEASE(m_pTexture_Brush_Inside);
	SAFE_RELEASE(m_pTexture_Brush_Outside); 
}

// 정방형 맵을 만들어주는 함수
void cHeightMap::Setup(int cellPerRow, float cellSpace)
{
	SetWire(false);

	m_nCellSpace = cellSpace;

	D3DXVECTOR3 startPos = D3DXVECTOR3(cellPerRow * 0.5f * -1, 0.0f, cellPerRow * 0.5f * -1);

	// >> 맵의 Mtl & Tex 설정
	cMtlTex* pMtlTex = new cMtlTex;
	pMtlTex->SetTexture(TEXTURE->GetTexture("map/SkyBox1/Bottom.bmp"));

	D3DMATERIAL9 stMtl;
	ZeroMemory(&stMtl, sizeof(D3DMATERIAL9));
	stMtl.Ambient = (D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f));
	stMtl.Diffuse = (D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f));
	stMtl.Specular = (D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f));

	pMtlTex->SetMaterial(stMtl);

	m_vecMtlTex.push_back(pMtlTex);
	// << 

	m_pTexture_Brush_Inside = TEXTURE->GetTexture("Image/range/range_in.png");

	int nNumVertex = pow(cellPerRow + 1, 2);

	int nRow = cellPerRow + 1;
	int nCol = nRow;
	int nTileN = nRow - 1;
	m_nCellPerRow = cellPerRow;

	vector<ST_PNT_VERTEX>vecVertex(nNumVertex);
	m_vecVertex.clear();
	m_vecVertex.resize(nNumVertex);

	vector<DWORD> vecIndex;
	vecIndex.reserve(nTileN * nTileN * 2 * 3);

	for (int i = 0; i < nNumVertex; i++)
	{
		ST_PNT_VERTEX v;
		v.p = D3DXVECTOR3(i%nCol * m_nCellSpace, 0.0f, i / nCol * m_nCellSpace) + startPos;
		v.n = D3DXVECTOR3(0, 1, 0);
		v.t = D3DXVECTOR2((i%nCol) / (float)nCol, (i / nCol) / (float)nCol);
		vecVertex[i] = v;
		m_vecVertex[i] = v;
	}

	for (int z = 1; z < nTileN; ++z)
	{
		for (int x = 1; x < nTileN; ++x)
		{
			int left = (z + 0) * nCol + x - 1;
			int right = (z + 0) * nCol + x + 1;
			int up = (z + 1) * nCol + x + 0;
			int down = (z - 1) * nCol + x + 0;

			D3DXVECTOR3 leftToRight = m_vecVertex[right].p - m_vecVertex[left].p;
			D3DXVECTOR3 downToUp = m_vecVertex[up].p - m_vecVertex[down].p;

			D3DXVECTOR3 normal;
			D3DXVec3Cross(&normal, &downToUp, &leftToRight);
			D3DXVec3Normalize(&normal, &normal);

			int nIndex = z*nCol + x;
			vecVertex[nIndex].n = normal;
			m_vecVertex = vecVertex;
		}
	}

	for (int z = 0; z < nTileN; z++)
	{
		for (int x = 0; x < nTileN; ++x)
		{
			int _0 = (z + 0) * nCol + x + 0;
			int _1 = (z + 0) * nCol + x + 1;
			int _2 = (z + 1) * nCol + x + 0;
			int _3 = (z + 1) * nCol + x + 1;

			vecIndex.push_back(_0);
			vecIndex.push_back(_2);
			vecIndex.push_back(_3);
			vecIndex.push_back(_0);
			vecIndex.push_back(_3);
			vecIndex.push_back(_1);
		}
	}

	m_vecIndex.clear();
	m_vecIndex = vecIndex;

	D3DXCreateMeshFVF(vecIndex.size() / 3, vecVertex.size(), D3DXMESH_MANAGED | D3DXMESH_32BIT, ST_PNT_VERTEX::FVF, DEVICE, &m_pMesh);

	ST_PNT_VERTEX *pV = NULL;
	m_pMesh->LockVertexBuffer(0, (LPVOID*)&pV);
	memcpy(pV, &vecVertex[0], vecVertex.size() * sizeof(ST_PNT_VERTEX));
	m_pMesh->UnlockVertexBuffer();

	DWORD* pI = NULL;
	m_pMesh->LockIndexBuffer(0, (LPVOID*)&pI);
	memcpy(pI, &vecIndex[0], vecIndex.size() * sizeof(DWORD));
	m_pMesh->UnlockIndexBuffer();

	DWORD* pA = NULL;
	m_pMesh->LockAttributeBuffer(0, &pA);
	ZeroMemory(pA, vecIndex.size() / 3 * sizeof(DWORD));
	m_pMesh->UnlockAttributeBuffer();

	vector<DWORD> vecAdj(vecIndex.size());
	m_pMesh->GenerateAdjacency(0.0f, &vecAdj[0]);

	m_sMesh = &m_pMesh;
	//m_pMesh->OptimizeInplace(D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_COMPACT | D3DXMESHOPT_VERTEXCACHE, &vecAdj[0], 0, 0, 0);
}

// Load 후에 초기화 해주는 함수
void cHeightMap::Setup(int cellPerRow, float cellSpace, vector<ST_PNT_VERTEX> vecVertex, vector<DWORD> vecIndex)
{
	SetWire(false);

	m_nCellSpace = cellSpace;

	D3DXVECTOR3 startPos = D3DXVECTOR3(cellPerRow * 0.5f * -1, 0.0f, cellPerRow * 0.5f * -1);

	m_pTexture_Brush_Inside = TEXTURE->GetTexture("Image/range/range_in.png");

	int nNumVertex = pow(cellPerRow + 1, 2);

	int nRow = cellPerRow + 1;
	int nCol = nRow;
	int nTileN = nRow - 1;
	m_nCellPerRow = cellPerRow;

	m_vecVertex = vecVertex;
	m_vecIndex = vecIndex;
}

void cHeightMap::Update()
{
	PickingCursor();
	if (m_isCursorOn) SetMapHeight_Inside();
	//SetMapHeight_Outside();
}

bool cHeightMap::GetHeight(IN float x, OUT float&y, IN float z)
{
	return true;
}

void cHeightMap::Render()
{
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);
	DEVICE->SetTransform(D3DTS_WORLD, &matWorld);

	if(m_drawWired) 
		DEVICE->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);


	DEVICE->SetRenderState(D3DRS_LIGHTING, false);
	DEVICE->SetMaterial(&(m_vecMtlTex[0]->GetMaterial()));
	DEVICE->SetTexture(0, m_vecMtlTex[0]->GetTexture());
	m_pMesh->DrawSubset(0);


	//DEVICE->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	if(m_isCursorOn) RenderBrush();
}

void cHeightMap::RenderBrush()
{
	if (m_nFaceIndex < 0) return;
	vector<ST_PNT_VERTEX> vecVertex_Inner;
	vector<ST_PC_VERTEX> vecVertex_Outer;

	ST_PNT_VERTEX vertex;

	// >> Inner Brush의 삼각형 면 만들어주는 부분	
	for (int i = 0; i < m_vecFace_Inside.size(); i++)
	{
		int index;
		index = m_vecFace_Inside[i] * 3 + 0;;

		if (index < 0 || index > m_vecIndex.size() - 1) continue;

		if (m_vecIndex[index + 0] >= 0 && m_vecIndex[index + 0] < m_vecVertex.size() - 1 &&
			m_vecIndex[index + 1] >= 0 && m_vecIndex[index + 1] < m_vecVertex.size() - 1 &&
			m_vecIndex[index + 2] >= 0 && m_vecIndex[index + 2] < m_vecVertex.size() - 1 &&
			MATH->CheckDistance(D3DXVECTOR2(m_vecVertex[m_vecIndex[m_vecFace_Inside[i] * 3 + 0]].p.x, m_vecVertex[m_vecIndex[m_vecFace_Inside[i] * 3 + 0]].p.y),
				D3DXVECTOR2(m_vCursorPos.x, m_vCursorPos.y), m_nBrushSize_Inside))
		{
			vertex.n = D3DXVECTOR3(0, 1, 0);
			vertex.p = m_vecVertex[m_vecIndex[m_vecFace_Inside[i] * 3 + 0]].p;
			{
				int faceCount = 1 + m_nBrushSize_Inside * 2;
				int faceIndex = m_vecFace_Inside[i];
				int col = (m_nFaceIndex % m_nCellPerRow) - (faceIndex % m_nCellPerRow) + m_nBrushSize_Inside;
				int row = (m_nFaceIndex / m_nCellPerRow) - (faceIndex / m_nCellPerRow) + m_nBrushSize_Inside + 1;
				//vertex.t = D3DXVECTOR2((float)col / (float)faceCount, (float)row / (float)faceCount);
				vertex.t = D3DXVECTOR2((float)1 / (float)3 * (float)(faceIndex % 3), (float)1 / (float)3 * (float)(faceIndex % 3 + 1));
				int a = 0;
			}
			vecVertex_Inner.push_back(vertex);

			vertex.p = m_vecVertex[m_vecIndex[m_vecFace_Inside[i] * 3 + 1]].p;
			if (m_vecFace_Inside[i] % 2 == 0)
			{
				int faceCount = 1 + m_nBrushSize_Inside * 2;
				int faceIndex = m_vecFace_Inside[i];
				int col = (m_nFaceIndex % m_nCellPerRow) - (faceIndex % m_nCellPerRow) + m_nBrushSize_Inside;
				int row = (m_nFaceIndex / m_nCellPerRow) - (faceIndex / m_nCellPerRow) + m_nBrushSize_Inside;
				//vertex.t = D3DXVECTOR2((float)col / (float)faceCount, (float)row / (float)faceCount);
				vertex.t = D3DXVECTOR2((float)1 / (float)3 * (float)(faceIndex % 3), (float)1 / (float)3 * (float)(faceIndex % 3));
			}
			else
			{
				int faceCount = 1 + m_nBrushSize_Inside * 2;
				int faceIndex = m_vecFace_Inside[i];
				int col = (m_nFaceIndex % m_nCellPerRow) - (faceIndex % m_nCellPerRow) + m_nBrushSize_Inside + 1;
				int row = (m_nFaceIndex / m_nCellPerRow) - (faceIndex / m_nCellPerRow) + m_nBrushSize_Inside;
				//vertex.t = D3DXVECTOR2((float)col / (float)faceCount, (float)row / (float)faceCount);
				vertex.t = D3DXVECTOR2((float)1 / (float)3 * (float)(faceIndex % 3 + 1), (float)1 / (float)3 * (float)(faceIndex % 3));
			}
			vecVertex_Inner.push_back(vertex);

			vertex.p = m_vecVertex[m_vecIndex[m_vecFace_Inside[i] * 3 + 2]].p;
			if (m_vecFace_Inside[i] % 2 == 0)
			{
				int faceCount = 1 + m_nBrushSize_Inside * 2;
				int faceIndex = m_vecFace_Inside[i];
				int col = (m_nFaceIndex % m_nCellPerRow) - (faceIndex % m_nCellPerRow) + m_nBrushSize_Inside + 1;
				int row = (m_nFaceIndex / m_nCellPerRow) - (faceIndex / m_nCellPerRow) + m_nBrushSize_Inside;
				//vertex.t = D3DXVECTOR2((float)col / (float)faceCount, (float)row / (float)faceCount);
				vertex.t = D3DXVECTOR2((float)1 / (float)3 * (float)(faceIndex % 3 + 1), (float)1 / (float)3 * (float)(faceIndex % 3));
			}
			else
			{
				int faceCount = 1 + m_nBrushSize_Inside * 2;
				int faceIndex = m_vecFace_Inside[i];
				int col = (m_nFaceIndex % m_nCellPerRow) - (faceIndex % m_nCellPerRow) + m_nBrushSize_Inside + 1;
				int row = (m_nFaceIndex / m_nCellPerRow) - (faceIndex / m_nCellPerRow) + m_nBrushSize_Inside + 1;
				//vertex.t = D3DXVECTOR2((float)col / (float)faceCount, (float)row / (float)faceCount);
				vertex.t = D3DXVECTOR2((float)1 / (float)3 * (float)(faceIndex % 3 + 1), (float)1 / (float)3 * (float)(faceIndex % 3 + 1));
			}
			vecVertex_Inner.push_back(vertex);
		}
	}

	m_sVeretx = &vecVertex_Inner;

	// <<

	// >> Outer Brush의 삼각형 면 만들어주는 부분
	//vertex.c = D3DCOLOR_ARGB(255, 255, 255, 0);
	//for (int i = 0; i < m_vecFace_Outside.size(); i++)
	//{
	//	int index;
	//	index = m_vecFace_Outside[i] * 3 + 0;;
	//
	//	if (index < 0 || index > m_vecIndex.size() - 1) continue;
	//
	//	if (m_vecIndex[index + 0] >= 0 && m_vecIndex[index + 0] < m_vecVertex.size() - 1 &&
	//		m_vecIndex[index + 1] >= 0 && m_vecIndex[index + 1] < m_vecVertex.size() - 1 &&
	//		m_vecIndex[index + 2] >= 0 && m_vecIndex[index + 2] < m_vecVertex.size() - 1 &&
	//		MATH->CheckDistance(D3DXVECTOR2(m_vecVertex[m_vecIndex[m_vecFace_Outside[i] * 3 + 0]].p.x, m_vecVertex[m_vecIndex[m_vecFace_Outside[i] * 3 + 0]].p.y),
	//			D3DXVECTOR2(m_vCursorPos.x, m_vCursorPos.y), (float)m_nBrushSize_Outside * sqrt(2)))
	//	{
	//		vertex.p = m_vecVertex[m_vecIndex[m_vecFace_Outside[i] * 3 + 0]].p;
	//		vecVertex_Inner.push_back(vertex);
	//		vertex.p = m_vecVertex[m_vecIndex[m_vecFace_Outside[i] * 3 + 1]].p;
	//		vecVertex_Inner.push_back(vertex);
	//		vertex.p = m_vecVertex[m_vecIndex[m_vecFace_Outside[i] * 3 + 2]].p;
	//		vecVertex_Inner.push_back(vertex);
	//	}
	//}
	// <<

	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);
	DEVICE->SetTransform(D3DTS_WORLD, &matWorld);
	DEVICE->SetFVF(ST_PNT_VERTEX::FVF);
	DEVICE->SetRenderState(D3DRS_LIGHTING, false);
	DEVICE->SetTexture(0, m_pTexture_Brush_Inside);

	//DEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	//
	//DEVICE->SetRenderState(D3DRS_ZWRITEENABLE, false)s;

	//
	//DEVICE->SetRenderState(D3DRS_POINTSCALEENABLE, true);
	//DEVICE->SetRenderState(D3DRS_POINTSIZE, FtoDw(5.0f));
	//DEVICE->SetRenderState(D3DRS_POINTSCALE_A, FtoDw(0.0f));
	//DEVICE->SetRenderState(D3DRS_POINTSCALE_B, FtoDw(0.0f));
	//DEVICE->SetRenderState(D3DRS_POINTSCALE_C, FtoDw(1.0f));	// 10.0f 바꿔서 확인해보셈
	//DEVICE->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
	//DEVICE->SetRenderState(D3DRS_POINTSIZE_MIN, FtoDw(0.0f));
	//DEVICE->SetRenderState(D3DRS_POINTSIZE_MAX, FtoDw(1000.0f));		// 변경해보기

	DEVICE->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	DEVICE->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	DEVICE->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	//DEVICE->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//DEVICE->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//DEVICE->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCALPHA);
	//DEVICE->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);


	if (vecVertex_Inner.size() > 0) DEVICE->DrawPrimitiveUP(D3DPT_TRIANGLELIST, vecVertex_Inner.size() / 3, &vecVertex_Inner[0], sizeof(ST_PNT_VERTEX));

	//if (vecVertex_Outer.size() > 0)DEVICE->DrawPrimitiveUP(D3DPT_TRIANGLELIST, vecVertex_Outer.size() / 3, &vecVertex_Outer[0], sizeof(ST_PC_VERTEX));

	//DEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	//DEVICE->SetRenderState(D3DRS_ZWRITEENABLE, true);
}

void cHeightMap::PickingCursor()
{
	POINT cursor;
	GetCursorPos(&cursor);
	ScreenToClient(g_hWnd, &cursor);
	D3DXVECTOR2 cursorPos = D3DXVECTOR2(cursor.x, cursor.y);
	float minX = (float)m_nCellPerRow * 0.5f * m_nCellSpace * -1;
	float maxX = (float)m_nCellPerRow * 0.5f * m_nCellSpace * +1;

	// 현재 커서와 맵이 충돌함을 확인하고, 각 멤버변수 채워주기
	cRay::IsCollidedWithMesh(cursorPos, m_pMesh, m_nFaceIndex, m_vCursorPos, minX, maxX);

	// 계산의 기준이 될 인덱스 재계산
	int nFaceIndex = (m_nFaceIndex % 2 == 0) ? m_nFaceIndex : m_nFaceIndex - 1;

	// >> 각 브러쉬의 Face를 벡터에 담는 부분
	m_vecFace_Inside.clear();
	m_vecFace_Outside.clear();
	for (int row = m_nBrushSize_Inside * -1; row <= m_nBrushSize_Inside; row++)
	{
		for (int col = m_nBrushSize_Inside * -1 * 2 - 1; col <= m_nBrushSize_Inside * 2; col++)
		{
			int nCurrentFaceIndex = nFaceIndex +(row * m_nCellPerRow * 2) + col + 1;
			
			if (nCurrentFaceIndex < 0) continue;
			if (nCurrentFaceIndex > (m_vecIndex.size() - 1) / 3) continue;
	
			m_vecFace_Inside.push_back(nCurrentFaceIndex);
		}
	}
	for (int row = m_nBrushSize_Outside * -1; row <= m_nBrushSize_Outside; row++)
	{
		for (int col = m_nBrushSize_Outside * -1 * 2 - 1; col <= m_nBrushSize_Outside * 2; col++)
		{
			int nCurrentFaceIndex = nFaceIndex + (row * m_nCellPerRow * 2) + col + 1;
	
			if (nCurrentFaceIndex < 0) continue;
			if (nCurrentFaceIndex >= m_vecIndex.size() - 1) continue;
	
			m_vecFace_Outside.push_back(nCurrentFaceIndex);
		}
	}
	// <<

}

void cHeightMap::SetMapHeight_Inside()
{
	if (!INPUT->IsMousePress(MOUSE_LEFT)) return;

	POINT cursor;
	GetCursorPos(&cursor);
	ScreenToClient(g_hWnd, &cursor);
	D3DXVECTOR2 cursorPos = D3DXVECTOR2(cursor.x, cursor.y);
	float minX = (float)m_nCellPerRow * 0.5f * m_nCellSpace * -1;
	float maxX = (float)m_nCellPerRow * 0.5f * m_nCellSpace * +1;

	// 맵 밖에 있으면 중단
	if(!cRay::IsMounseInMap(cursorPos, minX, maxX)) return;

	// >> 맵 높이 조정해주는 부분
	for (int i = 0; i < m_vecFace_Inside.size(); i++)
	{
		int faceIndex = m_vecFace_Inside[i];

		if (m_vecIndex[faceIndex * 3 + 0] >= 0 &&
			m_vecIndex[faceIndex * 3 + 0] < m_vecVertex.size() - 1 &&
			m_vecIndex[faceIndex * 3 + 1] >= 0 &&
			m_vecIndex[faceIndex * 3 + 1] < m_vecVertex.size() - 1 &&
			m_vecIndex[faceIndex * 3 + 2] >= 0 &&
			m_vecIndex[faceIndex * 3 + 2] < m_vecVertex.size() - 1)
		{
			D3DXVECTOR3& v0 = m_vecVertex[m_vecIndex[faceIndex * 3 + 0]].p;
			D3DXVECTOR3& v1 = m_vecVertex[m_vecIndex[faceIndex * 3 + 1]].p;
			D3DXVECTOR3& v2 = m_vecVertex[m_vecIndex[faceIndex * 3 + 2]].p;

			D3DXVECTOR2 currentPos, standardPos;
			standardPos = D3DXVECTOR2(m_vCursorPos.x, m_vCursorPos.z);

			currentPos = D3DXVECTOR2(v0.x, v0.z);
			if (MATH->CheckDistance(currentPos, standardPos, m_nBrushSize_Inside))
			{
				if(m_nOption == 1) v0.y += m_nBrushDepth_InSide * 0.01f;
				if (m_nOption == 2) v0.y = m_fFixedHeight;
			}
			currentPos = D3DXVECTOR2(v1.x, v1.z);
			if (MATH->CheckDistance(currentPos, standardPos, m_nBrushSize_Inside))
			{
				if (m_nOption == 1) v1.y += m_nBrushDepth_InSide * 0.01f;
				if (m_nOption == 2) v1.y = m_fFixedHeight;
			}
			currentPos = D3DXVECTOR2(v2.x, v2.z);
			if (MATH->CheckDistance(currentPos, standardPos, m_nBrushSize_Inside)) 
			{
				if (m_nOption == 1) v2.y += m_nBrushDepth_InSide * 0.01f;
				if (m_nOption == 2) v2.y = m_fFixedHeight;
			}

			// 법선벡터 재 설정해주는 부분
			SetNoramlVector(m_vecVertex[m_vecIndex[faceIndex * 3 + 0]], m_vecVertex[m_vecIndex[faceIndex * 3 + 1]], m_vecVertex[m_vecIndex[faceIndex * 3 + 2]]);
		}

	}

	ST_PNT_VERTEX *pV = NULL;
	m_pMesh->LockVertexBuffer(0, (LPVOID*)&pV);
	memcpy(pV, &m_vecVertex[0], m_vecVertex.size() * sizeof(ST_PNT_VERTEX));
	m_pMesh->UnlockVertexBuffer();
	// << 
}

// 문제가 있으니 우선은 사용 안함. 수정 필요
void cHeightMap::SetMapHeight_Outside()
{
	if (!INPUT->IsMousePress(MOUSE_LEFT)) return;

	POINT cursor;
	GetCursorPos(&cursor);
	ScreenToClient(g_hWnd, &cursor);
	D3DXVECTOR2 cursorPos = D3DXVECTOR2(cursor.x, cursor.y);
	float minX = (float)m_nCellPerRow * 0.5f * m_nCellSpace * -1;
	float maxX = (float)m_nCellPerRow * 0.5f * m_nCellSpace * +1;

	// 맵 밖에 있으면 중단
	if (!cRay::IsMounseInMap(cursorPos, minX, maxX)) return;

	// >> 맵 높이 조정해주는 부분
	for (int i = 0; i < m_vecFace_Outside.size(); i++)
	{
		int faceIndex = m_vecFace_Outside[i];

		if (m_vecIndex[faceIndex * 3 + 0] >= 0 &&
			m_vecIndex[faceIndex * 3 + 0] < m_vecVertex.size() - 1 &&
			m_vecIndex[faceIndex * 3 + 1] >= 0 &&
			m_vecIndex[faceIndex * 3 + 1] < m_vecVertex.size() - 1 &&
			m_vecIndex[faceIndex * 3 + 2] >= 0 &&
			m_vecIndex[faceIndex * 3 + 2] < m_vecVertex.size() - 1)
		{
			D3DXVECTOR3& v0 = m_vecVertex[m_vecIndex[faceIndex * 3 + 0]].p;
			D3DXVECTOR3& v1 = m_vecVertex[m_vecIndex[faceIndex * 3 + 1]].p;
			D3DXVECTOR3& v2 = m_vecVertex[m_vecIndex[faceIndex * 3 + 2]].p;

			D3DXVECTOR2 currentPos, standardPos;
			standardPos = D3DXVECTOR2(m_vCursorPos.x, m_vCursorPos.z);

			// >>	만약 안쪽 브러쉬의 경우 처리 안하도록
			if (MATH->CheckDistance(currentPos, D3DXVECTOR2(v0.x, v0.z), m_nBrushSize_Inside) &&
				MATH->CheckDistance(currentPos, D3DXVECTOR2(v1.x, v1.z), m_nBrushSize_Inside) &&
				MATH->CheckDistance(currentPos, D3DXVECTOR2(v2.x, v2.z), m_nBrushSize_Inside)) continue;
			// <<

			float addingY = 0;
			currentPos = D3DXVECTOR2(v0.x, v0.z);
			GetYInFormula(v0, addingY);
			if (MATH->CheckDistance(currentPos, standardPos, m_nBrushSize_Outside)) v0.y += addingY;
			currentPos = D3DXVECTOR2(v1.x, v1.z);
			GetYInFormula(v1, addingY);
			if (MATH->CheckDistance(currentPos, standardPos, m_nBrushSize_Outside)) v1.y += addingY;
			currentPos = D3DXVECTOR2(v2.x, v2.z);
			GetYInFormula(v2, addingY);
			if (MATH->CheckDistance(currentPos, standardPos, m_nBrushSize_Outside)) v2.y += addingY;
		}

	}

	ST_PNT_VERTEX *pV = NULL;
	m_pMesh->LockVertexBuffer(0, (LPVOID*)&pV);
	memcpy(pV, &m_vecVertex[0], m_vecVertex.size() * sizeof(ST_PNT_VERTEX));
	m_pMesh->UnlockVertexBuffer();
	// << 
}

void cHeightMap::GetYInFormula(IN D3DXVECTOR3& vertex, OUT float& addingY)
{
	// 공식
	float y = 0;
	
	// >> 공식 계산
	if (m_nBrushSharpness == -1)
	{
		// y = a(x-b)^2 + c
		float b = (m_nBrushSize_Outside - m_nBrushSize_Inside);
		float c = 0;
		float a = (m_nBrushDepth_InSide - c) / pow(0 - b, 2);
		float x = MATH->Distance(D3DXVECTOR2(m_vCursorPos.x, m_vCursorPos.z), D3DXVECTOR2(vertex.x, vertex.z))
			- m_nBrushSize_Inside;
		y = a * pow(x - b, 2) + c;
	}
	else if (m_nBrushSharpness == 0)
	{
		// y = a * x + b
		float b = m_nBrushDepth_InSide;

	}
	else if (m_nBrushSharpness == 1)
	{

	}
	
	addingY = y;
}

void cHeightMap::SetNoramlVector(IN OUT ST_PNT_VERTEX& v0, IN OUT ST_PNT_VERTEX& v1, IN OUT ST_PNT_VERTEX& v2)
{
	D3DXVECTOR3 u = v0.p - v1.p;
	D3DXVECTOR3 v = v2.p - v1.p;
	D3DXVECTOR3 n;

	D3DXVec3Cross(&n, &u, &v);
	D3DXVec3Normalize(&n, &n);

	v0.n = n;
	v1.n = n;
	v2.n = n;	
}

LPD3DXMESH cHeightMap::GetMesh()
{
	return *m_sMesh;
}

vector<ST_PNT_VERTEX> cHeightMap::GetVertex()
{
	return *m_sVeretx;
}
