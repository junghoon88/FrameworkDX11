#pragma once

namespace Landscape
{
	class Terrain
	{
	public:
		Terrain(ExecuteValues* values);
		~Terrain();

		void Update();
		void Render();
		void PostRender();

	private:
		ExecuteValues* values;
		class Data* data;

		Material* material;
		WorldBuffer* worldBuffer;

		ID3D11SamplerState* diffuseSampler;
		ID3D11SamplerState* detailSampler;
	};
}