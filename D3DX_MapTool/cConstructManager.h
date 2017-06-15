#pragma once

class cConstruct;

#define CONSTRUCT cConstructManager::GetInstance()

class cConstructManager
{
	SINGLETON(cConstructManager);

private:

public:
	void CreateConstruct();
	void RemoveConstruct();
	void Destroy();
};

