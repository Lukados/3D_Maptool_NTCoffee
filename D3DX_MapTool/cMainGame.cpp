#include "stdafx.h"
#include "cMainGame.h"

#include "cCamera.h"
#include "cGrid.h"
#include "cHeightMap.h"
#include "cMtlTex.h"

#include "cUITab.h"
#include "cUIButton.h"
#include "cUIScrollbar.h"
#include "cUITextView.h"
#include "cRadioButton.h"
#include "cUIInputField.h"

#include "cObjLoader.h"
#include "cObjectDB.h"
#include "cConstruct.h"
#include "cSkyBox.h"

#include "cFog.h"
#include "cWeather.h"

#include "cShadowManager.h"

cMainGame::cMainGame() : m_pCamera(NULL), m_vStandardPos(0,0,0), m_pGrid(NULL), m_nSize(150), m_fCellSpace(1.0f), m_pMap(NULL), m_vDir(0,0,1), m_pUISprite(NULL), m_vCursorPos(0,0,0),
m_pUITab_Menu(NULL), m_pUITab_Map(NULL), m_pUITab_Object(NULL), m_pUITab_Effect(NULL), m_fCamSpeed(10), m_isUpdateMap(false), m_isUpdateObj(false),
m_pRadioButton_Brush(NULL), m_pUIButton_GetHeight(NULL), m_pUIInputField_SetHeight(NULL), m_pUIButton_SetHeight(NULL),
m_pUIScroll_BrushSize_inside(NULL), m_pUIScroll_BrushSize_Outside(NULL),
m_pUIScroll_BrushDepth_inside(NULL), m_pUIScroll_BrushSharpness(NULL),
m_pUIInputField_CellSpace(NULL), m_pUIInputField_CellNum(NULL),m_pUIInputField_FilePath(NULL), m_getHeight(false),
m_pUIButton_create(NULL),m_pUIButton_save(NULL),m_pUIButton_load(NULL),
m_pUIButton_LLeft(NULL),m_pUIButton_LRight(NULL),m_pUIText_LID(NULL),
m_pUIButton_MLeft(NULL),m_pUIButton_MRight(NULL),m_pUIText_MID(NULL),
m_pUIButton_SLeft(NULL),m_pUIButton_SRight(NULL),m_pUIText_SID(NULL),
m_nObject_LIndex(0), m_nObject_MIndex(0), m_nObject_SIndex(0), m_nPage(1),
m_pConstruct(NULL), m_pRadioButton_Object(NULL),
m_pSkyBox(NULL), m_pRadioButton_Fog(NULL), m_pFog(NULL), m_isFogOn(false),
m_pUIButton_Fog_Minus(NULL), m_pUIButton_Fog_Plus(NULL), m_pUIText_Fog_Minus(NULL), m_pUIText_Fog_Plus(NULL), m_nPassIndex(2),
m_pUIButton_Shadow_Minus(NULL), m_pUIButton_Shadow_Plus(NULL), m_pUIText_Shadow_Minus(NULL), m_pUIText_Shadow_Plus(NULL),
m_pRadioButton_Shadow(NULL), m_isShodowOn(false),
m_pUIButton_Snow_Minus(NULL), m_pUIButton_Snow_Plus(NULL), m_pUIText_Snow_Minus(NULL), m_pUIText_Snow_Plus(NULL),
m_pRadioButton_Snow(NULL), m_isSnowOn(false), 
m_pSphere(NULL), m_vSpherePos(0, 0, 0),
m_pUIButton_Rain_Minus(NULL), m_pUIButton_Rain_Plus(NULL), m_pUIText_Rain_Minus(NULL), m_pUIText_Rain_Plus(NULL),
m_pRadioButton_Rain(NULL), m_isRainOn(false), 
m_diffuseAlpha(0.5f)
{
	m_stWeather = ST_WEATHER();
	m_stShadow = ST_SHADOW();
}


cMainGame::~cMainGame()
{
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pGrid);
	SAFE_RELEASE(m_pUISprite);
	SAFE_DELETE(m_pMap);
	SAFE_DELETE(m_pSkyBox);
	for each(auto p in m_vecUITab_Image)
	{
		SAFE_DELETE(p);
	}
	m_pUITab_Menu->Destroy();
	m_pUITab_Map->Destroy();
	m_pUITab_Object->Destroy();
	m_pUITab_Effect->Destroy();
	for each(auto p in m_vecConstruct)
	{
		SAFE_DELETE(p);
	}

	m_pFog->Destroy();
	SAFE_DELETE(m_pSnow);
	SAFE_DELETE(m_pRain);
	SAFE_RELEASE(m_pSphere);

	TEXTURE->Destroy();
	DEVICE->Release();
	FONT->Destroy();
}

void cMainGame::Setup()
{
	Setup_DirLight();

	m_pCamera = new cCamera;
	m_pCamera->Setup(&m_vStandardPos);

	m_pGrid = new cGrid;
	m_pGrid->Setup(m_nSize / 2);

	OBJECTDB->Setup();

	m_mtlSPhere.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_mtlSPhere.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_mtlSPhere.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	cHeightMap* pMap = new cHeightMap;
	pMap->Setup(m_nSize, m_fCellSpace);
	m_pMap = pMap;	

	D3DXCreateSprite(DEVICE, &m_pUISprite);
	Setup_UI();
	Setup_SkyBox();
	SHADOW->Setup(m_vecConstruct);


}

void cMainGame::Update(float deltaTime)
{
	Update_UI();

	if (m_pMap)
	{
		if (m_isUpdateMap)
		{
			MoveStandardPos(deltaTime);
			SetOption();
			if (m_pRadioButton_Brush->GetSID() == E_S_OBJECTID_BLANK)
			{
				m_pMap->SetOption(1);
				m_pMap->SetIsCursorOn(true);
			}
			else if (m_pRadioButton_Brush->GetSID() == E_S_OBJECTID_BLANK2)
			{
				m_pMap->SetOption(2);
				m_pMap->SetIsCursorOn(true);
			}
			else
			{
				m_pMap->SetOption(0);
				m_pMap->SetIsCursorOn(false);
			}
			m_pMap->Update();
			m_vCursorPos = m_pMap->GetCursorPosition();
		}
		if (m_isUpdateObj)
		{
			MoveStandardPos(deltaTime);
			m_pMap->SetIsCursorOn(false);
			m_pMap->Update();
			m_vCursorPos = m_pMap->GetCursorPosition(); 
		}
	}

	if (m_pCamera) m_pCamera->Update();	
	SHADOW->SetMatrix(&m_pCamera->GetMatrix());
	if (m_pSkyBox) m_pSkyBox->Update(m_pCamera);
}

void cMainGame::Render()
{
	DEVICE->Clear(NULL,	NULL,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL ,D3DCOLOR_XRGB(120, 150, 200),1.0f, 0);

	DEVICE->BeginScene();
	
	// if (m_pGrid) m_pGrid->Render();

	if (m_isFogOn) Render_Effect_Fog();
	else
	{		
		if (m_pMap) m_pMap->Render();
		if (m_pSkyBox) m_pSkyBox->Render();
		SHADOW->Render();

		Render_Object();
	}

	if (m_isSnowOn) m_pSnow->Render("obj/Effect/Snow/Snow.tga");
	if (m_isRainOn)	m_pRain->Render("obj/Effect/Rain/Rain2.tga");
	Render_UI(m_pUISprite);

	DEVICE->EndScene();
	DEVICE->Present(NULL, NULL, NULL, NULL);
}


void cMainGame::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_pCamera) m_pCamera->WndProc(hWnd, message, wParam, lParam);
	
	switch (message)
	{

	}
}

// 각종 옵션을 조정해주는 함수
void cMainGame::SetOption()
{
	if (INPUT->GetKeyState('1')) m_pMap->SetWire(false);			// 맵을 텍스쳐로 보이도록
	if (INPUT->GetKeyState('2')) m_pMap->SetWire(true);				// 맵을 와이어로 보이도록
}

// 카메라를 위한 기준점을 움직여주는 함수
void cMainGame::MoveStandardPos(float deltaTime)
{
	// >> 카메라의 방향을 업데이트 시켜주는 부분
	m_vDir = D3DXVECTOR3(0, 0, 1);
	float angleX = (m_pCamera->GetCamRotAngle()).x;
	float angleY = (m_pCamera->GetCamRotAngle()).y;

	D3DXMATRIXA16 matRY;
	D3DXMatrixRotationY(&matRY, angleY);
	D3DXVec3TransformNormal(&m_vDir, &m_vDir, &matRY);
	D3DXVec3Normalize(&m_vDir, &m_vDir);

	D3DXVECTOR3 vDir_right;
	D3DXVec3Cross(&vDir_right, &D3DXVECTOR3(0, 1, 0), &m_vDir);
	// <<

	// 각 키에 따른 카메라 기준점 이동
	if (INPUT->GetKeyState('W')) m_vStandardPos += m_vDir * m_fCamSpeed * deltaTime;
	if (INPUT->GetKeyState('S')) m_vStandardPos -= m_vDir * m_fCamSpeed * deltaTime;
	if (INPUT->GetKeyState('A')) m_vStandardPos -= vDir_right * m_fCamSpeed * deltaTime;
	if (INPUT->GetKeyState('D')) m_vStandardPos += vDir_right * m_fCamSpeed * deltaTime;
	//<<
}

