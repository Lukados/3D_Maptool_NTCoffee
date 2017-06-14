#pragma once

class cObject
{
protected:
	ULONG m_ulRefCount;

public:
	cObject();

	virtual ~cObject();
	virtual void AddRef();
	virtual void Release();
};

