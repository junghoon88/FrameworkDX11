#include "stdafx.h"
#include "FbxData.h"
#include "FbxModel.h"

#include "../Utilities/BinaryFile.h"
#include "../Utilities/String.h"
#include "../Utilities/Path.h"

#include "MeshPart.h"


void Fbx::MaterialData::Write(BinaryWriter* w)
{
	assert(w != NULL);

	w->UInt(Number);
	w->String(Name);
	
	w->Color4f(Ambient);
	w->Color4f(Diffuse);
	w->Color4f(Specular);
	w->Color4f(Emissive);

	w->Float(Shininess);
	w->Float(DetailFactor);

	w->String(ShaderFile);
	w->String(DiffuseFile);
	w->String(SpecularFile);
	w->String(EmissiveFile);
	w->String(NormalFile);
	w->String(DetailFile);
}

void Fbx::MaterialData::Write(BinaryWriter* w, Material* material)
{
	assert(w != NULL);
	assert(material != NULL);

	MaterialData data;
	
	data.Number = material->GetNumber();
	data.Name = String::WStringToString(material->GetName());

	data.Ambient  = *(material->GetAmbient());
	data.Diffuse  = *(material->GetDiffuse());
	data.Specular = *(material->GetSpecular());
	data.Emissive = *(material->GetEmissive());

	data.Shininess    = *(material->GetShininess());
	data.DetailFactor = *(material->GetDetailFactor());

	data.ShaderFile   = material->GetShader()		== NULL ? "" : String::WStringToString(material->GetShader()->GetFile());
	data.DiffuseFile  = material->GetDiffuseMap()	== NULL ? "" : String::WStringToString(material->GetDiffuseMap()->GetFile());
	data.SpecularFile = material->GetSpecularMap()	== NULL ? "" : String::WStringToString(material->GetSpecularMap()->GetFile());
	data.EmissiveFile = material->GetEmissiveMap()	== NULL ? "" : String::WStringToString(material->GetEmissiveMap()->GetFile());
	data.NormalFile   = material->GetNormalMap()	== NULL ? "" : String::WStringToString(material->GetNormalMap()->GetFile());
	data.DetailFile   = material->GetDetailMap()	== NULL ? "" : String::WStringToString(material->GetDetailMap()->GetFile());

	data.Write(w);
}

void Fbx::MaterialData::Read(BinaryReader* r, Material** material)
{
	assert(r != NULL);

	MaterialData data;
	data.Number = r->UInt();
	data.Name = r->String();

	data.Ambient  = r->Color4f();
	data.Diffuse  = r->Color4f();
	data.Specular = r->Color4f();
	data.Emissive = r->Color4f();

	data.Shininess    = r->Float();
	data.DetailFactor = r->Float();

	data.ShaderFile   = r->String();
	data.DiffuseFile  = r->String();
	data.SpecularFile = r->String();
	data.EmissiveFile = r->String();
	data.NormalFile   = r->String();
	data.DetailFile   = r->String();

	wstring shaderFile = L"";
	if (data.ShaderFile.size() == 0)
		shaderFile = FbxModel::DefaultShader;
	else
		shaderFile = String::StringToWString(data.ShaderFile);

	*material = new Material(shaderFile);

	(*material)->SetNumber(data.Number);
	(*material)->SetName(String::StringToWString(data.Name));

	(*material)->SetAmbient(data.Ambient);
	(*material)->SetDiffuse(data.Diffuse);
	(*material)->SetSpecular(data.Specular);
	(*material)->SetEmissive(data.Emissive);

	(*material)->SetShininess(data.Shininess);
	(*material)->SetDetailFactor(data.DetailFactor);

	(*material)->SetDiffuseMap(data.DiffuseFile);
	(*material)->SetSpecularMap(data.SpecularFile);
	(*material)->SetEmissiveMap(data.EmissiveFile);
	(*material)->SetNormalMap(data.NormalFile);
	(*material)->SetDetailMap(data.DetailFile);
}

void Fbx::MeshData::Write(BinaryWriter* w)
{
	w->String(Name);
	w->Matrix(World);

	w->UInt(Vertices.size());
	w->Byte(&Vertices[0], sizeof(VertexTextureNormal) * Vertices.size());

	w->UInt(Indices.size());
	w->Byte(&Indices[0], sizeof(UINT) * Indices.size());
}

void Fbx::MeshData::Write(BinaryWriter* w, Model::MeshPart* meshPart)
{
	MeshData data;
	data.Name = meshPart->name;

	data.World = meshPart->world;

	data.Vertices.clear();
	data.Vertices.assign(meshPart->vertexData.begin(), meshPart->vertexData.end());

	data.Indices.clear();
	data.Indices.assign(meshPart->indexData.begin(), meshPart->indexData.end());

	data.Write(w);
}

void Fbx::MeshData::Read(BinaryReader* r, Model::MeshPart** meshPart)
{
	assert(r != NULL);

	MeshData data;

	data.Name = r->String();
	data.World = r->Matrix();

	//Vertex
	{
		UINT count = r->UInt();
		assert(count > 0);		
		//초기화
		data.Vertices.assign(count, VertexTextureNormal());

		void* ptr = (void*)&(data.Vertices[0]);
		r->Byte(&ptr, sizeof(VertexTextureNormal) * count);
	}

	//Index
	{
		UINT count = r->UInt();
		assert(count > 0);
		//초기화
		data.Indices.assign(count, UINT());

		void* ptr = (void*)&(data.Indices[0]);
		r->Byte(&ptr, sizeof(UINT) * count);
	}

	*meshPart = new Model::MeshPart(data);
}
