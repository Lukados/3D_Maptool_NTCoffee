#pragma once

#define CONSTRUCT	cConstructManager::GetInstance()

class cConstruct;

class cConstructManager
{
	SINGLETON(cConstructManager);

private:


public:
	void AddConstruct(cConstruct* pConsturct);
	void DeleteConstruct(cConstruct* pConstruct);
	void Destroy();
};

