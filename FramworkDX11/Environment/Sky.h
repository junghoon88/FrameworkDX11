#pragma once

namespace Model { class BinModel; }

namespace Environment
{
	class Sky
	{
	public:
		class Buffer : public ShaderBuffer
		{
		public:
			Buffer() : ShaderBuffer(&Data, sizeof(Struct))
			{
				Data.Apex   = D3DXCOLOR(0xFF0080FF);
				Data.Center = D3DXCOLOR(0xFF9BCDFF);
				Data.Height = 2.0f;
			}

			struct Struct
			{
				D3DXCOLOR Center;	//
				D3DXCOLOR Apex;		//정점
				float Height;		//높이 보정값
				float Padding[3];
			} Data;
		};

		Sky(ExecuteValues* values);
		~Sky();

		void Update(void);
		void Render(void);
		void PostRender(void);

	private:
		ExecuteValues*		values;


		wstring				shaderFile;
		wstring				modelFile;

		Model::BinModel*	model;
		Material*			material;

		Buffer*				buffer;

		ID3D11RasterizerState*		rasterizer[2];
		ID3D11DepthStencilState*	depthStencilState[2];
	};
}
