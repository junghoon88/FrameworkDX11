#pragma once
#include "stdafx.h"

class WorldBuffer : public ShaderBuffer
{
public:
	WorldBuffer() : ShaderBuffer(&Data, sizeof(Struct))
	{
		D3DXMatrixIdentity(&Data.World);
	}

	void SetMatrix(D3DXMATRIX mat)
	{
		Data.World = mat;
		//DX10 ���� HLSL�� ���켱���� �ٲ���� ������ Transpose �� �ؾ��Ѵ�. 
		//View �� ��켱(D3DXMatrixLookAtLH)���� ������� 
		//�� ������ �����Ϸ��� World���� TRS, RH�� ���� �ؾ���.;;
		D3DXMatrixTranspose(&Data.World, &Data.World);
	}

	struct Struct
	{
		D3DXMATRIX World;
	};

private:
	Struct Data;
};

class ViewProjectionBuffer : public ShaderBuffer
{
public:
	ViewProjectionBuffer() : ShaderBuffer(&Data, sizeof(Struct))
	{
		D3DXMatrixIdentity(&Data.View);
		D3DXMatrixIdentity(&Data.Projection);
	}

	void SetView(D3DXMATRIX mat)
	{
		Data.View = mat;
		//DX10 ���� HLSL�� ���켱���� �ٲ���� ������ Transpose �� �ؾ��Ѵ�. 
		//View �� ��켱(D3DXMatrixLookAtLH)���� ������� 
		//�� ������ �����Ϸ��� World���� TRS, RH�� ���� �ؾ���.;;
		D3DXMatrixTranspose(&Data.View, &Data.View);
	}

	void SetProjection(D3DXMATRIX mat)
	{
		Data.Projection = mat;
		//DX10 ���� HLSL�� ���켱���� �ٲ���� ������ Transpose �� �ؾ��Ѵ�. 
		//Projection �� ��켱(D3DXMatrixPerspectiveFovLH)���� ������� 
		//�� ������ �����Ϸ��� World���� TRS, RH�� ���� �ؾ���.;;
		D3DXMatrixTranspose(&Data.Projection, &Data.Projection);
	}

	struct Struct
	{
		D3DXMATRIX View;
		D3DXMATRIX Projection;
	};

private:
	Struct Data;
};

class LightBuffer : public ShaderBuffer
{
public:
	LightBuffer() : ShaderBuffer(&Data, sizeof(Struct))
	{
		Data.Direction = D3DXVECTOR3(-1, -1, -1);
	}

	struct Struct
	{
		D3DXVECTOR3 Direction;

		float Padding;
	};

	Struct Data;
};
