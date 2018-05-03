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
		//DX10 부터 HLSL이 열우선으로 바뀌었기 때문에 Transpose 를 해야한다. 
		//View 를 행우선(D3DXMatrixLookAtLH)으로 만들었음 
		//이 구조를 변경하려면 World부터 TRS, RH로 만들어서 해야함.;;
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
		//DX10 부터 HLSL이 열우선으로 바뀌었기 때문에 Transpose 를 해야한다. 
		//View 를 행우선(D3DXMatrixLookAtLH)으로 만들었음 
		//이 구조를 변경하려면 World부터 TRS, RH로 만들어서 해야함.;;
		D3DXMatrixTranspose(&Data.View, &Data.View);
	}

	void SetProjection(D3DXMATRIX mat)
	{
		Data.Projection = mat;
		//DX10 부터 HLSL이 열우선으로 바뀌었기 때문에 Transpose 를 해야한다. 
		//Projection 을 행우선(D3DXMatrixPerspectiveFovLH)으로 만들었음 
		//이 구조를 변경하려면 World부터 TRS, RH로 만들어서 해야함.;;
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
