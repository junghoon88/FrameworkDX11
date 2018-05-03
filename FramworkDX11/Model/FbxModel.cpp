#include "stdafx.h"
#include "FbxModel.h"
#include "FbxData.h"
#include "FbxUtility.h"

#include "BinModel.h"
#include "MeshPart.h"

#include "../Utilities/BinaryFile.h"
#include "../Utilities/String.h"
#include "../Utilities/Path.h"

const wstring Fbx::FbxModel::DefaultShader = Shaders + L"Mesh.hlsl";

Fbx::FbxModel::FbxModel(wstring filename)
{
	D3DXMatrixIdentity(&world);

	name = String::WStringToString(Path::GetFileNameWithoutExtension(filename));
}

Fbx::FbxModel::~FbxModel()
{
	for (MaterialData* data : materials)
		SAFE_DELETE(data);

	for (MeshData* data : meshes)
		SAFE_DELETE(data);
}

void Fbx::FbxModel::PushMaterialData(FbxSurfaceMaterial* material, int number)
{
	MaterialData* data = new MaterialData();

	data->Number = number;
	data->Name = material->GetName();

	wstring texture;
	string prop = "", factor = "";

	//Ambient
	prop = FbxSurfaceMaterial::sAmbient;
	factor = FbxSurfaceMaterial::sAmbientFactor;
	FbxUtility::GetMaterialProperty(&data->Ambient, &texture, material, prop, factor);

	//Diffuse
	prop = FbxSurfaceMaterial::sDiffuse;
	factor = FbxSurfaceMaterial::sDiffuseFactor;
	FbxUtility::GetMaterialProperty(&data->Diffuse, &texture, material, prop, factor);
	data->DiffuseFile = String::WStringToString(texture);

	//Specular
	prop = FbxSurfaceMaterial::sSpecular;
	factor = FbxSurfaceMaterial::sSpecularFactor;
	FbxUtility::GetMaterialProperty(&data->Specular, &texture, material, prop, factor);
	data->SpecularFile = String::WStringToString(texture);

	//Emissive
	prop = FbxSurfaceMaterial::sEmissive;
	factor = FbxSurfaceMaterial::sEmissiveFactor;
	FbxUtility::GetMaterialProperty(&data->Emissive, &texture, material, prop, factor);
	data->EmissiveFile = String::WStringToString(texture);

	//NormalMap
	prop = FbxSurfaceMaterial::sNormalMap;
	factor = "";
	FbxUtility::GetMaterialProperty(NULL, &texture, material, prop, factor);
	data->NormalFile = String::WStringToString(texture);

	FbxProperty shininessProp = material->FindProperty(FbxSurfaceMaterial::sShininess);
	if (shininessProp.IsValid())
		data->Shininess = (float)shininessProp.Get<FbxDouble>();

	materials.push_back(data);
}

void Fbx::FbxModel::Write(wstring file)
{
	BinaryWriter* w = new BinaryWriter();

	w->Open(file);
	{
		w->String(name);
		w->UInt(materials.size());
		for (MaterialData* data : materials)
			data->Write(w);

		w->UInt(meshes.size());
		for (MeshData* data : meshes)
			data->Write(w);
	}
	w->Close();

	SAFE_DELETE(w);
}

void Fbx::FbxModel::Write(wstring file, Model::BinModel* model)
{
	BinaryWriter* w = new BinaryWriter();

	w->Open(file);
	{
		w->String(model->name);

		w->UInt(model->meshParts.size()); 
		for (Model::MeshPart* part : model->meshParts)
		{
			MaterialData::Write(w, part->material);
		}

		w->UInt(model->meshParts.size());
		for (Model::MeshPart* part : model->meshParts)
		{
			MeshData::Write(w, part);
		}
	}
	w->Close();

	SAFE_DELETE(w);
}

void Fbx::FbxModel::Read(wstring file, Model::BinModel** model)
{
	BinaryReader* r = new BinaryReader();

	r->Open(file);
	{
		*model = new Model::BinModel();

		(*model)->name = r->String();

		UINT materialCount = r->UInt();
		vector<Material*> materials;
		materials.reserve(materialCount);

		for (UINT i = 0; i < materialCount; i++)
		{
			Material* material = NULL;
			MaterialData::Read(r, &material);
			materials.push_back(material);
		}

		UINT meshCount = r->UInt();
		for (UINT i = 0; i < meshCount; i++)
		{
			Model::MeshPart* part = NULL;
			MeshData::Read(r, &part);

			if (i < materialCount)
				part->material = materials[i];
			else
				part->material = new Material(DefaultShader);

			part->SetParentModel(*model);
			(*model)->meshParts.push_back(part);
		}
		
		(*model)->ScanPointMinMax();
		(*model)->boxCollider->CreateBuffer();
	}
	r->Close();

	SAFE_DELETE(r);
}
