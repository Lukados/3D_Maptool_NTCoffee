#pragma once

#define DEVICEMGR cDeviceManager::GetInstance()
#define DEVICE cDeviceManager::GetInstance()->GetDevice()

class cDeviceManager
{
public:
	SINGLETON(cDeviceManager);	

private:
	LPDIRECT3D9			m_pD3D;
	LPDIRECT3DDEVICE9	m_pD3DDevice;

public:
	LPDIRECT3DDEVICE9 GetDevice();
	void Destroy();
};

