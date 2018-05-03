#pragma once

namespace Model { class BinModel; }

namespace Fbx
{
	struct MaterialData;
	struct MeshData;

	class FbxModels;

	class FbxModel
	{
	public:
		friend FbxModels;

	private:
		FbxModel(wstring filename);
		~FbxModel();

	private:
		void PushMaterialData(FbxSurfaceMaterial* material, int number);

		void Write(wstring file);
		static void Write(wstring file, Model::BinModel* model);
		static void Read(wstring file, Model::BinModel** model);

	private:
		string name;

		D3DXMATRIX world;

		vector<MaterialData*>	materials;
		vector<MeshData*>		meshes;
	
	public:
		static const wstring DefaultShader;
	};
}