#pragma once

namespace Model { class BinModel; }

namespace Fbx
{
	class FbxModel;

	class FbxModels
	{
	public:
		friend FbxModel;

	public:
		static void Convert(wstring fbxFile, wstring saveFile);
		static void WriteBinary(wstring binFile, Model::BinModel* model);
		static void ReadBinary(wstring binFile, Model::BinModel** model);

	private:
		FbxModels();
		~FbxModels();

	private:
		void Open(wstring filename, FbxModel** model);
		void ProcessMaterial(void);
		void ProcessMesh(FbxNode* node);

	private:
		FbxModel* model;

		//Fbx Lib class
		FbxManager*				manager;
		FbxImporter*			importer;
		
		FbxScene*				scene;
		FbxGeometryConverter*	converter;
		//~Fbx Lib class

		vector<FbxSurfaceMaterial*> fbxMaterials;
	};
}