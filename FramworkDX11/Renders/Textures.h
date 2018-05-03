#pragma once

class Texture
{
public:
	friend class Textures;

public:
	static void SetShaderResources(UINT slot, UINT count, Texture** textures);
	static void SetBlankShaderResource(UINT slot);
	static void SetBlankSamplerState(UINT slot);


public:
	Texture(wstring file, D3DX11_IMAGE_LOAD_INFO* loadInfo = NULL);
	~Texture();


	D3D11_TEXTURE2D_DESC ReadPixels(DXGI_FORMAT readFormat, vector<D3DXCOLOR>* pixels);

	inline wstring GetFile(void) { return file; }
	inline void GetImageInfo(D3DX11_IMAGE_INFO* info) { *info = this->info; }
	inline UINT GetWidth(void)  { return info.Width; }
	inline UINT GetHeight(void) { return info.Height; }
	inline ID3D11ShaderResourceView* GetView(void) { return view; }
	inline void SetSamplerState(D3D11_SAMPLER_DESC desc) { SAFE_RELEASE(samplerState); States::CreateSampler(&desc, &samplerState); }

	void SetShaderResource(UINT slot);
	void SetShaderSampler(UINT slot);


private:
	wstring file;

	D3DX11_IMAGE_INFO info;
	ID3D11ShaderResourceView* view;
	ID3D11SamplerState* samplerState;

};

struct TextureDesc
{
	wstring file;
	UINT width, height;
	D3DX11_IMAGE_INFO info;
	ID3D11ShaderResourceView* view;

	UINT ref;

	bool operator==(const TextureDesc& desc)
	{
		bool b = true;
		b &= file == desc.file;
		b &= width == desc.width;
		b &= height == desc.height;

		return b;
	}
};

class Textures
{
public:
	friend class Texture;

public:
	static void Delete(void);

private:
	static void Load(Texture* texture, D3DX11_IMAGE_LOAD_INFO* loadInfo = NULL);
	static void UnLoad(Texture* texture);

private:
	static vector<TextureDesc> descs;
};