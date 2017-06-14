#include "stdafx.h"
#include "cMath.h"


cMath::cMath()
{
}


cMath::~cMath()
{
}

int cMath::Max(int num, int num2)
{
	return (num >= num2) ? num : num2;
}

int cMath::Min(int num, int num2)
{
	return (num <= num2) ? num : num2;
}

int cMath::Clamp(int min, int max, int num)
{
	if (num >= max) num = max;
	if (num <= min) num = min;

	return num;
}

float cMath::Max(float num, float num2)
{
	return (num >= num2) ? num : num2;
}

float cMath::Min(float num, float num2)
{
	return (num <= num2) ? num : num2;
}

float cMath::Clamp(float min, float max, float num)
{
	if (num >= max) num = max;
	if (num <= min) num = min;

	return num;
}

// 두 점 사이의 거리가 range 이하인지 확인
bool cMath::CheckDistance(D3DXVECTOR3 v1, D3DXVECTOR3 v2, float range)
{
	float distance = sqrt(pow(v1.x - v2.x, 2) + pow(v1.y - v2.y, 2) + pow(v1.z - v2.z, 2));
	return (distance <= range);
}

// 두 점 사이의 거리가 range 이하인지 확인
bool cMath::CheckDistance(D3DXVECTOR2 v1, D3DXVECTOR2 v2, float range)
{
	float distance = sqrt(pow(v1.x - v2.x, 2) + pow(v1.y - v2.y, 2));
	return (distance <= range);
}

bool cMath::CheckDistanceOfTri(D3DXVECTOR3 v1, D3DXVECTOR3 v2, D3DXVECTOR3 v3, D3DXVECTOR3 vStandard, float range)
{
	bool distance1 = CheckDistance(D3DXVECTOR2(v1.x, v1.z), D3DXVECTOR2(vStandard.x, vStandard.z), range);
	bool distance2 = CheckDistance(D3DXVECTOR2(v2.x, v2.z), D3DXVECTOR2(vStandard.x, vStandard.z), range);
	bool distance3 = CheckDistance(D3DXVECTOR2(v3.x, v3.z), D3DXVECTOR2(vStandard.x, vStandard.z), range);
	return true;
	return distance1 || distance2 || distance3;
}

// 두 점 사이의 거리
float cMath::Distance(D3DXVECTOR2 v1, D3DXVECTOR2 v2)
{
	float sqrDistance = pow(v1.x - v2.x, 2) + pow(v1.y - v2.y, 2);
	return sqrt(sqrDistance);
}

// 두 점 사이의 거리
float cMath::Distance(D3DXVECTOR3 v1, D3DXVECTOR3 v2)
{
	float sqrDistance = pow(v1.x - v2.x, 2) + pow(v1.y - v2.y, 2) + pow(v1.z - v2.z, 2);
	return sqrt(sqrDistance);
}