#include "stdafx.h"
#include "States.h"

D3D11_RASTERIZER_DESC States::rasterizerDesc;
D3D11_DEPTH_STENCIL_DESC States::depthStencilDesc;
D3D11_SAMPLER_DESC States::samplerDesc;

void States::Create(void)
{
	CreateRasterizerDesc();
	CreateDepthStencilDesc();
	CreateSamplerDesc();
}

void States::Delete(void)
{
}

void States::GetRasterizerDesc(D3D11_RASTERIZER_DESC* desc)
{
	memcpy(desc, &rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
}

void States::CreateRasterizer(D3D11_RASTERIZER_DESC* desc, ID3D11RasterizerState** state)
{
	HRESULT hr;

	hr = gDevice->CreateRasterizerState(desc, state);
	assert(SUCCEEDED(hr));
}

void States::SetRasterizer(ID3D11RasterizerState * state)
{
	gDC->RSSetState(state);
}

void States::GetDepthStencilDesc(D3D11_DEPTH_STENCIL_DESC* desc)
{
	memcpy(desc, &depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
}

void States::CreateDepthStencil(D3D11_DEPTH_STENCIL_DESC * desc, ID3D11DepthStencilState ** state)
{
	HRESULT hr;

	hr = gDevice->CreateDepthStencilState(desc, state);
	assert(SUCCEEDED(hr));
}

void States::SetDepthStencil(ID3D11DepthStencilState * state)
{
	gDC->OMSetDepthStencilState(state, 1);
}

void States::GetSamplerDesc(D3D11_SAMPLER_DESC * desc)
{
	memcpy(desc, &samplerDesc, sizeof(D3D11_SAMPLER_DESC));
}

void States::CreateSampler(D3D11_SAMPLER_DESC * desc, ID3D11SamplerState ** state)
{
	HRESULT hr;

	hr = gDevice->CreateSamplerState(desc, state);
	assert(SUCCEEDED(hr));
}

void States::SetSampler(UINT slot, UINT count, ID3D11SamplerState * state)
{
	gDC->PSSetSamplers(slot, count, &state);
}

void States::CreateRasterizerDesc(void)
{
	ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	rasterizerDesc.AntialiasedLineEnable = false;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.DepthBias = 0;
	rasterizerDesc.DepthBiasClamp = 0.0f;
	rasterizerDesc.DepthClipEnable = true;
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.MultisampleEnable = false;
	rasterizerDesc.ScissorEnable = false;
	rasterizerDesc.SlopeScaledDepthBias = 0.0f;
}

void States::CreateDepthStencilDesc(void)
{
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
}

void States::CreateSamplerDesc(void)
{
	//MIN_MAG_MIP  
	//MIN : 텍스쳐가 축소됐을때(예:텍스쳐(256x256)가 화면에서 ( 64x 64)로 보일 때)
	//MAG : 텍스쳐가 확대됐을때(예:텍스쳐(256x256)가 화면에서 (512x512)로 보일 때)
	//MIP : 밉맵 : 텍스쳐원본 기준으로 2의승수 크기마다 새로운 텍스쳐를 만든다.
	//             (예: 텍스쳐(256x256) -> 텍스쳐1(512x512), 텍스쳐2(128x128), 텍스쳐3(64x64) ...
	//             이후에 보여줄 사이즈의 한단계큰값, 한단계 작은값을 가진 텍스쳐 2개를 기준으로 보간한다.
	//POINT : 상수보간
	//LINEAR : 선형보간

	//비등방 필터링 : 카메라 각도랑 수직에 가까운경우 품질개선을 위해 사용

	ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
}