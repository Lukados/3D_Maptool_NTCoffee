#pragma once
#include <set>

class cObject;

#define g_pObjectManager cObjectManager::GetInstance()

class cObjectManager
{
private:
	SINGLETON(cObjectManager);

	set<cObject*> m_setObject;
	
public:
	void AddObject(cObject* pObject);
	void RemoveObject(cObject* pObject);
	void Destroy();
};

