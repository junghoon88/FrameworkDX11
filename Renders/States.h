#pragma once
class States
{
public:
	static void Create(void);
	static void Delete(void);

	static void GetRasterizerDesc(D3D11_RASTERIZER_DESC* desc);
	static void CreateRasterizer(D3D11_RASTERIZER_DESC* desc, ID3D11RasterizerState** state);
	static void SetRasterizer(ID3D11RasterizerState* state);

	static void GetDepthStencilDesc(D3D11_DEPTH_STENCIL_DESC* desc);
	static void CreateDepthStencil(D3D11_DEPTH_STENCIL_DESC* desc, ID3D11DepthStencilState** state);
	static void SetDepthStencil(ID3D11DepthStencilState* state);

	static void GetSamplerDesc(D3D11_SAMPLER_DESC* desc);
	static void CreateSampler(D3D11_SAMPLER_DESC* desc, ID3D11SamplerState** state);
	static void SetSampler(UINT slot, UINT count, ID3D11SamplerState* state);

private:
	static void CreateRasterizerDesc(void);
	static void CreateDepthStencilDesc(void);
	static void CreateSamplerDesc(void);

	static D3D11_RASTERIZER_DESC rasterizerDesc;
	static D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	static D3D11_SAMPLER_DESC samplerDesc;
};

