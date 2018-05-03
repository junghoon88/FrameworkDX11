#include "stdafx.h"
#include "Terrain.h"
#include "Data.h"

Landscape::Terrain::Terrain(ExecuteValues*  values)
{
	material = new Material(Shaders + L"Material.hlsl");
	material->SetDiffuseMap(Contents + L"Landscape/Dirt3.png");
	material->SetDetailMap(Contents + L"Landscape/Dirt3_detail.png");

	worldBuffer = new WorldBuffer();

	data = new Data(Contents + L"Landscape/HeightMap256.png");


	D3D11_SAMPLER_DESC samplerDesc;
	States::GetSamplerDesc(&samplerDesc);
	States::CreateSampler(&samplerDesc, &diffuseSampler);

	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	States::CreateSampler(&samplerDesc, &detailSampler);
}

Landscape::Terrain::~Terrain()
{
	SAFE_RELEASE(diffuseSampler);
	SAFE_RELEASE(detailSampler);

	SAFE_DELETE(data);

	SAFE_DELETE(worldBuffer);
	SAFE_DELETE(material);
}

void Landscape::Terrain::Update()
{
	
}

void Landscape::Terrain::Render()
{
	worldBuffer->SetVSBuffer(1);
	material->SetBuffer();

	D3D::GetDC()->PSSetSamplers(0, 1, &diffuseSampler);
	D3D::GetDC()->PSSetSamplers(1, 1, &detailSampler);

	data->Render();
}

void Landscape::Terrain::PostRender()
{
}