// UI 초기화
void cMainGame::Setup_UI()
{
	// >> 변수 선언
	int titleW = 60;
	int titleH = 40;
	int titleX_standard = VIEW_WIDTH - UIBORDER;
	int titleGap = 10;

	int menuspaceX = titleX_standard;
	int menuspaceY = titleH;
	int menuspaceW = 300;
	int menuspaceH = VIEW_HEIGHT - 40;

	int index = 0;
	// << 


	// >> 메뉴에 대한 탭
	m_pUITab_Menu = new cUITab();
	m_pUITab_Menu->Setup(D3DXVECTOR3(titleX_standard, 0, 0), E_UI_TAB);
	m_pUITab_Menu->Setup_tab(titleW, titleH, "Menu", "image/rect/darkgray.png", "image/rect/gray.png");

	cUITab* pUITab_Menu_space = new cUITab();
	pUITab_Menu_space->Setup(D3DXVECTOR3(-(titleW + titleGap) * index, menuspaceY, 0), E_UI_TAB);
	pUITab_Menu_space->Setup_tab(menuspaceW, menuspaceH, "", "image/rect/darkgray.png", "image/rect/gray.png");
	m_pUITab_Menu->AddChild(pUITab_Menu_space);

	m_pUIButton_create = new cUIButton();
	m_pUIButton_create->Setup(D3DXVECTOR3(100, 220, 0), E_UI_BUTTON);
	m_pUIButton_create->Setup_button(100, 50, "Create", "image/rect/white.png", "image/rect/black.png", "image/rect/yellow.png");
	pUITab_Menu_space->AddChild(m_pUIButton_create);

	cUITextView* pUIText_CellSpace = new cUITextView;
	pUIText_CellSpace->Setup(D3DXVECTOR3(30, 30, 0), E_UI_TEXT);
	pUIText_CellSpace->Setup_Text(ST_SIZE(100, 35), "Celll Space");
	pUITab_Menu_space->AddChild(pUIText_CellSpace);

	m_pUIInputField_CellSpace = new cUIInputField;
	m_pUIInputField_CellSpace->Setup(D3DXVECTOR3(170, 30, 0), E_UI_INPUTFIELD);
	m_pUIInputField_CellSpace->Setup_field(70, 35, "image/rect/darkgray.png", "image/rect/white.png");
	pUITab_Menu_space->AddChild(m_pUIInputField_CellSpace);

	cUITextView* pUIText_CellNum = new cUITextView;
	pUIText_CellNum->Setup(D3DXVECTOR3(30, 80, 0), E_UI_TEXT);
	pUIText_CellNum->Setup_Text(ST_SIZE(100, 35), "Celll Num");
	pUITab_Menu_space->AddChild(pUIText_CellNum);

	m_pUIInputField_CellNum = new  cUIInputField;
	m_pUIInputField_CellNum->Setup(D3DXVECTOR3(170, 80, 0), E_UI_INPUTFIELD);
	m_pUIInputField_CellNum->Setup_field(70, 35, "image/rect/darkgray.png", "image/rect/white.png");
	pUITab_Menu_space->AddChild(m_pUIInputField_CellNum);

	m_pUIInputField_FilePath = new	  cUIInputField;
	m_pUIInputField_FilePath->Setup(D3DXVECTOR3(20, 350, 0), E_UI_INPUTFIELD);
	m_pUIInputField_FilePath->Setup_field(250, 35, "image/rect/darkgray.png", "image/rect/white.png");
	pUITab_Menu_space->AddChild(m_pUIInputField_FilePath);

	m_pUIButton_save = new cUIButton();
	m_pUIButton_save->Setup(D3DXVECTOR3(50, 400, 0), E_UI_BUTTON);
	m_pUIButton_save->Setup_button(70, 50, "Save", "image/rect/white.png", "image/rect/black.png", "image/rect/yellow.png");
	pUITab_Menu_space->AddChild(m_pUIButton_save);

	m_pUIButton_load = new cUIButton();
	m_pUIButton_load->Setup(D3DXVECTOR3(170, 400, 0), E_UI_BUTTON);
	m_pUIButton_load->Setup_button(70, 50, "Load", "image/rect/white.png", "image/rect/black.png", "image/rect/yellow.png");
	pUITab_Menu_space->AddChild(m_pUIButton_load);

	m_pUITab_Menu->SetHiddenAll(false);
	// << 
	


	// >> 맵에 대한 탭
	index++; 

	m_pUITab_Map = new cUITab();
	m_pUITab_Map->Setup(D3DXVECTOR3(titleX_standard + (titleW + titleGap) * 1, 0, 0), E_UI_TAB);
	m_pUITab_Map->Setup_tab(titleW, titleH, "Map", "image/rect/darkgray.png", "image/rect/gray.png");

	cUITab* pUITab_Map_space = new cUITab();
	pUITab_Map_space->Setup(D3DXVECTOR3(-(titleW + titleGap) * index, menuspaceY, 0), E_UI_TAB);
	pUITab_Map_space->Setup_tab(menuspaceW, menuspaceH, "", "image/rect/darkgray.png", "image/rect/gray.png");
	m_pUITab_Map->AddChild(pUITab_Map_space);

	m_pRadioButton_Brush = new cRadioButton;
	m_pRadioButton_Brush->Setup(D3DXVECTOR3(10, 10, 0), E_UI_RADIOBUTTON);
	m_pRadioButton_Brush->Setup_RadioButton();
	pUITab_Map_space->AddChild(m_pRadioButton_Brush);
	m_pRadioButton_Brush->Add_RadioButton(D3DXVECTOR3(15, 10, 0), ST_SIZE(130, 45), E_S_OBJECTID_BLANK, E_UISTATE_IDLE, NULL);
	m_pRadioButton_Brush->Add_RadioButton(D3DXVECTOR3(145, 10, 0), ST_SIZE(130, 45), E_S_OBJECTID_BLANK2, E_UISTATE_IDLE, NULL);
	cUITextView* m_pUIText_DefaultBrush = new cUITextView();
	m_pUIText_DefaultBrush->Setup(D3DXVECTOR3(15, 10, 0), E_UI_TEXT);
	m_pUIText_DefaultBrush->Setup_Text(ST_SIZE(120, 45), "Def Brush");
	m_pRadioButton_Brush->AddChild(m_pUIText_DefaultBrush);
	cUITextView* m_pUIText_FixedBrush = new cUITextView();
	m_pUIText_FixedBrush->Setup(D3DXVECTOR3(145, 10, 0), E_UI_TEXT);
	m_pUIText_FixedBrush->Setup_Text(ST_SIZE(120, 45), "Fixed Brush");
	m_pRadioButton_Brush->AddChild(m_pUIText_FixedBrush);

	cUITextView* pUIText_InnerBrush = new cUITextView();
	pUIText_InnerBrush->Setup(D3DXVECTOR3(25, 100, 0), E_UI_TEXT);
	pUIText_InnerBrush->Setup_Text(ST_SIZE(100, 50), "Brush-Inner");
	pUITab_Map_space->AddChild(pUIText_InnerBrush);

	cUIScrollbar* pUIScroll_BrushSize_inside = new cUIScrollbar();
	pUIScroll_BrushSize_inside->Setup(D3DXVECTOR3(80, 150, 0), E_UI_SCROLL);
	pUIScroll_BrushSize_inside->Setup_scroll(150, 50, "Size", "image/rect/sky.png", 1, 10);
	pUITab_Map_space->AddChild(pUIScroll_BrushSize_inside);
	pUIScroll_BrushSize_inside->Setup_bar(15, 30);
	m_pUIScroll_BrushSize_inside = pUIScroll_BrushSize_inside;

	cUIScrollbar* pUIScroll_BrushDepth_inside = new cUIScrollbar();
	pUIScroll_BrushDepth_inside->Setup(D3DXVECTOR3(80, 190, 0), E_UI_SCROLL);
	pUIScroll_BrushDepth_inside->Setup_scroll(150, 50, "Depth", "image/rect/sky.png", -15, 15);
	pUITab_Map_space->AddChild(pUIScroll_BrushDepth_inside);
	pUIScroll_BrushDepth_inside->Setup_bar(15, 30);
	m_pUIScroll_BrushDepth_inside = pUIScroll_BrushDepth_inside;

	cUITextView* pUIText_OuterBrush = new cUITextView();
	pUIText_OuterBrush->Setup(D3DXVECTOR3(25, 250, 0), E_UI_TEXT);
	pUIText_OuterBrush->Setup_Text(ST_SIZE(100, 50), "Brush-Outer");
	pUITab_Map_space->AddChild(pUIText_OuterBrush);

	cUIScrollbar* pUIScroll_BrushSize_Outside = new cUIScrollbar();
	pUIScroll_BrushSize_Outside->Setup(D3DXVECTOR3(80, 290, 0), E_UI_SCROLL);
	pUIScroll_BrushSize_Outside->Setup_scroll(150, 50, "Size", "image/rect/sky.png", 5, 20);
	pUITab_Map_space->AddChild(pUIScroll_BrushSize_Outside);
	pUIScroll_BrushSize_Outside->Setup_bar(15, 30);
	m_pUIScroll_BrushSize_Outside = pUIScroll_BrushSize_Outside;	

	m_pUIScroll_BrushSharpness = new cUIScrollbar();
	m_pUIScroll_BrushSharpness->Setup(D3DXVECTOR3(80, 340, 0), E_UI_SCROLL);
	m_pUIScroll_BrushSharpness->Setup_scroll(150, 50, "Sharp\n-ness", "image/rect/sky.png", -1, 1);
	pUITab_Map_space->AddChild(m_pUIScroll_BrushSharpness);
	m_pUIScroll_BrushSharpness->Setup_bar(15, 30);

	m_pUIButton_GetHeight = new cUIButton();
	m_pUIButton_GetHeight->Setup(D3DXVECTOR3(30, 450, 0), E_UI_BUTTON);
	m_pUIButton_GetHeight->Setup_button(60, 40, "Get Y", "image/rect/white.png", "image/rect/black.png", "image/rect/yellow.png");
	pUITab_Map_space->AddChild(m_pUIButton_GetHeight);
	
	m_pUIInputField_SetHeight = new  cUIInputField;
	m_pUIInputField_SetHeight->Setup(D3DXVECTOR3(100, 450, 0), E_UI_INPUTFIELD);
	m_pUIInputField_SetHeight->Setup_field(90, 40, "image/rect/darkgray.png", "image/rect/white.png");
	pUITab_Map_space->AddChild(m_pUIInputField_SetHeight);

	m_pUIButton_SetHeight = new cUIButton();
	m_pUIButton_SetHeight->Setup(D3DXVECTOR3(200, 450, 0), E_UI_BUTTON);
	m_pUIButton_SetHeight->Setup_button(60, 40, "Set Y", "image/rect/white.png", "image/rect/black.png", "image/rect/yellow.png");
	pUITab_Map_space->AddChild(m_pUIButton_SetHeight);

	m_pUITab_Map->SetHiddenAll(true);
	// << 



	// >> 오브젝트에 대한 탭
	index++;
	m_pUITab_Object = new cUITab();
	m_pUITab_Object->Setup(D3DXVECTOR3(titleX_standard + (titleW + titleGap) * 2, 0, 0), E_UI_TAB);
	m_pUITab_Object->Setup_tab(titleW, titleH, "Object", "image/rect/darkgray.png", "image/rect/gray.png");

	cUITab* pUITab_Object_space = new cUITab();
	pUITab_Object_space->Setup(D3DXVECTOR3(-(titleW + titleGap) * index, menuspaceY, 0), E_UI_TAB);
	pUITab_Object_space->Setup_tab(menuspaceW, menuspaceH, "", "image/rect/darkgray.png", "image/rect/gray.png");
	m_pUITab_Object->AddChild(pUITab_Object_space);

	m_pUIButton_LLeft = new cUIButton();
	m_pUIButton_LLeft->Setup(D3DXVECTOR3(30, 20, 0), E_UI_BUTTON);
	m_pUIButton_LLeft->Setup_button(40, 40, "", "image/rect/yellow.png", "image/rect/yellow.png", "image/rect/black.png");
	pUITab_Object_space->AddChild(m_pUIButton_LLeft);

	m_pUIButton_LRight = new cUIButton();
	m_pUIButton_LRight->Setup(D3DXVECTOR3(230, 20, 0), E_UI_BUTTON);
	m_pUIButton_LRight->Setup_button(40, 40, "", "image/rect/yellow.png", "image/rect/yellow.png", "image/rect/black.png");
	pUITab_Object_space->AddChild(m_pUIButton_LRight);

	m_pUIText_LID = new cUITextView();
	m_pUIText_LID->Setup(D3DXVECTOR3(110, 20, 0), E_UI_BUTTON);
	m_pUIText_LID->Setup_Text(ST_SIZE(100, 40), "");
	pUITab_Object_space->AddChild(m_pUIText_LID);

	m_pUIButton_MLeft = new cUIButton;
	m_pUIButton_MLeft->Setup(D3DXVECTOR3(30, 70, 0), E_UI_BUTTON);
	m_pUIButton_MLeft->Setup_button(40, 40, "", "image/rect/yellow.png", "image/rect/yellow.png", "image/rect/black.png");
	pUITab_Object_space->AddChild(m_pUIButton_MLeft);

	m_pUIButton_MRight = new cUIButton;
	m_pUIButton_MRight->Setup(D3DXVECTOR3(230, 70, 0), E_UI_BUTTON);
	m_pUIButton_MRight->Setup_button(40, 40, "", "image/rect/yellow.png", "image/rect/yellow.png", "image/rect/black.png");
	pUITab_Object_space->AddChild(m_pUIButton_MRight);

	m_pUIText_MID = new cUITextView();
	m_pUIText_MID->Setup(D3DXVECTOR3(110, 70, 0), E_UI_BUTTON);
	m_pUIText_MID->Setup_Text(ST_SIZE(100, 40), "");
	pUITab_Object_space->AddChild(m_pUIText_MID);

	m_pUIButton_SLeft = new cUIButton;
	m_pUIButton_SLeft->Setup(D3DXVECTOR3(30, 700, 0), E_UI_BUTTON);
	m_pUIButton_SLeft->Setup_button(40, 40, "", "image/rect/yellow.png", "image/rect/yellow.png", "image/rect/black.png");
	pUITab_Object_space->AddChild(m_pUIButton_SLeft);

	m_pUIButton_SRight = new cUIButton;
	m_pUIButton_SRight->Setup(D3DXVECTOR3(230, 700, 0), E_UI_BUTTON);
	m_pUIButton_SRight->Setup_button(40, 40, "", "image/rect/yellow.png", "image/rect/yellow.png", "image/rect/black.png");
	pUITab_Object_space->AddChild(m_pUIButton_SRight);

	m_pUIText_SID = new cUITextView();
	m_pUIText_SID->Setup(D3DXVECTOR3(110, 700, 0), E_UI_BUTTON);
	m_pUIText_SID->Setup_Text(ST_SIZE(100, 40), "");
	pUITab_Object_space->AddChild(m_pUIText_SID);

	m_pRadioButton_Object = new cRadioButton;
	m_pRadioButton_Object->Setup(D3DXVECTOR3(10, 130, 0), E_UI_RADIOBUTTON);
	m_pRadioButton_Object->Setup_RadioButton();
	pUITab_Object_space->AddChild(m_pRadioButton_Object);

	m_pRadioButton_Object->Add_RadioButton(D3DXVECTOR3(15, 10, 0), ST_SIZE(128, 128), E_S_OBJECTID_H_DW_BARN, E_UISTATE_IDLE, NULL);
	m_pRadioButton_Object->Add_RadioButton(D3DXVECTOR3(145, 10, 0), ST_SIZE(128, 128), E_S_OBJECTID_H_DW_BLACKSMITH, E_UISTATE_IDLE, NULL);
	m_pRadioButton_Object->Add_RadioButton(D3DXVECTOR3(15, 140, 0), ST_SIZE(128, 128), E_S_OBJECTID_H_DW_CHAPEL, E_UISTATE_IDLE, NULL);
	m_pRadioButton_Object->Add_RadioButton(D3DXVECTOR3(145, 140, 0), ST_SIZE(128, 128), E_S_OBJECTID_H_DW_FARM, E_UISTATE_IDLE, NULL);
	m_pRadioButton_Object->Add_RadioButton(D3DXVECTOR3(15, 270, 0), ST_SIZE(128, 128), E_S_OBJECTID_H_DW_INN, E_UISTATE_IDLE, NULL);
	m_pRadioButton_Object->Add_RadioButton(D3DXVECTOR3(145, 270, 0), ST_SIZE(128, 128), E_S_OBJECTID_H_DW_LUMBERMILL, E_UISTATE_IDLE, NULL);
	m_pRadioButton_Object->Add_RadioButton(D3DXVECTOR3(15, 400, 0), ST_SIZE(128, 128), E_S_OBJECTID_H_DW_MAGETOWER, E_UISTATE_IDLE, NULL);
	m_pRadioButton_Object->Add_RadioButton(D3DXVECTOR3(145, 400, 0), ST_SIZE(128, 128), E_S_OBJECTID_H_DW_STABLE, E_UISTATE_IDLE, NULL);

	for (int i = 0; i < m_pRadioButton_Object->GetVecSIndex().size(); i++)
	{
		m_pRadioButton_Object->SetTexture(i, TEXTURE->GetTexture("obj/Image/Object1.png"));
	}


	m_pUITab_Object->SetHiddenAll(true);
// <<



// >> 효과에 대한 탭
	index++;
	m_pUITab_Effect = new cUITab();
	m_pUITab_Effect->Setup(D3DXVECTOR3(titleX_standard + (titleW + titleGap) * 3, 0, 0), E_UI_TAB);
	m_pUITab_Effect->Setup_tab(titleW, titleH, "Effect", "image/rect/darkgray.png", "image/rect/gray.png");

	cUITab* pUITab_Effect_space = new cUITab();
	pUITab_Effect_space->Setup(D3DXVECTOR3(-(titleW + titleGap) * index, menuspaceY, 0), E_UI_TAB);
	pUITab_Effect_space->Setup_tab(menuspaceW, menuspaceH, "", "image/rect/darkgray.png", "image/rect/gray.png");
	m_pUITab_Effect->AddChild(pUITab_Effect_space);

	// Fog
	m_pRadioButton_Fog = new cRadioButton();
	m_pRadioButton_Fog->Setup(D3DXVECTOR3(10, 50, 0), E_UI_RADIOBUTTON);
	m_pRadioButton_Fog->Setup_RadioButton();
	pUITab_Effect_space->AddChild(m_pRadioButton_Fog);

	m_pRadioButton_Fog->Add_RadioButton(D3DXVECTOR3(75, 10, 0), ST_SIZE(130, 60), E_S_OBJECTID_BLANK, E_UISTATE_IDLE, NULL);

	cUITextView* m_UIText_Fog = new cUITextView();
	m_UIText_Fog->Setup(D3DXVECTOR3(70, -5, 0), E_UI_TEXT);
	m_UIText_Fog->Setup_Text(ST_SIZE(130, 80), "Fog");
	m_pRadioButton_Fog->AddChild(m_UIText_Fog);

	m_pFog = new cFog();
	m_pFog->Setup("obj/Effect/Fog/fog.txt");

	m_pUIButton_Fog_Minus = new cUIButton();
	m_pUIButton_Fog_Minus->Setup(D3DXVECTOR3(20, 70, 0), E_UI_BUTTON);
	m_pUIButton_Fog_Minus->Setup_button(50, 40, "", "image/rect/sky.png", "image/rect/sky.png", "image/rect/black.png");
	pUITab_Effect_space->AddChild(m_pUIButton_Fog_Minus);

	m_pUIButton_Fog_Plus = new cUIButton();
	m_pUIButton_Fog_Plus->Setup(D3DXVECTOR3(210, 70, 0), E_UI_BUTTON);
	m_pUIButton_Fog_Plus->Setup_button(60, 40, "", "image/rect/sky.png", "image/rect/sky.png", "image/rect/black.png");
	pUITab_Effect_space->AddChild(m_pUIButton_Fog_Plus);

	m_pUIText_Fog_Minus = new cUITextView();
	m_pUIText_Fog_Minus->Setup(D3DXVECTOR3(20, 70, 0), E_UI_BUTTON);
	m_pUIText_Fog_Minus->Setup_Text(ST_SIZE(50, 40), "-");
	pUITab_Effect_space->AddChild(m_pUIText_Fog_Minus);

	m_pUIText_Fog_Plus = new cUITextView();
	m_pUIText_Fog_Plus->Setup(D3DXVECTOR3(210, 70, 0), E_UI_BUTTON);
	m_pUIText_Fog_Plus->Setup_Text(ST_SIZE(60, 40), "+");
	pUITab_Effect_space->AddChild(m_pUIText_Fog_Plus);

	// Shadow
	m_pRadioButton_Shadow = new cRadioButton();
	m_pRadioButton_Shadow->Setup(D3DXVECTOR3(10, 70, 0), E_UI_RADIOBUTTON);
	m_pRadioButton_Shadow->Setup_RadioButton();
	pUITab_Effect_space->AddChild(m_pRadioButton_Shadow);

	m_pRadioButton_Shadow->Add_RadioButton(D3DXVECTOR3(75, 70, 0), ST_SIZE(130, 60), E_S_OBJECTID_BLANK, E_UISTATE_IDLE, NULL);

	cUITextView* m_UIText_Shadow = new cUITextView();
	m_UIText_Shadow->Setup(D3DXVECTOR3(70, 55, 0), E_UI_TEXT);
	m_UIText_Shadow->Setup_Text(ST_SIZE(130, 80), "Shadow");
	m_pRadioButton_Shadow->AddChild(m_UIText_Shadow);

	m_pUIButton_Shadow_Minus = new cUIButton();
	m_pUIButton_Shadow_Minus->Setup(D3DXVECTOR3(20, 150, 0), E_UI_BUTTON);
	m_pUIButton_Shadow_Minus->Setup_button(50, 40, "", "image/rect/sky.png", "image/rect/sky.png", "image/rect/black.png");
	pUITab_Effect_space->AddChild(m_pUIButton_Shadow_Minus);

	m_pUIButton_Shadow_Plus = new cUIButton();
	m_pUIButton_Shadow_Plus->Setup(D3DXVECTOR3(210, 150, 0), E_UI_BUTTON);
	m_pUIButton_Shadow_Plus->Setup_button(60, 40, "", "image/rect/sky.png", "image/rect/sky.png", "image/rect/black.png");
	pUITab_Effect_space->AddChild(m_pUIButton_Shadow_Plus);

	m_pUIText_Shadow_Minus = new cUITextView();
	m_pUIText_Shadow_Minus->Setup(D3DXVECTOR3(20, 150, 0), E_UI_BUTTON);
	m_pUIText_Shadow_Minus->Setup_Text(ST_SIZE(50, 40), "-");
	pUITab_Effect_space->AddChild(m_pUIText_Shadow_Minus);

	m_pUIText_Shadow_Plus = new cUITextView();
	m_pUIText_Shadow_Plus->Setup(D3DXVECTOR3(210, 150, 0), E_UI_BUTTON);
	m_pUIText_Shadow_Plus->Setup_Text(ST_SIZE(60, 40), "+");
	pUITab_Effect_space->AddChild(m_pUIText_Shadow_Plus);

	// Snow
	m_pRadioButton_Snow = new cRadioButton();
	m_pRadioButton_Snow->Setup(D3DXVECTOR3(10, 90, 0), E_UI_RADIOBUTTON);
	m_pRadioButton_Snow->Setup_RadioButton();
	pUITab_Effect_space->AddChild(m_pRadioButton_Snow);

	m_pRadioButton_Snow->Add_RadioButton(D3DXVECTOR3(75, 130, 0), ST_SIZE(130, 60), E_S_OBJECTID_BLANK, E_UISTATE_IDLE, NULL);

	cUITextView* m_UIText_Snow = new cUITextView();
	m_UIText_Snow->Setup(D3DXVECTOR3(70, 115, 0), E_UI_TEXT);
	m_UIText_Snow->Setup_Text(ST_SIZE(130, 80), "Snow");
	m_pRadioButton_Snow->AddChild(m_UIText_Snow);

	m_pSnow = new cWeather();
	m_pSnow->Setup(m_nSize, m_nSize, m_nSize, 1500);

	m_pUIButton_Snow_Minus = new cUIButton();
	m_pUIButton_Snow_Minus->Setup(D3DXVECTOR3(20, 230, 0), E_UI_BUTTON);
	m_pUIButton_Snow_Minus->Setup_button(50, 40, "", "image/rect/sky.png", "image/rect/sky.png", "image/rect/black.png");
	pUITab_Effect_space->AddChild(m_pUIButton_Snow_Minus);

	m_pUIButton_Snow_Plus = new cUIButton();
	m_pUIButton_Snow_Plus->Setup(D3DXVECTOR3(210, 230, 0), E_UI_BUTTON);
	m_pUIButton_Snow_Plus->Setup_button(60, 40, "", "image/rect/sky.png", "image/rect/sky.png", "image/rect/black.png");
	pUITab_Effect_space->AddChild(m_pUIButton_Snow_Plus);

	m_pUIText_Snow_Minus = new cUITextView();
	m_pUIText_Snow_Minus->Setup(D3DXVECTOR3(20, 230, 0), E_UI_BUTTON);
	m_pUIText_Snow_Minus->Setup_Text(ST_SIZE(50, 40), "-");
	pUITab_Effect_space->AddChild(m_pUIText_Snow_Minus);

	m_pUIText_Snow_Plus = new cUITextView();
	m_pUIText_Snow_Plus->Setup(D3DXVECTOR3(210, 230, 0), E_UI_BUTTON);
	m_pUIText_Snow_Plus->Setup_Text(ST_SIZE(60, 40), "+");
	pUITab_Effect_space->AddChild(m_pUIText_Snow_Plus);

	// Rain
	m_pRadioButton_Rain = new cRadioButton();
	m_pRadioButton_Rain->Setup(D3DXVECTOR3(10, 110, 0), E_UI_RADIOBUTTON);
	m_pRadioButton_Rain->Setup_RadioButton();
	pUITab_Effect_space->AddChild(m_pRadioButton_Rain);

	m_pRadioButton_Rain->Add_RadioButton(D3DXVECTOR3(75, 190, 0), ST_SIZE(130, 60), E_S_OBJECTID_BLANK, E_UISTATE_IDLE, NULL);

	cUITextView* m_UIText_Rain = new cUITextView();
	m_UIText_Rain->Setup(D3DXVECTOR3(70, 175, 0), E_UI_TEXT);
	m_UIText_Rain->Setup_Text(ST_SIZE(130, 80), "Rain");
	m_pRadioButton_Rain->AddChild(m_UIText_Rain);

	m_pRain = new cWeather();
	m_pRain->Setup(m_nSize, m_nSize, m_nSize, 3000);

	m_pUIButton_Rain_Minus = new cUIButton();
	m_pUIButton_Rain_Minus->Setup(D3DXVECTOR3(20, 310, 0), E_UI_BUTTON);
	m_pUIButton_Rain_Minus->Setup_button(50, 40, "", "image/rect/sky.png", "image/rect/sky.png", "image/rect/black.png");
	pUITab_Effect_space->AddChild(m_pUIButton_Rain_Minus);

	m_pUIButton_Rain_Plus = new cUIButton();
	m_pUIButton_Rain_Plus->Setup(D3DXVECTOR3(210, 310, 0), E_UI_BUTTON);
	m_pUIButton_Rain_Plus->Setup_button(60, 40, "", "image/rect/sky.png", "image/rect/sky.png", "image/rect/black.png");
	pUITab_Effect_space->AddChild(m_pUIButton_Rain_Plus);

	m_pUIText_Rain_Minus = new cUITextView();
	m_pUIText_Rain_Minus->Setup(D3DXVECTOR3(20, 310, 0), E_UI_BUTTON);
	m_pUIText_Rain_Minus->Setup_Text(ST_SIZE(50, 40), "-");
	pUITab_Effect_space->AddChild(m_pUIText_Rain_Minus);

	m_pUIText_Rain_Plus = new cUITextView();
	m_pUIText_Rain_Plus->Setup(D3DXVECTOR3(210, 310, 0), E_UI_BUTTON);
	m_pUIText_Rain_Plus->Setup_Text(ST_SIZE(60, 40), "+");
	pUITab_Effect_space->AddChild(m_pUIText_Rain_Plus);

	m_pUITab_Effect->SetHiddenAll(true);
	// << 
}

