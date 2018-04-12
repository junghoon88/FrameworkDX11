#pragma once

//=======================================================================================

struct Vertex
{
	Vertex() : position(0, 0, 0) {}
	Vertex(const D3DXVECTOR3& position) : position(position) {}

	D3DXVECTOR3      position;

	static D3D11_INPUT_ELEMENT_DESC desc[];
	static UINT      count;
};

//=======================================================================================

struct VertexColor
{
	VertexColor()
		: position(0, 0, 0)
		, color(0, 0, 0, 1)
	{}

	VertexColor(const D3DXVECTOR3& position,
		const D3DXCOLOR& color
	)
		: position(position)
		, color(color)
	{}

	D3DXVECTOR3      position;
	D3DXCOLOR      color;

	static D3D11_INPUT_ELEMENT_DESC desc[];
	static UINT      count;
};

//=======================================================================================

struct VertexColorNormal
{
	VertexColorNormal()
		: position(0, 0, 0)
		, color(0, 0, 0, 1)
		, normal(0, 0, 0)
	{}

	VertexColorNormal(const D3DXVECTOR3& position,
		const D3DXCOLOR& color,
		const D3DXVECTOR3& normal
	)
		: position(position)
		, color(color)
		, normal(normal)
	{}

	D3DXVECTOR3      position;
	D3DXCOLOR      color;
	D3DXVECTOR3      normal;

	static D3D11_INPUT_ELEMENT_DESC desc[];
	static UINT      count;
};

//=======================================================================================

struct VertexTexture
{
	VertexTexture()
		: position(0, 0, 0)
		, uv(0, 0)
	{}

	VertexTexture(const D3DXVECTOR3& position,
		const D3DXVECTOR2& uv
	)
		: position(position)
		, uv(uv)
	{}

	D3DXVECTOR3      position;
	D3DXVECTOR2      uv;

	static D3D11_INPUT_ELEMENT_DESC desc[];
	static UINT      count;
};

//=======================================================================================

struct VertexTextureNormal
{
	VertexTextureNormal()
		: position(0, 0, 0)
		, uv(0, 0)
		, normal(0, 0, 0)
	{}

	VertexTextureNormal(const D3DXVECTOR3& position,
		const D3DXVECTOR2& uv,
		const D3DXVECTOR3& normal
	)
		: position(position)
		, uv(uv)
		, normal(normal)
	{}

	D3DXVECTOR3      position;
	D3DXVECTOR2      uv;
	D3DXVECTOR3      normal;

	static D3D11_INPUT_ELEMENT_DESC desc[];
	static UINT      count;
};

//=======================================================================================

struct VertexColorTextureNormal
{
	VertexColorTextureNormal()
		: position(0, 0, 0)
		, color(0, 0, 0, 1)
		, uv(0, 0)
		, normal(0, 0, 0)
	{}

	VertexColorTextureNormal(const D3DXVECTOR3& position,
		const D3DXCOLOR& color,
		const D3DXVECTOR2& uv,
		const D3DXVECTOR3& normal
	)
		: position(position)
		, color(color)
		, uv(uv)
		, normal(normal)
	{}

	D3DXVECTOR3      position;
	D3DXCOLOR      color;
	D3DXVECTOR2      uv;
	D3DXVECTOR3      normal;

	static D3D11_INPUT_ELEMENT_DESC desc[];
	static UINT      count;
};