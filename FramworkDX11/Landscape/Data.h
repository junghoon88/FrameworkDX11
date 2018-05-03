#pragma once

namespace Landscape
{
	class Data
	{
	public:
		typedef VertexTextureNormal VertexType;

	public:
		Data(wstring file = L"", float heightRatio = 10.0f);
		~Data();

		float GetHeight(D3DXVECTOR3 position);

		void SetHeightMapFile(wstring file, float heightRatio = 10.0f);
		void UpdateVertexData(D3D11_BOX* box = NULL);

		void Render();

	private:
		void Clear();

		void FillVertexData(float heightRatio);
		void FillNormalData();
		
		void CreateBuffer();

	private:
		Texture* heightMap;

		UINT width, height;
		UINT vertexCount, indexCount;
		ID3D11Buffer* vertexBuffer, *indexBuffer;

		VertexType* vertexData;
		UINT* indexData;
	};
}