void cMainGame::Update_UI()
{
	// >> UI 건드릴 때 맵 업데이트 안하도록 하는 bool값 변경
	if (!m_pUITab_Map->GetHidden())
	{
		if (INPUT->GetMousePos().x > VIEW_WIDTH - UIBORDER) m_isUpdateMap = false;
		else m_isUpdateMap = true;
	}
	// << 

	// >> UI 건드릴 때 맵 업데이트 안하도록 하는 bool값 변경
	if (!m_pUITab_Object->GetHidden())
	{
		if (INPUT->GetMousePos().x > VIEW_WIDTH - UIBORDER) m_isUpdateObj = false;
		else m_isUpdateObj = true;
	}
	// << 

	// >> 탭 켜고 끄는 부분
	if (INPUT->IsMouseDown(MOUSE_LEFT))
	{
		if (INPUT->IsCollided(m_pUITab_Menu->LeftTop(), m_pUITab_Menu->RightBottom()))
		{
			m_pUITab_Menu->SetHiddenAll(false);
			m_pUITab_Map->SetHiddenAll(true);
			m_pUITab_Object->SetHiddenAll(true);
			m_pUITab_Effect->SetHiddenAll(true);
			m_isUpdateMap = false;
			m_isUpdateObj = false;
			m_pMap->SetIsCursorOn(false);
		}
		else if (INPUT->IsCollided(m_pUITab_Map->LeftTop(), m_pUITab_Map->RightBottom()))
		{
			m_pUITab_Menu->SetHiddenAll(true);
			m_pUITab_Map->SetHiddenAll(false);
			m_pUITab_Object->SetHiddenAll(true);
			m_pUITab_Effect->SetHiddenAll(true);
			m_isUpdateMap = true;
			m_isUpdateObj = false;
			m_pMap->SetIsCursorOn(true);
		}
		else if (INPUT->IsCollided(m_pUITab_Object->LeftTop(), m_pUITab_Object->RightBottom()))
		{
			m_pUITab_Menu->SetHiddenAll(true);
			m_pUITab_Map->SetHiddenAll(true);
			m_pUITab_Object->SetHiddenAll(false);
			m_pUITab_Effect->SetHiddenAll(true);
			m_isUpdateMap = false;
			m_isUpdateObj = true;
			m_pMap->SetIsCursorOn(false);
		}
		else if (INPUT->IsCollided(m_pUITab_Effect->LeftTop(), m_pUITab_Effect->RightBottom()))
		{
			m_pUITab_Menu->SetHiddenAll(true);
			m_pUITab_Map->SetHiddenAll(true);
			m_pUITab_Object->SetHiddenAll(true);
			m_pUITab_Effect->SetHiddenAll(false);
			m_isUpdateMap = false;
			m_isUpdateObj = false;
			m_pMap->SetIsCursorOn(false);
		}
	}
	// << 


	// >> 부모인 UITab들 업데이트, 하위 목록도 전부 업뎃 됨
	if (m_pUITab_Menu) m_pUITab_Menu->Update(0);
	if (m_pUITab_Map) m_pUITab_Map->Update(0);
	if (m_pUITab_Object) m_pUITab_Object->Update(0);
	if (m_pUITab_Effect) m_pUITab_Effect->Update(0);
	// << 

	Update_MapBrush();

	Update_Menu();
	Update_Object();
	Update_Effect();
}

