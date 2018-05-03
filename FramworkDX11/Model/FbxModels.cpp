#include "stdafx.h"
#include "FbxModels.h"
#include "FbxModel.h"
#include "FbxUtility.h"
#include "FbxData.h"

#include "../Utilities/String.h"

void Fbx::FbxModels::Convert(wstring fbxFile, wstring saveFile)
{
	FbxModel* fbxModel = NULL;
	FbxModels* models = new FbxModels;
	
	models->Open(fbxFile, &fbxModel);
	
	SAFE_DELETE(models);

	fbxModel->Write(saveFile);
	SAFE_DELETE(fbxModel);
}

void Fbx::FbxModels::WriteBinary(wstring binFile, Model::BinModel* model)
{
	FbxModel::Write(binFile, model);
}

void Fbx::FbxModels::ReadBinary(wstring binFile, Model::BinModel** model)
{
	FbxModel::Read(binFile, model);
}

Fbx::FbxModels::FbxModels()
{
	manager = FbxManager::Create();
	scene = FbxScene::Create(manager, "");
}

Fbx::FbxModels::~FbxModels()
{
	scene->Destroy();
	manager->Destroy();
}

void Fbx::FbxModels::Open(wstring filename, FbxModel** model)
{
	FbxIOSettings* ios = FbxIOSettings::Create(manager, IOSROOT);
	ios->SetBoolProp(IMP_FBX_TEXTURE, true); //fbx 내에서 압축되어있는 부분을 해제
	manager->SetIOSettings(ios);

	importer = FbxImporter::Create(manager, "");

	bool bCheck = importer->Initialize(String::WStringToString(filename).c_str(), -1, ios);
	assert(bCheck == true);

	importer->Import(scene);
	converter = new FbxGeometryConverter(manager); //삼각형 형태로 바꾸기
	{
		*model = new FbxModel(filename);
		this->model = *model;

		ProcessMaterial();
		ProcessMesh(scene->GetRootNode());

		ios->Destroy();
	}
	SAFE_DELETE(converter);
	importer->Destroy();
}

void Fbx::FbxModels::ProcessMaterial(void)
{
	for (int i = 0; i < scene->GetMaterialCount(); i++)
	{
		FbxSurfaceMaterial* material = scene->GetMaterial(i);
		fbxMaterials.push_back(material);

		model->PushMaterialData(material, i);
	}
}

void Fbx::FbxModels::ProcessMesh(FbxNode * node)
{
	//노드를 돌면서 매쉬정보만 불러온다.
	FbxNodeAttribute* attribute = node->GetNodeAttribute();

	if (attribute != NULL)
	{
		FbxNodeAttribute::EType nodeType = attribute->GetAttributeType();
		
		if (nodeType == FbxNodeAttribute::eMesh)
		{
			converter->Triangulate(attribute, true, true); //legacy = false 면 복잡함..

			FbxMesh* mesh = node->GetMesh();

			MeshData* meshData = new MeshData();
			meshData->Name = node->GetName();
			
			//Control Point 위치정보(겹치는 위치가 없음)
			int controlPointCount = mesh->GetControlPointsCount();
			FbxVector4* controlPoints = mesh->GetControlPoints();

			//삼각형
			for (int ti = 0; ti < mesh->GetPolygonCount(); ti++)
			{
				//삼각형 버텍스
				for (int vi = 0; vi < 3; vi++)
				{
					int vertexIndex = mesh->GetPolygonVertex(ti, vi);
					if (vertexIndex < 0 || vertexIndex >= controlPointCount) continue;

					VertexTextureNormal vertex;
					vertex.position = FbxUtility::GetPosition(controlPoints[vertexIndex]);
					vertex.uv = FbxUtility::GetUV(mesh, 0, ti, vi, vertexIndex);
					//vertex.normal = FbxUtility::GetNormal(mesh, ti, vertexIndex);
					vertex.normal = FbxUtility::GetNormal(mesh, ti, vi);

					meshData->Vertices.push_back(vertex);
					//TODO : 인덱스를 받도록 변경해야함.
					meshData->Indices.push_back(meshData->Indices.size());
				}
			}

			meshData->World = FbxUtility::GetGeometricOffset(node);
			model->meshes.push_back(meshData);
		}
	}

	//재귀
	for (int i = 0; i < node->GetChildCount(); i++)
	{
		ProcessMesh(node->GetChild(i));
	}
}


