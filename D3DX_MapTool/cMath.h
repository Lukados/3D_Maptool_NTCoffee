#pragma once

#define MATH cMath::GetInstance()
class cMath
{
	SINGLETON(cMath);
public:
	int Max(int num, int num2);
	int Min(int num, int num2);
	int Clamp(int min, int max, int num);
	float Max(float num, float num2);
	float Min(float num, float num2);
	float Clamp(float min, float max, float num);
	bool CheckDistance(D3DXVECTOR3 v1, D3DXVECTOR3 v2, float range);
	bool CheckDistance(D3DXVECTOR2 v1, D3DXVECTOR2 v2, float range);
	bool CheckDistanceOfTri(D3DXVECTOR3 v1, D3DXVECTOR3 v2, D3DXVECTOR3 v3, D3DXVECTOR3 vStandard, float range);
	float Distance(D3DXVECTOR2 v1, D3DXVECTOR2 v2);
	float Distance(D3DXVECTOR3 v1, D3DXVECTOR3 v2);
};