void cMainGame::Render_UI(LPD3DXSPRITE pSprite)
{
	if (m_pUITab_Menu) m_pUITab_Menu->Render(pSprite);
	if (m_pUITab_Map) m_pUITab_Map->Render(pSprite);
	if (m_pUITab_Object) m_pUITab_Object->Render(pSprite);
	if (m_pUITab_Effect) m_pUITab_Effect->Render(pSprite);
}

void cMainGame::Update_Object()
{
	// 대분류 버튼 처리-> 중분류 인덱스의 클램프를 여기서 처리
	Update_L_Object();

	// 중분류 버튼 처리-> 소분류 인덱스의 클램프를 여기서 처리
	Update_M_Object();

	// 소분류 버튼 처리
	Update_S_Object();

	if (m_pRadioButton_Object->GetSID() != -1)
	{
		if (m_pRadioButton_Object->GetIsClicked())
		{
			if (m_pConstruct != NULL) m_pConstruct->Destroy();

			m_pConstruct = new cConstruct;

			char* folder = OBJECTDB->GetMapObject(m_pRadioButton_Object->GetSID())->szFolder;
			char* file = OBJECTDB->GetMapObject(m_pRadioButton_Object->GetSID())->szFile;
			m_pConstruct->SetSObjID(m_pRadioButton_Object->GetSID());

			if (m_pConstruct->GetSObjID() >= E_S_OBJECTID_P_DW_START && m_pConstruct->GetSObjID() <= E_S_OBJECTID_P_ETC_END
				 || m_pConstruct->GetSObjID() >= E_S_OBJECTID_N_H_START && m_pConstruct->GetSObjID() <= E_S_OBJECTID_N_O_END)
			{
				m_pConstruct->Setup(folder, file, false);
			}
			else m_pConstruct->Setup(folder, file, true);

		}

		if (INPUT->GetKeyState('Z'))	m_pConstruct->SetScale(m_pConstruct->GetScale() + D3DXVECTOR3(0.02f, 0.02f, 0.02f));
		if (INPUT->GetKeyState('X'))	m_pConstruct->SetScale(m_pConstruct->GetScale() - D3DXVECTOR3(0.02f, 0.02f, 0.02f));

		if (INPUT->GetKeyState('Q'))	m_pConstruct->SetRotationY(m_pConstruct->GetRotationY() + 0.02f);
		if (INPUT->GetKeyState('E'))	m_pConstruct->SetRotationY(m_pConstruct->GetRotationY() - 0.02f);

		m_pConstruct->SetPosition(D3DXVECTOR3(m_vCursorPos.x, m_vCursorPos.y, m_vCursorPos.z));

		m_pConstruct->Update();

		if (INPUT->IsMouseDown(MOUSE_LEFT) && m_isUpdateObj)
		{
			cConstruct* pConstruct = new cConstruct;
			pConstruct->SetScale(m_pConstruct->GetScale());
			pConstruct->SetRotationX(m_pConstruct->GetRotationX());
			pConstruct->SetRotationY(m_pConstruct->GetRotationY());
			pConstruct->SetRotationZ(m_pConstruct->GetRotationZ());
			pConstruct->SetPosition(m_pConstruct->GetPosition());

			pConstruct->Create(m_pRadioButton_Object->GetSID());

			m_vecConstruct.push_back(pConstruct);

			E_L_OBJECTID largeID = OBJECTDB->GetMapObject(m_pRadioButton_Object->GetSID())->eLargeID;
		}
	}


	if (m_pRadioButton_Object->GetSID() == -1 && m_pRadioButton_Object->GetIsClicked() == false)
	{
		if (m_pConstruct)	SAFE_DELETE(m_pConstruct);
	}

	SHADOW->Update(m_vecConstruct);
}

