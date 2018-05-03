#include "stdafx.h"
#include "Material.h"
#include "../Utilities/String.h"

Material::Material(wstring filename)
	: shader(NULL)
	, diffuseMap(NULL), specularMap(NULL), emissiveMap(NULL), normalMap(NULL), detailMap(NULL)
{
	if(filename.length() > 0)
		shader = new Shader(filename);

	buffer = new MaterialBuffer();
}

Material::~Material()
{
	SAFE_DELETE(diffuseMap);
	SAFE_DELETE(specularMap);
	SAFE_DELETE(emissiveMap);
	SAFE_DELETE(normalMap);
	SAFE_DELETE(detailMap);

	SAFE_DELETE(buffer);
	SAFE_DELETE(shader);
}

void Material::SetShader(wstring filename)
{
	if (filename.length() == 0) return;

	SAFE_DELETE(shader);
	shader = new Shader(filename);
}

void Material::SetDiffuseMap(string file, D3DX11_IMAGE_LOAD_INFO * info)
{
	SetDiffuseMap(String::StringToWString(file));
}

void Material::SetDiffuseMap(wstring file, D3DX11_IMAGE_LOAD_INFO * info)
{
	if (file.length() == 0) return;
	SAFE_DELETE(diffuseMap);

	diffuseMap = new Texture(file, info);
}

void Material::SetSpecularMap(string file, D3DX11_IMAGE_LOAD_INFO * info)
{
	SetSpecularMap(String::StringToWString(file));
}

void Material::SetSpecularMap(wstring file, D3DX11_IMAGE_LOAD_INFO * info)
{
	if (file.length() == 0) return;
	SAFE_DELETE(specularMap);

	specularMap = new Texture(file, info);
}

void Material::SetEmissiveMap(string file, D3DX11_IMAGE_LOAD_INFO * info)
{
	SetEmissiveMap(String::StringToWString(file));
}

void Material::SetEmissiveMap(wstring file, D3DX11_IMAGE_LOAD_INFO * info)
{
	if (file.length() == 0) return;
	SAFE_DELETE(emissiveMap);

	emissiveMap = new Texture(file, info);
}

void Material::SetNormalMap(string file, D3DX11_IMAGE_LOAD_INFO * info)
{
	SetNormalMap(String::StringToWString(file));
}

void Material::SetNormalMap(wstring file, D3DX11_IMAGE_LOAD_INFO * info)
{
	if (file.length() == 0) return;
	SAFE_DELETE(normalMap);

	normalMap = new Texture(file, info);
}

void Material::SetDetailMap(string file, D3DX11_IMAGE_LOAD_INFO * info)
{
	SetDetailMap(String::StringToWString(file));
}

void Material::SetDetailMap(wstring file, D3DX11_IMAGE_LOAD_INFO * info)
{
	if (file.length() == 0) return;
	SAFE_DELETE(detailMap);

	detailMap = new Texture(file, info);
}

void Material::SetBuffer(void)
{
	UINT slot = 0;

	if (diffuseMap != NULL)
	{
		diffuseMap->SetShaderResource(slot);
		diffuseMap->SetShaderSampler(slot);
	}
	else
	{
		//Texture::SetBlankShaderResource(slot);
		//Texture::SetBlankSamplerState(slot);
	}


	slot = 1;
	if (specularMap != NULL)
	{
		specularMap->SetShaderResource(slot);
		specularMap->SetShaderSampler(slot);
	}
	else
	{
		//Texture::SetBlankShaderResource(slot);
		//Texture::SetBlankSamplerState(slot);
	}


	slot = 2;
	if (emissiveMap != NULL)
	{
		//emissiveMap->SetShaderResource(slot);
		//emissiveMap->SetShaderSampler(slot);
	}
	else
	{
		//Texture::SetBlankShaderResource(slot);
		//Texture::SetBlankSamplerState(slot);
	}


	slot = 3;
	if (normalMap != NULL)
	{
		normalMap->SetShaderResource(slot);
		normalMap->SetShaderSampler(slot);
	}
	else
	{
		//Texture::SetBlankShaderResource(slot);
		//Texture::SetBlankSamplerState(slot);
	}


	slot = 4;
	if (detailMap != NULL)
	{
		detailMap->SetShaderResource(slot);
		detailMap->SetShaderSampler(slot);
	}
	else
	{
		//Texture::SetBlankShaderResource(slot);
		//Texture::SetBlankSamplerState(slot);
	}

	buffer->SetPSBuffer(1);

	if(shader)	shader->Render();
}
