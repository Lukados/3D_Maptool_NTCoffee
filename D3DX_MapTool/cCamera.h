#pragma once
class cCamera
{
public:
	cCamera();
	~cCamera();

private : 
	D3DXVECTOR3	 m_vEye; 
	D3DXVECTOR3	 m_vLookAt;
	D3DXVECTOR3	 m_vUp;
	D3DXVECTOR3	* m_pvTarget;

	POINT		m_ptPrevMouse; 
	bool		m_isLButtonDown; 
	float		m_fCameraDistance; 
	D3DXVECTOR3 m_vCamRotAngle; 

public :
	void Setup(D3DXVECTOR3* pvTarget);
	void Update();

	void WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam); 

	D3DXVECTOR3& GetEye() { return m_vEye; }
	D3DXVECTOR3 GetCamRotAngle() { return m_vCamRotAngle; }

	// >> : SkyBox에서 끌어쓰기 위해 추가
	D3DXVECTOR3& GetLookAt() { return m_vLookAt; }
	D3DXVECTOR3& GetUp() { return m_vUp; }

	D3DXMATRIXA16 m_useShadow;
	D3DXMATRIXA16 GetMatrix();
	// << :
};