void cMainGame::Update_L_Object()
{
	// 클릭되었을 때 MIndex값과 SIndex값을 초기화해줘야 페이지가 여러번 넘어가는 현상 방지 -> Middle 함수에서도 같은 처리
	// 단, 오른쪽 클릭일 때는 index값이 커지는 것이기 때문에 초기화를 해주지 않아도 첫번째 값으로 들어가게 됨!
	// 초기화는 각각 LButton일 때와 MButton일 때 초기화해줘야 하는 인덱스가 다르기 때문에 각자 함수에서 처리
	if (m_pUIButton_LLeft->GetCurrentState() == E_UISTATE_CLICKED)
	{
		m_nPage = 1;
		m_nObject_LIndex--;

		switch (m_nObject_LIndex)
		{
		case E_L_OBJECTID_HUMAN:
			m_nObject_MIndex = E_M_OBJECTID_H_DUSKWOOD;
			m_nObject_SIndex = E_S_OBJECTID_H_DW_START;
			break;

		case E_L_OBJECTID_ORC:
			m_nObject_MIndex = E_M_OBJECTID_O_KALIMDOR;
			m_nObject_SIndex = E_S_OBJECTID_O_KD_START;
			break;

		case E_L_OBJECTID_PROPS:
			m_nObject_MIndex = E_M_OBJECTID_P_DUSKWOOD;
			m_nObject_SIndex = E_S_OBJECTID_P_DW_START;

		case E_L_OBJECTID_VILLAGE:
			m_nObject_MIndex = E_M_OBJECTID_V_VILLAGE;
			m_nObject_SIndex = E_S_OBJECTID_V_STORMWIND;
			break;
		}

	}
	if (m_pUIButton_LRight->GetCurrentState() == E_UISTATE_CLICKED)
	{
		m_nPage = 1;
		m_nObject_LIndex++;
	}

	m_nObject_LIndex = (m_nObject_LIndex <= E_L_OBJECTID_START) ? (E_L_OBJECTID_START + 1) : m_nObject_LIndex;
	m_nObject_LIndex = (m_nObject_LIndex >= E_L_OBJECTID_END) ? (E_L_OBJECTID_END - 1) : m_nObject_LIndex;


	switch (m_nObject_LIndex)
	{
	case E_L_OBJECTID_HUMAN:
		m_pUIText_LID->SetText("HUMAN");

		m_nObject_MIndex = (m_nObject_MIndex <= E_M_OBJECTID_H_START) ? (E_M_OBJECTID_H_START + 1) : m_nObject_MIndex;
		m_nObject_MIndex = (m_nObject_MIndex >= E_M_OBJECTID_H_END) ? (E_M_OBJECTID_H_END - 1) : m_nObject_MIndex;
		break;

	case E_L_OBJECTID_ORC:
		m_pUIText_LID->SetText("ORC");

		m_nObject_MIndex = (m_nObject_MIndex <= E_M_OBJECTID_O_START) ? (E_M_OBJECTID_O_START + 1) : m_nObject_MIndex;
		m_nObject_MIndex = (m_nObject_MIndex >= E_M_OBJECTID_O_END) ? (E_M_OBJECTID_O_END - 1) : m_nObject_MIndex;
		break;

	case E_L_OBJECTID_PROPS:
		m_pUIText_LID->SetText("PROPS");

		m_nObject_MIndex = (m_nObject_MIndex <= E_M_OBJECTID_P_START) ? (E_M_OBJECTID_P_START + 1) : m_nObject_MIndex;
		m_nObject_MIndex = (m_nObject_MIndex >= E_M_OBJECTID_P_END) ? (E_M_OBJECTID_P_END - 1) : m_nObject_MIndex;
		break;

	case E_L_OBJECTID_VILLAGE:
		m_pUIText_LID->SetText("VILLAGE");

		m_nObject_MIndex = (m_nObject_MIndex <= E_M_OBJECTID_V_START) ? (E_M_OBJECTID_V_START + 1) : m_nObject_MIndex;
		m_nObject_MIndex = (m_nObject_MIndex >= E_M_OBJECTID_V_END) ? (E_M_OBJECTID_V_END - 1) : m_nObject_MIndex;
		break;

	case E_L_OBJECTID_NPC:
		m_pUIText_LID->SetText("NPC");

		m_nObject_MIndex = (m_nObject_MIndex <= E_M_OBJECTID_N_START) ? (E_M_OBJECTID_N_START + 1) : m_nObject_MIndex;
		m_nObject_MIndex = (m_nObject_MIndex >= E_M_OBJECTID_N_END) ? (E_M_OBJECTID_N_END - 1) : m_nObject_MIndex;
		break;
	}
}

