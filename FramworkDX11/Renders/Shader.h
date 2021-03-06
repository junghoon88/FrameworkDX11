#pragma once

class Shader
{
public:
	Shader(wstring shaderFile);
	~Shader();

	void Render(void);

public:
	inline wstring GetFile(void) { return shaderFile; }

private:
	void CreateVertexShader(void);
	void CreatePixelShader(void);
	void CheckShaderError(HRESULT hr, ID3DBlob* error);
	void CreateInputLayout(void);

private:
	wstring shaderFile;
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;

	ID3D11InputLayout* inputLayout;

	ID3DBlob* vertexBlob;
	ID3DBlob* pixelBlob;

	ID3D11ShaderReflection* reflection;
};