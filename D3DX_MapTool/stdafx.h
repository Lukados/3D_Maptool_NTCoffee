// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
#include <vector>
#include <map>
#include <Windows.h>
#include <string>
#include <assert.h>
#include <fstream>
#include <ctime>

#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

using namespace std;

extern HWND g_hWnd;

#define SAFE_RELEASE(p) { if(p) p->Release(); p = NULL; }
#define SAFE_DELETE(p) {if(p) delete p; p = NULL;}
#define SINGLETON(class_name)\
		private: \
			class_name(void) ;\
			~class_name(void);\
		public:\
			static class_name* GetInstance()\
			{\
				static class_name instance; \
				return &instance;\
			}
#define SYNTHESIZE(varType, varName, funName)\
protected: varType varName;\
public: inline varType Get##funName(void) const { return varName; }\
public: inline void Set##funName(varType var){ varName = var; }

#define SAFE_ADD_REF(p) { if(p) p->AddRef() ; }

#define SYNTHESIZE_PASS_BY_REF(varType, varName, funName)\
protected: varType varName;\
public: inline varType& Get##funName(void) { return varName; }\
public: inline void Set##funName(varType& var){ varName = var; }

#define SYNTHESIZE_ADD_REF(varType, varName, funName)    \
protected: varType varName; \
public: virtual varType Get##funName(void) const { return varName; } \
public: virtual void Set##funName(varType var){\
	if (varName != var) {\
	SAFE_ADD_REF(var);\
	SAFE_RELEASE(varName);\
	varName = var;\
	}\
}

#define VIEW_WIDTH 1400
#define VIEW_HEIGHT	900
#define UIBORDER 300

struct ST_PC_VERTEX
{
	D3DXVECTOR3 p;
	D3DCOLOR	c;
	enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };
};

struct ST_PN_VERTEX
{
	D3DXVECTOR3 p;
	D3DXVECTOR3	n;
	enum { FVF = D3DFVF_XYZ | D3DFVF_NORMAL };
};

struct ST_PT_VERTEX
{
	D3DXVECTOR3 p;
	D3DXVECTOR2 t;

	enum { FVF = D3DFVF_XYZ | D3DFVF_TEX1 };
};

struct ST_PNT_VERTEX
{
	D3DXVECTOR3 p;
	D3DXVECTOR3 n;
	D3DXVECTOR2 t;

	enum { FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 };
};

struct ST_SIZE
{
	int nWidth;
	int nHeight;

	ST_SIZE() {}
	ST_SIZE(int width, int height) { nWidth = width; nHeight = height; }
};

enum eUIOBJECT
{
	E_UI_NONE, E_UI_TAB, E_UI_BUTTON, E_UI_SCROLL, E_UI_TEXT, E_UI_RADIOBUTTON, E_UI_INPUTFIELD, 
};

enum eUISTATE
{
	E_UISTATE_NONE, E_UISTATE_IDLE, E_UISTATE_MOUSEOVER, E_UISTATE_CLICKED, E_UISTATE_SELECTED, E_UISTATE_PRESSED, 
};

enum eFONTTYPE
{
	E_FONT_DEFAULT,
};


#include "cDeviceManager.h"
#include "cTextureManager.h"
#include "cInputManager.h"
#include "cTimeManager.h"
#include "cFontManager.h"
#include "cObjectManager.h"
#include "cObjectDB.h"
#include "cObject.h"
#include "cMath.h"
#include "cRay.h"