void cMainGame::Update_M_Object()
{
	if (m_pUIButton_MLeft->GetCurrentState() == E_UISTATE_CLICKED)
	{
		m_nObject_MIndex--;
		m_nPage = 1;

		switch (m_nObject_LIndex)
		{
		case E_L_OBJECTID_HUMAN:
			m_nObject_SIndex = E_S_OBJECTID_H_DW_START;
			break;

		case E_L_OBJECTID_ORC:
			m_nObject_SIndex = E_S_OBJECTID_O_KD_START;
			break;

		case E_L_OBJECTID_PROPS:
			if (m_nObject_MIndex == E_M_OBJECTID_P_DUSKWOOD)	m_nObject_SIndex = E_S_OBJECTID_P_DW_START;
			if (m_nObject_MIndex == E_M_OBJECTID_P_NORTHREND)	m_nObject_SIndex = E_S_OBJECTID_P_NR_START;
			break;

		case E_L_OBJECTID_VILLAGE:
			m_nObject_SIndex = E_S_OBJECTID_V_START;
			break;

		case E_L_OBJECTID_NPC:
			m_nObject_SIndex = E_S_OBJECTID_N_H_START;
			break;
		}
	}

	if (m_pUIButton_MRight->GetCurrentState() == E_UISTATE_CLICKED)
	{
		m_nObject_MIndex++;
		m_nPage = 1;
	}


	int nStartIndex = 1;

	switch (m_nObject_MIndex)
	{
	case E_M_OBJECTID_H_DUSKWOOD:
		m_pUIText_MID->SetText("DUSKWOOD");
		m_nObject_SIndex = (m_nObject_SIndex <= E_S_OBJECTID_H_DW_START) ? (E_S_OBJECTID_H_DW_START + 1) : m_nObject_SIndex;

		nStartIndex = E_S_OBJECTID_H_DW_START + 1;
		break;

	case E_M_OBJECTID_H_DRAENOR:
		m_pUIText_MID->SetText("DRAENOR");
		m_nObject_SIndex = (m_nObject_SIndex <= E_S_OBJECTID_H_DN_START) ? (E_S_OBJECTID_H_DN_START + 1) : m_nObject_SIndex;

		nStartIndex = E_S_OBJECTID_H_DN_START + 1;
		break;

	case E_M_OBJECTID_O_KALIMDOR:
		m_pUIText_MID->SetText("KALIMDOR");
		m_nObject_SIndex = (m_nObject_SIndex <= E_S_OBJECTID_O_KD_START) ? (E_S_OBJECTID_O_KD_START + 1) : m_nObject_SIndex;

		nStartIndex = E_S_OBJECTID_O_KD_START + 1;
		break;

	case E_M_OBJECTID_O_NORTHREND:
		m_pUIText_MID->SetText("NORTHREND");
		m_nObject_SIndex = (m_nObject_SIndex <= E_S_OBJECTID_O_NR_START) ? (E_S_OBJECTID_O_NR_START + 1) : m_nObject_SIndex;

		nStartIndex = E_S_OBJECTID_O_NR_START + 1;
		break;

	case E_M_OBJECTID_P_DUSKWOOD:
		m_pUIText_MID->SetText("DUSKWOOD");
		m_nObject_SIndex = (m_nObject_SIndex <= E_S_OBJECTID_P_DW_START) ? (E_S_OBJECTID_P_DW_START + 1) : m_nObject_SIndex;

		nStartIndex = E_S_OBJECTID_P_DW_START + 1;
		break;

	case E_M_OBJECTID_P_NORTHREND:
		m_pUIText_MID->SetText("NORTHREND");
		m_nObject_SIndex = (m_nObject_SIndex <= E_S_OBJECTID_P_NR_START) ? (E_S_OBJECTID_P_NR_START + 1) : m_nObject_SIndex;

		nStartIndex = E_S_OBJECTID_P_NR_START + 1;
		break;

	case E_M_OBJECTID_P_ETC:
		m_pUIText_MID->SetText("ETC");
		m_nObject_SIndex = (m_nObject_SIndex <= E_S_OBJECTID_P_ETC_START) ? (E_S_OBJECTID_P_ETC_START + 1) : m_nObject_SIndex;

		nStartIndex = E_S_OBJECTID_P_ETC_START + 1;
		break;

	case E_M_OBJECTID_V_VILLAGE:
		m_pUIText_MID->SetText("VILLAGE");
		m_nObject_SIndex = (m_nObject_SIndex <= E_S_OBJECTID_V_START) ? (E_S_OBJECTID_V_START + 1) : m_nObject_SIndex;

		nStartIndex = E_S_OBJECTID_V_START + 1;
		break;

	case E_M_OBJECTID_N_HUMAN:
		m_pUIText_MID->SetText("HUMAN");
		m_nObject_SIndex = (m_nObject_SIndex <= E_S_OBJECTID_N_H_START) ? (E_S_OBJECTID_N_H_START + 1) : m_nObject_SIndex;

		nStartIndex = E_S_OBJECTID_N_H_START + 1;
		break;

	case E_M_OBJECTID_N_ORC:
		m_pUIText_MID->SetText("ORC");
		m_nObject_SIndex = (m_nObject_SIndex <= E_S_OBJECTID_N_O_START) ? (E_S_OBJECTID_N_O_START + 1) : m_nObject_SIndex;

		nStartIndex = E_S_OBJECTID_N_O_START + 1;
		break;
	}

	if (m_nPage > 0) m_nObject_SIndex = 8 * (m_nPage - 1) + nStartIndex;
}

