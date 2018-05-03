#pragma once
class Math
{
public:
	static const float PI;

	static float ToRadian(float degree);
	static D3DXVECTOR3 ToRadian(D3DXVECTOR3 vec);

	static float ToDegree(float radian);

	static int RandomRange(int r1, int r2);
};