void cMainGame::Update_S_Object()
{
	if (m_pUIButton_SLeft->GetCurrentState() == E_UISTATE_CLICKED)
	{
		if (m_nPage < 1) m_nPage = 1;
		else m_nPage--;
	}

	switch (m_nObject_MIndex)
	{
		// HUMAN
	case E_M_OBJECTID_H_DUSKWOOD:
	{
		for (int i = 0; i < m_pRadioButton_Object->GetVecSIndex().size(); i++)
		{
			if (m_nObject_SIndex + i >= E_S_OBJECTID_H_DW_END)
			{
				m_pRadioButton_Object->SetSID(i, E_S_OBJECTID_BLANK);
			}
			else m_pRadioButton_Object->SetSID(i, m_nObject_SIndex + i);
		}

		if (m_pUIButton_SRight->GetCurrentState() == E_UISTATE_CLICKED)
		{
			int count = E_S_OBJECTID_H_DW_END - E_S_OBJECTID_H_DW_START - 1;
			int maxPage = (count / 8) + 1;

			if (maxPage > m_nPage) m_nPage++;
		}
		break;
	}

	case E_M_OBJECTID_H_DRAENOR:
	{
		for (int i = 0; i < m_pRadioButton_Object->GetVecSIndex().size(); i++)
		{
			if (m_nObject_SIndex + i >= E_S_OBJECTID_H_DN_END) 	m_pRadioButton_Object->SetSID(i, E_S_OBJECTID_BLANK);
			else m_pRadioButton_Object->SetSID(i, m_nObject_SIndex + i);
		}

		if (m_pUIButton_SRight->GetCurrentState() == E_UISTATE_CLICKED)
		{
			int count = E_S_OBJECTID_H_DN_END - E_S_OBJECTID_H_DN_START - 1;
			int maxPage = (count / 8) + 1;

			if (maxPage > m_nPage) m_nPage++;
		}
		break;
	}

	// ORC
	case E_M_OBJECTID_O_KALIMDOR:
	{
		for (int i = 0; i < m_pRadioButton_Object->GetVecSIndex().size(); i++)
		{
			if (m_nObject_SIndex + i >= E_S_OBJECTID_O_KD_END)	m_pRadioButton_Object->SetSID(i, E_S_OBJECTID_BLANK);
			else m_pRadioButton_Object->SetSID(i, m_nObject_SIndex + i);
		}

		if (m_pUIButton_SRight->GetCurrentState() == E_UISTATE_CLICKED)
		{
			int count = E_S_OBJECTID_O_KD_END - E_S_OBJECTID_O_KD_START - 1;
			int maxPage = (count / 8) + 1;

			if (maxPage > m_nPage) m_nPage++;
		}
		break;
	}

	case E_M_OBJECTID_O_NORTHREND:
	{
		for (int i = 0; i < m_pRadioButton_Object->GetVecSIndex().size(); i++)
		{
			if (m_nObject_SIndex + i >= E_S_OBJECTID_O_NR_END)	m_pRadioButton_Object->SetSID(i, E_S_OBJECTID_BLANK);
			else m_pRadioButton_Object->SetSID(i, m_nObject_SIndex + i);
		}

		if (m_pUIButton_SRight->GetCurrentState() == E_UISTATE_CLICKED)
		{
			int count = E_S_OBJECTID_O_NR_END - E_S_OBJECTID_O_NR_START - 1;
			int maxPage = (count / 8) + 1;

			if (maxPage > m_nPage) m_nPage++;
		}
		break;
	}

	// PROPS
	case E_M_OBJECTID_P_DUSKWOOD:
	{
		for (int i = 0; i < m_pRadioButton_Object->GetVecSIndex().size(); i++)
		{
			if (m_nObject_SIndex + i >= E_S_OBJECTID_P_DW_END)	m_pRadioButton_Object->SetSID(i, E_S_OBJECTID_BLANK);
			else m_pRadioButton_Object->SetSID(i, m_nObject_SIndex + i);
		}

		if (m_pUIButton_SRight->GetCurrentState() == E_UISTATE_CLICKED)
		{
			int count = E_S_OBJECTID_P_DW_END - E_S_OBJECTID_P_DW_START - 1;
			int maxPage = (count / 8) + 1;

			if (maxPage > m_nPage) m_nPage++;
		}
		break;
	}

	case E_M_OBJECTID_P_NORTHREND:
	{
		for (int i = 0; i < m_pRadioButton_Object->GetVecSIndex().size(); i++)
		{
			if (m_nObject_SIndex + i >= E_S_OBJECTID_P_NR_END)	m_pRadioButton_Object->SetSID(i, E_S_OBJECTID_BLANK);
			else m_pRadioButton_Object->SetSID(i, m_nObject_SIndex + i);
		}

		if (m_pUIButton_SRight->GetCurrentState() == E_UISTATE_CLICKED)
		{
			int count = E_S_OBJECTID_P_NR_END - E_S_OBJECTID_P_NR_START - 1;
			int maxPage = (count / 8) + 1;

			if (maxPage > m_nPage) m_nPage++;
		}
		break;
	}

	case E_M_OBJECTID_P_ETC:
	{
		for (int i = 0; i < m_pRadioButton_Object->GetVecSIndex().size(); i++)
		{
			if (m_nObject_SIndex + i >= E_S_OBJECTID_P_ETC_END)	m_pRadioButton_Object->SetSID(i, E_S_OBJECTID_BLANK);
			else m_pRadioButton_Object->SetSID(i, m_nObject_SIndex + i);
		}

		if (m_pUIButton_SRight->GetCurrentState() == E_UISTATE_CLICKED)
		{
			int count = E_S_OBJECTID_P_ETC_END - E_S_OBJECTID_P_ETC_START - 1;
			int maxPage = (count / 8) + 1;

			if (maxPage > m_nPage) m_nPage++;
		}
		break;
	}

	// VILLAGE
	case E_M_OBJECTID_V_VILLAGE:
	{
		for (int i = 0; i < m_pRadioButton_Object->GetVecSIndex().size(); i++)
		{
			if (m_nObject_SIndex + i >= E_S_OBJECTID_V_END)	m_pRadioButton_Object->SetSID(i, E_S_OBJECTID_BLANK);
			else m_pRadioButton_Object->SetSID(i, m_nObject_SIndex + i);
		}

		if (m_pUIButton_SRight->GetCurrentState() == E_UISTATE_CLICKED)
		{
			int count = E_S_OBJECTID_V_END - E_S_OBJECTID_V_START - 1;
			int maxPage = (count / 8) + 1;

			if (maxPage > m_nPage) m_nPage++;
		}
		break;
	}

	case E_M_OBJECTID_N_HUMAN:
	{
		for (int i = 0; i < m_pRadioButton_Object->GetVecSIndex().size(); i++)
		{
			if (m_nObject_SIndex + i >= E_S_OBJECTID_N_H_END)	m_pRadioButton_Object->SetSID(i, E_S_OBJECTID_BLANK);
			else m_pRadioButton_Object->SetSID(i, m_nObject_SIndex + i);
		}

		if (m_pUIButton_SRight->GetCurrentState() == E_UISTATE_CLICKED)
		{
			int count = E_S_OBJECTID_N_H_END - E_S_OBJECTID_N_H_START - 1;
			int maxPage = (count / 8) + 1;

			if (maxPage > m_nPage) m_nPage++;
		}
		break;
	}


	case E_M_OBJECTID_N_ORC:
	{
		for (int i = 0; i < m_pRadioButton_Object->GetVecSIndex().size(); i++)
		{
			if (m_nObject_SIndex + i >= E_S_OBJECTID_N_O_END)	m_pRadioButton_Object->SetSID(i, E_S_OBJECTID_BLANK);
			else m_pRadioButton_Object->SetSID(i, m_nObject_SIndex + i);
		}

		if (m_pUIButton_SRight->GetCurrentState() == E_UISTATE_CLICKED)
		{
			int count = E_S_OBJECTID_N_O_END - E_S_OBJECTID_N_O_START - 1;
			int maxPage = (count / 8) + 1;

			if (maxPage > m_nPage) m_nPage++;
		}
		break;
	}
	}
}
// 오브젝트 랜더
void cMainGame::Render_Object()
{
	if (m_pConstruct != NULL) m_pConstruct->Render();
	for (int i = 0; i < m_vecConstruct.size(); i++)
	{
		m_vecConstruct[i]->Render();
	}
}

// 오브젝트 지우기
void cMainGame::Delete_Object()
{
	if (m_pRadioButton_Object->GetSID() == -1)
	{
		D3DXVECTOR3 vDummy;
		for (int i = 0; i < m_vecConstruct.size(); i++)
		{
			if (cRay::IsCollidedWithMesh(D3DXVECTOR2(INPUT->GetMousePos().x, INPUT->GetMousePos().y),
				m_vecConstruct[i]->GetObjMesh(), vDummy))
			{
				if (m_pSphere) SAFE_RELEASE(m_pSphere);
				m_vSpherePos = m_vecConstruct[i]->GetPosition();
				D3DXCreateSphere(DEVICE, 20, 10, 10, &m_pSphere, NULL);
			}
		}
	}
}

void cMainGame::Render_Sphere()
{
	if (!m_pSphere) return;

	D3DXMATRIXA16 matWorld, matR, matT;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matR);
	D3DXMatrixIdentity(&matT);

	D3DXMatrixTranslation(&matT, m_vSpherePos.x, m_vSpherePos.y, m_vSpherePos.z);
	matWorld = matR*matT;

	DEVICE->SetTransform(D3DTS_WORLD, &matWorld);
	DEVICE->SetMaterial(&m_mtlSPhere);
	DEVICE->SetTexture(0, NULL);

	DEVICE->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pSphere->DrawSubset(0);
	DEVICE->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

// 맵 브러쉬 업데이트 
void cMainGame::Update_MapBrush()
{
	m_pMap->SetBrushInside(m_pUIScroll_BrushSize_inside->GetValue());
	m_pMap->SetBrushOutside(m_pUIScroll_BrushSize_Outside->GetValue());
	m_pMap->SetBrushDepthInside(m_pUIScroll_BrushDepth_inside->GetValue());
	m_pMap->SetBrushSharpness(m_pUIScroll_BrushSharpness->GetValue());

	if (m_pUIButton_GetHeight->GetCurrentState() == E_UISTATE_CLICKED) m_getHeight = true;
	if (m_getHeight)
	{
		if (INPUT->IsMouseDown(MOUSE_LEFT) && INPUT->GetMousePos().x < VIEW_WIDTH - UIBORDER)
		{
			float height = m_vCursorPos.y;
			m_pUIInputField_SetHeight->SetText(to_string(height));
			m_getHeight = false;
		}
		if (INPUT->IsMouseDown(MOUSE_RIGHT)) m_getHeight = false;
	}
	if (m_pUIButton_SetHeight->GetCurrentState() == E_UISTATE_CLICKED) m_pMap->SetFixedHeight(stof(m_pUIInputField_SetHeight->GetText()));
}

// 메뉴 업데이트
void cMainGame::Update_Menu()
{
	// InputField 켜진 상황에서 맵 업데이트 안되도록
	if (m_pUIInputField_CellSpace->GetIsOn() ||
		m_pUIInputField_CellNum->GetIsOn() ||
		m_pUIInputField_FilePath->GetIsOn())
		m_isUpdateMap = false;

	if (m_pUIButton_create->GetCurrentState() == E_UISTATE_CLICKED)
	{
		// 기존 오브젝트 삭제
		for each(auto p in m_vecConstruct)
		{
			p->Destroy();
		}

		float fCellSpace = atof(m_pUIInputField_CellSpace->GetText().c_str());
		int nCellNum = atoi(m_pUIInputField_CellNum->GetText().c_str());

		if (fCellSpace > 0.0f && fCellSpace < 5.0f &&
			nCellNum > 0 && nCellNum < 500)
		{
			m_fCellSpace = fCellSpace;
			m_nSize = nCellNum;
			//SAFE_DELETE(m_pMap);
			m_pMap = new cHeightMap;
			m_pMap->Setup(m_nSize, m_fCellSpace);

			SAFE_DELETE(m_pGrid);
			m_pGrid = new cGrid;
			m_pGrid->Setup(m_nSize / 2);
		}
	}

	if (m_pUIButton_save->GetCurrentState() == E_UISTATE_CLICKED) SaveMap();
	if (m_pUIButton_load->GetCurrentState() == E_UISTATE_CLICKED) LoadMap();
}

void cMainGame::Setup_DirLight()
{
	D3DLIGHT9 stLight;
	ZeroMemory(&stLight, sizeof(D3DLIGHT9));

	stLight.Type = D3DLIGHT_DIRECTIONAL;
	stLight.Ambient = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);				// 주변에 영향을 받는 것들은 색을 띄게 만듬
	stLight.Diffuse = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	stLight.Specular = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);

	D3DXVECTOR3	vDir(1.0f, 1.0f, 1.0f);
	D3DXVec3Normalize(&vDir, &vDir);
	stLight.Direction = vDir;
	DEVICE->SetLight(0, &stLight);
	SHADOW->SetLight(stLight, vDir);
	DEVICE->LightEnable(0, true);
}

// 세이브 버튼 눌렀을 때, 맵 정보 저장하도록.
void cMainGame::SaveMap()
{
	string filePath = "./map/" + m_pUIInputField_FilePath->GetText() + ".txt";
	string filePath_mtl = "./map/" + m_pUIInputField_FilePath->GetText() + "_mtl.txt";
	ofstream fileOut(filePath);
	ofstream fileOut_mtl(filePath_mtl);

	if (fileOut.is_open() && fileOut_mtl.is_open())
	{
		{
			fileOut << "cs " << m_pMap->GetCellSpace() << endl;
			fileOut << "cr " << m_pMap->GetCellPerRow() << endl;
			fileOut << "ws " << m_isSnowOn << ' ' << m_pSnow->GetCount() << ' ' << m_pSnow->GetMove() << ' ' << m_pSnow->GetSpeed() <<  endl;		// 눈 정보
			fileOut << "wr " << m_isRainOn << ' ' << m_pRain->GetCount() << ' ' << m_pRain->GetMove() << ' ' << m_pRain->GetSpeed() << endl;		// 비 정보
			fileOut << "wf " << m_isFogOn << ' ' << m_nPassIndex << endl;		// 안개 정보
			fileOut << "es " << m_isShodowOn << ' ' << m_diffuseAlpha << endl;
		}

		cMtlTex* pMtlTex = m_pMap->GetVecMtlTex()[0];
		{
			fileOut << "mtllib " << m_pUIInputField_FilePath->GetText() + "_mtl.txt" << endl;

			fileOut_mtl << "newmtl " << "Map" << endl;
			fileOut_mtl << "Ka " << "0.700000 0.700000 0.700000" << endl;
			fileOut_mtl << "Kd " << "0.700000 0.700000 0.700000" << endl;
			fileOut_mtl << "Ks " << "0.700000 0.700000 0.700000" << endl;
			fileOut_mtl << "map_Kd bottom.bmp" << endl;
		}

		vector<ST_PNT_VERTEX> vecVertex = m_pMap->GetVecVertex();
		for (int i = 0; i < vecVertex.size(); i++)
		{
			fileOut << "v " << vecVertex[i].p.x << ' ' << vecVertex[i].p.y << ' ' << vecVertex[i].p.z << endl;
			fileOut << "vt " << vecVertex[i].t.x << ' ' << vecVertex[i].t.y << ' ' << endl;
			fileOut << "vn " << vecVertex[i].n.x << ' ' << vecVertex[i].n.y << ' ' << vecVertex[i].n.z << endl;
		}

		{
			fileOut << "usemtl Map" << endl;
		}

		vector<DWORD>vecIndex = m_pMap->GetVecIndex();
		for (int i = 0; i < vecIndex.size(); i += 3)
		{
			fileOut << "f " << vecIndex[i + 0] + 1 << '/' << vecIndex[i + 0] + 1 << '/' << vecIndex[i + 0] + 1;
			fileOut << " " << vecIndex[i + 1] + 1 << '/' << vecIndex[i + 1] + 1 << '/' << vecIndex[i + 1] + 1;
			fileOut << " " << vecIndex[i + 2] + 1 << '/' << vecIndex[i + 2] + 1 << '/' << vecIndex[i + 2] + 1 << endl;
		}

		for (int i = 0; i < m_vecConstruct.size(); i++)
		{
			cConstruct* pConstruct = m_vecConstruct[i];

			int nSID = pConstruct->GetSObjID();
			D3DXVECTOR3 vPos = (pConstruct->GetPosition());
			D3DXVECTOR3 vScale = pConstruct->GetScale();
			float fRotX = (pConstruct->GetRotationX());
			float fRotY = (pConstruct->GetRotationY());
			float fRotZ = (pConstruct->GetRotationZ());

			if(nSID < E_S_OBJECTID_N_H_START) fileOut << "o ";
			else fileOut << "p ";
			
			fileOut<< nSID << ' '
				<< vPos.x << ' ' << vPos.y << ' ' << vPos.z << ' '
				<< vScale.x << ' ' << vScale.y << ' ' << vScale.z << ' '
				<< fRotX << ' ' << fRotY << ' ' << fRotZ << endl;

		}
	}
	fileOut << endl;
	fileOut_mtl << endl;
	fileOut.close();
	fileOut_mtl.close();
}

// 로드 버튼 눌렀을 때, 맵 불러와서 생성하도록.
void cMainGame::LoadMap()
{
	// >> 오브젝트 초기화
	m_pConstruct = NULL;
	for each(auto p in m_vecConstruct)
	{
		p->Destroy();
	}
	// << 



	// >> 맵 로드
	char* folderPath = "map";
	string temp = m_pUIInputField_FilePath->GetText() + ".txt";
	char* filePath = strdup(temp.c_str());
	
	//if (m_pMap) SAFE_DELETE(m_pMap);
	
	m_pMap = new cHeightMap();
	cObjLoader loader;
	vector<cMtlTex*> vecMtlTex;
	vector<ST_PNT_VERTEX> vecVertex;
	vector<DWORD> vecIndex;
	int nCellPerRow = 0;
	float fCellSpace = 0.0f;
	LPD3DXMESH pMesh = loader.LoadMesh_Map(vecMtlTex, vecVertex, vecIndex, nCellPerRow, fCellSpace, m_vecConstruct, folderPath, filePath, false);
	m_stWeather = loader.GetWeatherInfo();
	m_stShadow = loader.GetShadowInfo();
	m_pMap->Setup(nCellPerRow, fCellSpace, vecVertex, vecIndex);
	m_pMap->SetMesh(pMesh);
	m_pMap->SetVecMtlTex(vecMtlTex);
	// << 
}

void cMainGame::Setup_SkyBox()
{
	m_pSkyBox = new cSkyBox();
	m_pSkyBox->Setup(m_nSize / 2, m_nSize / 2, m_nSize / 2, "map/SkyBox/Town_Human", "png");
}

void cMainGame::Update_Effect()
{
	// Fog 처리
	if (m_pRadioButton_Fog->GetSID() != -1)
	{
		m_isFogOn = true;
		m_pFog->Update(m_pCamera);

		if (m_pUIButton_Fog_Minus->GetCurrentState() == E_UISTATE_CLICKED)
		{
			m_nPassIndex--;
			if (m_nPassIndex < 0) m_nPassIndex = 0;
		}
		if (m_pUIButton_Fog_Plus->GetCurrentState() == E_UISTATE_CLICKED)
		{
			m_nPassIndex++;
			if (m_nPassIndex > 5) m_nPassIndex = 5;
		}
	}
	else if (m_pRadioButton_Fog->GetSID() == -1) m_isFogOn = false;

	//shadow 처리
	if (m_pRadioButton_Shadow->GetSID() != -1)
	{
		m_isShodowOn = true;	
		SHADOW->SetViewOk(true);
		if (m_pUIButton_Shadow_Minus->GetCurrentState() == E_UISTATE_CLICKED)
		{
			m_diffuseAlpha = m_diffuseAlpha - 0.1f;
		}
		if (m_pUIButton_Shadow_Plus->GetCurrentState() == E_UISTATE_CLICKED)
		{		
			m_diffuseAlpha = m_diffuseAlpha + 0.1f;
		}
		SHADOW->UpdateAlpha(m_diffuseAlpha);
	}
	else if (m_pRadioButton_Fog->GetSID() == -1) SHADOW->SetViewOk(false);
	// Snow 처리
	if (m_pRadioButton_Snow->GetSID() != -1)
	{
		m_isSnowOn = true;
		m_pSnow->Update(0.1f, 0.1f);

		if (m_pUIButton_Snow_Minus->GetCurrentState() == E_UISTATE_CLICKED)
		{
			m_pSnow->DeleteParticle(1000);
		}
		if (m_pUIButton_Snow_Plus->GetCurrentState() == E_UISTATE_CLICKED)
		{
			if (m_pSnow->GetVerParticleVertex().size() <= 5000) m_pSnow->AddParticle(1000);
		}
	}
	else if (m_pRadioButton_Snow->GetSID() == -1) m_isSnowOn = false;

	// Rain 처리
	if (m_pRadioButton_Rain->GetSID() != -1)
	{
		m_isRainOn = true;
		m_pRain->Update(0.1f, 1.0f);

		if (m_pUIButton_Rain_Minus->GetCurrentState() == E_UISTATE_CLICKED)
		{
			m_pRain->DeleteParticle(1000);
		}
		if (m_pUIButton_Rain_Plus->GetCurrentState() == E_UISTATE_CLICKED)
		{
			if(m_pRain->GetVerParticleVertex().size() <= 7000) m_pRain->AddParticle(1000);
		}
	}
	else if (m_pRadioButton_Rain->GetSID() == -1) m_isRainOn = false;
}

void cMainGame::Render_Effect()
{

}

void cMainGame::Render_Effect_Fog()
{
	m_pFog->Render_Begin(m_nPassIndex);

	if (m_pMap) m_pMap->Render();
	if (m_pSkyBox) m_pSkyBox->Render();
	Render_Object();

	m_pFog->Render_End();
}