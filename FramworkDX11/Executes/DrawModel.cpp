#include "stdafx.h"
#include "DrawModel.h"

#include "../Model/FbxModels.h"
#include "../Model/MeshPart.h"
#include "../Renders/Material.h"

#include "../Utilities/Path.h"
#include "../Utilities/String.h"

#include "rapidjson/document.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/reader.h"
#include "rapidjson/writer.h"

#include "../Command/Command.h"
#include "../Command/CommandDrawModel.h"

using namespace Model;
using namespace rapidjson;

#define JHKIM_TEST 0

DrawModel::DrawModel(ExecuteValues* values)
	: Execute(values)
	, selectedPart(NULL)
	, bSelectPart(false)
	, bMovedOld(false)
	, selectedFbxFile(L"")
	, hoverModel(NULL)
	, hoverModelPart(NULL)
{
	cmdManager = new CommandManager();
}

DrawModel::~DrawModel()
{
	SAFE_DELETE(cmdManager);

	for(BinModel* model : models)
		SAFE_DELETE(model);
}

void DrawModel::Update(void)
{
	if (buttonSaveJsonModels.Clicked)
	{
		//SaveJsonModels(L"");
		Path::SaveFileDialog(L"*.json", L"*.json", L"./", buttonSaveJsonModels.Func);
	}

	if (buttonLoadJsonModels.Clicked)
	{
		//LoadJsonModels(L"");
		Path::SaveFileDialog(L"*.json", L"*.json", L"./", buttonLoadJsonModels.Func);
	}

	UpdateKeyboard();
	UpdateMouse();
	UpdateCommands();


	if (selectedPart)
	{
		selectedPart->SetisSelected(true);
	}
	else
	{
		for (BinModel* model : selectedModels)
			model->SetisSelected(true);
	}

	if (hoverModel)
	{
		bool bFind = false;
		for (BinModel* model : selectedModels)
		{
			if (model == hoverModel)
			{
				bFind = true;
				break;
			}
		}

		if (bFind)	hoverModelPart->SetisHovering(true);
		else		hoverModel->SetisHovering(true);
	}

	for (BinModel* model : models)
		model->Update();
}

void DrawModel::Render(void)
{
	for (BinModel* model : models)
	{
		model->Render();
	}
}

void DrawModel::PostRender(void)
{
	PostRenderModelProp();
	//menubar
	PostRenderModelMenu();

	

#if JHKIM_TEST
	ImGui::Begin("Files");

	wstring path = L"";
	for (UINT i = 0; i < fbxFileListName.size(); i++)
	{
		if (path != Path::GetDirectoryName(fbxFileList[i]))
		{
			path = Path::GetDirectoryName(fbxFileList[i]);
			ImGui::Text(String::WStringToString(path).c_str());
		}
		if (ImGui::Button(String::WStringToString(fbxFileListName[i]).c_str()))
		{
			LoadModel(fbxFileList[i]);
		}
	}

	ImGui::End();
#endif

	
}

#if JHKIM_TEST
void DrawModel::SaveJsonModels(wstring filename)
{
	if (models.size() == 0)
		return;

	wstring wjson = L"{\n";
	WCHAR str[100] = L"";

	int i = 0;
	swprintf_s(str, L"\"modelnum\" : %d,\n", models.size());
	wjson += str;

	for (UINT i = 0; i < models.size(); i++)
	{
		swprintf_s(str, L"\"filename%d\" : \"%s\",\n", i, models[i]->GetFileName().c_str());
		wjson += str;

		swprintf_s(str, L"\"shadername%d\" : \"%s\",\n", i, models[i]->GetShaderName().c_str());
		wjson += str;

		swprintf_s(str, L"\"trScale%d\" : [%.3f, %.3f, %.3f],\n"
			, i
			, models[i]->GetTransformScaleRef().x
			, models[i]->GetTransformScaleRef().y
			, models[i]->GetTransformScaleRef().z);
		wjson += str;

		swprintf_s(str, L"\"trRotateDeg%d\" : [%.3f, %.3f, %.3f],\n"
			, i
			, models[i]->GetTransformRotateDegRef().x
			, models[i]->GetTransformRotateDegRef().y
			, models[i]->GetTransformRotateDegRef().z);
		wjson += str;

		swprintf_s(str, L"\"trPosition%d\" : [%.3f, %.3f, %.3f],\n"
			, i
			, models[i]->GetTransformPositionRef().x
			, models[i]->GetTransformPositionRef().y
			, models[i]->GetTransformPositionRef().z);
		wjson += str;

		swprintf_s(str, L"\"color%d\" : [%.3f, %.3f, %.3f],\n"
			, i
			, models[i]->GetColor().x
			, models[i]->GetColor().y
			, models[i]->GetColor().z);
		wjson += str;

		swprintf_s(str, L"\"bcCenter%d\" : [%.3f, %.3f, %.3f],\n"
			, i
			, models[i]->GetBoxCollider()->Center.x
			, models[i]->GetBoxCollider()->Center.y
			, models[i]->GetBoxCollider()->Center.z);
		wjson += str;

		swprintf_s(str, L"\"bcSize%d\" : [%.3f, %.3f, %.3f],\n"
			, i
			, models[i]->GetBoxCollider()->Size.x
			, models[i]->GetBoxCollider()->Size.y
			, models[i]->GetBoxCollider()->Size.z);
		wjson += str;
	}
	wjson += L"\"end\" : \"true\"";
	wjson += L"}\n";

	Document doc;

	string json = String::WStringToString(wjson);

	int len = json.length();
	assert(len < 65536);

	doc.Parse(json.c_str());
	FILE* fp;

	//fopen_s(&fp, "json/output.json", "wt");
	if (Path::GetExtension(filename) != L"json") filename += L".json";
	fopen_s(&fp, String::WStringToString(filename).c_str(), "wt");
	char writeBuffer[65536];
	FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
	Writer<FileWriteStream> writer(os);
	assert(doc.Accept(writer));
	fclose(fp);
}

void DrawModel::LoadJsonModels(wstring filename)
{
	for (FbxModel* model : models)
		SAFE_DELETE(model);
	models.clear();

	selectedModel = NULL;
	selectedPart = NULL;

	Document doc;
	FILE* fp = NULL;
	//fopen_s(&fp, "json/output.json", "rt");
	fopen_s(&fp, String::WStringToString(filename).c_str(), "rt");
	assert(fp != NULL);
	char readBuffer[65536];
	FileReadStream is(fp, readBuffer, sizeof(readBuffer));
	doc.ParseStream(is);
	fclose(fp);

	assert(doc["modelnum"].IsInt());
	int num = doc["modelnum"].GetInt();

	char strKey[100] = "";
	for (int i = 0; i < num; i++)
	{
		sprintf_s(strKey, "filename%d", i);
		assert(doc[strKey].IsString());
		wstring wsFilename = String::StringToWString(doc[strKey].GetString());

		sprintf_s(strKey, "shadername%d", i);
		assert(doc[strKey].IsString());
		wstring wsShadername = String::StringToWString(doc[strKey].GetString());
	
		sprintf_s(strKey, "trScale%d", i);
		assert(doc[strKey].IsArray());
		const Value& scale = doc[strKey];
		D3DXVECTOR3 transformScale;
		transformScale.x = (float)(scale[0].GetDouble());
		transformScale.y = (float)(scale[1].GetDouble());
		transformScale.z = (float)(scale[2].GetDouble());

		sprintf_s(strKey, "trRotateDeg%d", i);
		assert(doc[strKey].IsArray());
		const Value& rotDeg = doc[strKey];
		D3DXVECTOR3 transformRotateDeg;
		transformRotateDeg.x = (float)(rotDeg[0].GetDouble());
		transformRotateDeg.y = (float)(rotDeg[1].GetDouble());
		transformRotateDeg.z = (float)(rotDeg[2].GetDouble());

		sprintf_s(strKey, "trPosition%d", i);
		assert(doc[strKey].IsArray());
		const Value& pos = doc[strKey];
		D3DXVECTOR3 transformPosition;
		transformPosition.x = (float)(pos[0].GetDouble());
		transformPosition.y = (float)(pos[1].GetDouble());
		transformPosition.z = (float)(pos[2].GetDouble());

		sprintf_s(strKey, "color%d", i);
		assert(doc[strKey].IsArray());
		const Value& color = doc[strKey];
		D3DXVECTOR3 vecColor;
		vecColor.x = (float)(color[0].GetDouble());
		vecColor.y = (float)(color[1].GetDouble());
		vecColor.z = (float)(color[2].GetDouble());

		sprintf_s(strKey, "bcCenter%d", i);
		assert(doc[strKey].IsArray());
		const Value& center = doc[strKey];
		D3DXVECTOR3 bcCenter;
		bcCenter.x = (float)(center[0].GetDouble());
		bcCenter.y = (float)(center[1].GetDouble());
		bcCenter.z = (float)(center[2].GetDouble());

		sprintf_s(strKey, "bcSize%d", i);
		assert(doc[strKey].IsArray());
		const Value& size = doc[strKey];
		D3DXVECTOR3 bcSize;
		bcSize.x = (float)(size[0].GetDouble());
		bcSize.y = (float)(size[1].GetDouble());
		bcSize.z = (float)(size[2].GetDouble());

		FbxModel* pModel = new FbxModel(wsFilename, wsShadername);
		pModel->SetTransformScale(transformScale);
		pModel->SetTransformRotateDeg(transformRotateDeg);
		pModel->SetTransformPosition(transformPosition);
		pModel->SetColor(vecColor);
		pModel->SetBoxColliderCenter(bcCenter);
		pModel->SetBoxColliderSize(bcSize);
		pModel->GetBoxCollider()->remakeVertex();
		models.push_back(pModel);

		cmdManager->ClearCommands();
	}
}


void DrawModel::LoadModel(wstring filename)
{
	cmdManager->AddCommand(new CommandModelLoad(models, new FbxModel(filename, Shaders + L"Mesh.hlsl")));

	selectedModel = models.back();
}


void DrawModel::loadPaths(void)
{
	//모든 파일 스캔 test
	WIN32_FIND_DATA wfd;
	HANDLE handle = NULL;

	paths.push_back(Assets);
	UINT count = 0;

	
	while (count < paths.size())
	{
		wstring findfile = paths[count] + wstring(L"*.");
		handle = FindFirstFile(findfile.c_str(), &wfd);
		// 찾는 파일이 있다면,
		while (handle != INVALID_HANDLE_VALUE)
		{
			if (wfd.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
			{
				if (wcscmp(wfd.cFileName, L".") != 0 && wcscmp(wfd.cFileName, L"..") != 0)
				{
					paths.push_back(paths[count] + wfd.cFileName + L"/");
				}
			}
			else
			{
				fbxFileList.push_back(paths[count] + wstring(wfd.cFileName));
				fbxFileListName.push_back(Path::GetFileNameWithoutExtension(wfd.cFileName));
			}

			// FindNextFile 리턴 값으로 다음값이 있을 경우 TRUE
			// 없을 경우 FALSE 값 리턴
			if (!FindNextFile(handle, &wfd))
			{
				break;
			}
		}
		// 파일 찾기 핸들 값 닫기   
		FindClose(handle);

		count++;
	}
}

void DrawModel::loadFbxFileLists(void)
{
	//모든 파일 스캔 test
	WIN32_FIND_DATA wfd;
	HANDLE handle = NULL;

	for (UINT i = 0; i < paths.size(); i++)
	{
		wstring findfile = paths[i] + wstring(L"*.fbx");
		handle = FindFirstFile(findfile.c_str(), &wfd);
		// 찾는 파일이 있다면,
		while (handle != INVALID_HANDLE_VALUE)
		{
			if (wfd.dwFileAttributes == FILE_ATTRIBUTE_ARCHIVE)
			{
				fbxFileList.push_back(paths[i] + wstring(wfd.cFileName));
				fbxFileListName.push_back(Path::GetFileNameWithoutExtension(wfd.cFileName));
			}

			// FindNextFile 리턴 값으로 다음값이 있을 경우 TRUE
			// 없을 경우 FALSE 값 리턴
			if (!FindNextFile(handle, &wfd))
			{
				break;
			}
		}
		// 파일 찾기 핸들 값 닫기   
		FindClose(handle);
	}

	printf("");
}

#endif

void DrawModel::UpdateKeyboard(void)
{
	float speed = Time::Delta() * 3.0f;
	char cMoved = 0;

	if (selectedModels.size() > 0)
	{
		//x
		if (gKeyboard->IsPress(VK_NUMPAD6))
		{
			cMoved |= 0x01;
		}
		if (gKeyboard->IsPress(VK_NUMPAD4))
		{
			cMoved |= 0x02;
		}
		//z
		if (gKeyboard->IsPress(VK_NUMPAD9))
		{
			cMoved |= 0x10;
		}
		if (gKeyboard->IsPress(VK_NUMPAD1))
		{
			cMoved |= 0x20;
		}
		//y
		if (gKeyboard->IsPress(VK_NUMPAD8))
		{
			cMoved |= 0x04;
		}
		if (gKeyboard->IsPress(VK_NUMPAD2))
		{
			cMoved |= 0x08;
		}

		if (cMoved != 0)
		{
			D3DXVECTOR3 vec = D3DXVECTOR3(0, 0, 0);

			//x
			if		((cMoved & 0x03) == 0x01) vec.x =  speed;
			else if ((cMoved & 0x03) == 0x02) vec.x = -speed;
			//y
			if		((cMoved & 0x0c) == 0x04) vec.y =  speed;
			else if ((cMoved & 0x0c) == 0x08) vec.y = -speed;
			//z
			if		((cMoved & 0x30) == 0x10) vec.z =  speed;
			else if ((cMoved & 0x30) == 0x20) vec.z = -speed;

			if (selectedPart)
			{
				vecTransformOld.clear();
				vecTransformOld.resize(1);
				if (bMovedOld == false)
				{
					vecTransformOld[0] = selectedPart->GetTransform();
				}
				selectedPart->SetTransformTranslate(vec);
			}
			else
			{
				vecTransformOld.clear();
				vecTransformOld.resize(selectedModels.size());
				if (bMovedOld == false)
				{
					for (UINT i = 0; i < selectedModels.size(); i++)
						vecTransformOld[i] = selectedModels[i]->GetTransform();
				}
				for (BinModel* model : selectedModels)
					model->SetTransformTranslate(vec);
			}
		}
		else if (bMovedOld == true)
		{
			if (selectedPart)
			{
				cmdManager->AddCommand(new CommandMeshTransform(selectedPart, vecTransformOld[0], selectedPart->GetTransform()));
				vecTransformOld.clear();
			}
			else
			{
				cmdManager->AddCommand(new CommandModelTransform(selectedModels, vecTransformOld));
				vecTransformOld.clear();
			}
		}

		bMovedOld = (cMoved == 0) ? false : true;
	}
	else
	{
		bMovedOld = false;
	}
}

void DrawModel::UpdateMouse(void)
{
	D3DXVECTOR3 start;
	D3DXVECTOR3 direction;
	values->MainCamera->GetPosition(&start);
	direction = values->MainCamera->GetDirection(values->Viewport, values->Perspective);

	float dist = 0.0f;
	//map<float, FbxModel*> picks;
	map<float, MeshPart*> picks;
	map<float, MeshPart*> pickkedParts;

	for (UINT i = 0; i < models.size(); i++)
	{
		if (models[i]->MousePickked(pickkedParts, start, direction))
		{
			//picks.insert(make_pair(dist, models[i]));
			auto iter = pickkedParts.begin();
			for (; iter != pickkedParts.end();)
			{
				picks[(*iter).first] = (*iter).second;
				iter = pickkedParts.erase(iter);
			}
			pickkedParts.clear();
		}
	}

	if (picks.size() > 0)
	{
		hoverModelPart = (*picks.begin()).second;
		hoverModel = hoverModelPart->GetParentModel();

		if(selectedModels.size() == 1 && selectedModels[0] == hoverModel)
			bSelectPart = true;
		else
			bSelectPart = false;


		if (gMouse->IsDown(0))
		{
			if (bSelectPart)
			{
				selectedPart = hoverModelPart;


				selectedColor.BufferColor	= *selectedPart->GetColor();
				if (selectedPart->GetMaterial())
				{
					Material* material = selectedPart->GetMaterial();
					selectedColor.Ambient		= *material->GetAmbient();
					selectedColor.Diffuse		= *material->GetDiffuse();
					selectedColor.Specular		= *material->GetSpecular();
					selectedColor.Emissive		= *material->GetEmissive();
					selectedColor.Shininess		= *material->GetShininess();
					selectedColor.DetailFactor	= *material->GetDetailFactor();
				}






			}
			else
			{
				if (gKeyboard->IsPress(VK_CONTROL) || gKeyboard->IsPress(VK_SHIFT))
				{
					bool bFind = false;
					for (UINT i = 0; i < selectedModels.size(); i++)
					{
						if (selectedModels[i] == hoverModel)
						{
							selectedModels.erase(selectedModels.begin() + i);
							bFind = true;
							break;
						}
					}
					if (!bFind)
					{
						selectedModels.push_back(hoverModel);
					}
				}
				else
				{
					selectedModels.clear();
					selectedModels.push_back(hoverModel);
				}
				selectedPart = NULL;
			}
		}
	}
	else
	{
		hoverModelPart = NULL;
		hoverModel = NULL;
		//if (gMouse->IsDown(0))
		//{
		//	selectedModels.clear();
		//	selectedPart = NULL;
		//}
	}


	while (picks.size() > 0)
		picks.erase(picks.begin());
	picks.clear();
}

void DrawModel::UpdateCommands(void)
{
	//복사
	if (gKeyboard->IsDown('V') && gKeyboard->IsPress(VK_CONTROL))
	{
		if(selectedModels.size() > 0)
			cmdManager->AddCommand(new CommandModelCopy(models, selectedModels));
	}

	//뒤로가기
	if (gKeyboard->IsDown('Z') && gKeyboard->IsPress(VK_CONTROL))
	{
		cmdManager->UndoCommand();

		//bool bFind = false;
		//for (BinModel* model : models)
		//{
		//	if (model == selectedModel)
		//	{
		//		bFind = true;
		//		break;
		//	}
		//}
		//if (!bFind) selectedModel = NULL;
		selectedModels.clear();
	}

	//되돌리기
	if (gKeyboard->IsDown('Y') && gKeyboard->IsPress(VK_CONTROL))
	{
		cmdManager->RedoCommand();

		//bool bFind = false;
		//for (BinModel* model : models)
		//{
		//	if (model == selectedModel)
		//	{
		//		bFind = true;
		//		break;
		//	}
		//}
		//if (!bFind) selectedModel = NULL;
		selectedModels.clear();
	}

	//삭제
	if (gKeyboard->IsDown(VK_DELETE))
	{
		if (selectedModels.size() > 0)
		{
			cmdManager->AddCommand(new CommandModelDelete(models, selectedModels));

			selectedModels.clear();
			selectedPart = NULL;
		}
	}

}

void DrawModel::PostRenderModelProp(void)
{
	if (ImGui::Begin("Model"))
	{
		//Models
		ImGui::Text("Models");
		for (UINT i = 0; i < models.size(); i++)
		{
			char str[10];
			sprintf_s(str, "%d : ", i + 1);
			string name = str + Path::GetFileNameWithoutExtension(models[i]->GetFileName());
			if (ImGui::Button(name.c_str()))
			{
				//버튼 클릭시
				BinModel* pModel = models[i];
				if (gKeyboard->IsPress(VK_CONTROL) || gKeyboard->IsPress(VK_SHIFT))
				{
					bool bFind = false;
					for (UINT j = 0; j < selectedModels.size(); j++)
					{
						if (selectedModels[j] == pModel)
						{
							selectedModels.erase(selectedModels.begin() + j);
							bFind = true;
							break;
						}
					}
					if (!bFind)
					{
						selectedModels.push_back(pModel);
					}
				}
				else
				{
					selectedModels.clear();
					selectedModels.push_back(pModel);
				}
				selectedPart = NULL;
			}
		}

		ImGui::Separator();

		//selectedModel
		if (selectedModels.size() > 0)
		{
			for (BinModel* model : selectedModels)
			{
				string name = "Selected Model : " + Path::GetFileNameWithoutExtension(model->GetFileName());
				ImGui::Text(name.c_str());
			}

			if (selectedPart)
			{
				string name = "Selected Part : " + Path::GetFileNameWithoutExtension(selectedPart->GetName());
				ImGui::Text(name.c_str());

				TRANSFORM transform = selectedPart->GetTransform();
				//D3DXVECTOR3 boxCenter = selectedModel->GetBoxCollider()->Center;
				//D3DXVECTOR3 boxSize = selectedModel->GetBoxCollider()->Size;

				//Transform
				ImGui::Text("TRANSFORM");
				if (ImGui::InputFloat3("Scale", transform.Scale)
					|| ImGui::InputFloat3("Rotate", transform.RotationDeg)
					|| ImGui::InputFloat3("Position", transform.Position))
				{
					cmdManager->AddCommand(new CommandMeshTransform(selectedPart, selectedPart->GetTransform(), transform));
				}
				ImGui::Separator();

				//Color
				ImGui::Text("Material");
				D3DXCOLOR* bufferColor	= selectedPart->GetColor();
				Material*  material		= selectedPart->GetMaterial();
				D3DXCOLOR* ambient		= material->GetAmbient();
				D3DXCOLOR* diffuse		= material->GetDiffuse();
				D3DXCOLOR* specular		= material->GetSpecular();
				D3DXCOLOR* emissive		= material->GetEmissive();
				float*     shininess	= material->GetShininess();
				float*     detailFactor = material->GetDetailFactor();

				if (ImGui::ColorEdit4("Mesh Buffer Color", bufferColor[0], ImGuiColorEditFlags_::ImGuiColorEditFlags_NoInputs) == false)
				{
					if (ImGui::IsMouseReleased(0))
					{
						cmdManager->AddCommand(new CommandMeshColor(selectedPart, MaterialColorType::BufferColor, selectedColor.BufferColor, *bufferColor));
						selectedColor.BufferColor = *bufferColor;
					}
				}

				if (ImGui::ColorEdit4("Ambient", ambient[0], ImGuiColorEditFlags_::ImGuiColorEditFlags_NoInputs) == false)
				{
					if (ImGui::IsMouseReleased(0))
					{
						cmdManager->AddCommand(new CommandMeshColor(selectedPart, MaterialColorType::Ambient, selectedColor.Ambient, *ambient));
						selectedColor.Ambient = *ambient;
					}
				}

				if (ImGui::ColorEdit4("Diffuse", diffuse[0], ImGuiColorEditFlags_::ImGuiColorEditFlags_NoInputs) == false)
				{
					if (ImGui::IsMouseReleased(0))
					{
						cmdManager->AddCommand(new CommandMeshColor(selectedPart, MaterialColorType::Diffuse, selectedColor.Diffuse, *diffuse));
						selectedColor.Diffuse = *diffuse;
					}
				}
				if (ImGui::ColorEdit4("Specular", specular[0], ImGuiColorEditFlags_::ImGuiColorEditFlags_NoInputs) == false)
				{
					if (ImGui::IsMouseReleased(0))
					{
						cmdManager->AddCommand(new CommandMeshColor(selectedPart, MaterialColorType::Specular, selectedColor.Specular, *specular));
						selectedColor.Specular = *specular;
					}
				}

				if (ImGui::ColorEdit4("Emissive", emissive[0], ImGuiColorEditFlags_::ImGuiColorEditFlags_NoInputs) == false)
				{
					if (ImGui::IsMouseReleased(0))
					{
						cmdManager->AddCommand(new CommandMeshColor(selectedPart, MaterialColorType::Emissive, selectedColor.Emissive, *emissive));
						selectedColor.Emissive = *emissive;
					}
				}

				if (ImGui::SliderFloat("Shininess", shininess, 0.0f, 100.0f) == false)
				{
					if (ImGui::IsMouseReleased(0))
					{
						cmdManager->AddCommand(new CommandMeshColor(selectedPart, MaterialColorType::Shininess, selectedColor.Shininess, *shininess));
						selectedColor.Shininess = *shininess;
					}
				}

				if (ImGui::SliderFloat("Detail Factor", detailFactor, 0.0f, 1.0f) == false)
				{
					if (ImGui::IsMouseReleased(0))
					{
						cmdManager->AddCommand(new CommandMeshColor(selectedPart, MaterialColorType::DetailFactor, selectedColor.DetailFactor, *detailFactor));
						selectedColor.DetailFactor = *detailFactor;
					}
				}



				string diffuseMapFile	= material->GetDiffuseMap()		== NULL ? "" : Path::GetFileNameWithoutExtension(String::WStringToString(material->GetDiffuseMap()->GetFile()));
				string specularMapFile	= material->GetSpecularMap()	== NULL ? "" : Path::GetFileNameWithoutExtension(String::WStringToString(material->GetSpecularMap()->GetFile()));
				string emissiveMapFile	= material->GetEmissiveMap()	== NULL ? "" : Path::GetFileNameWithoutExtension(String::WStringToString(material->GetEmissiveMap()->GetFile()));
				string normalMapFile	= material->GetNormalMap()		== NULL ? "" : Path::GetFileNameWithoutExtension(String::WStringToString(material->GetNormalMap()->GetFile()));
				string shaderFile		= material->GetShader()			== NULL ? "" : Path::GetFileNameWithoutExtension(String::WStringToString(material->GetShader()->GetFile()));

				if (ImGui::Button(string("DiffuseMap : " + diffuseMapFile).c_str()))
				{
					//material->SetDiffuseMap();
				}
				if (ImGui::Button(string("SpecularMap : " + specularMapFile).c_str()))
				{
				}
				if (ImGui::Button(string("EmissiveMap : " + emissiveMapFile).c_str()))
				{
				}
				if (ImGui::Button(string("NormalMap : " + normalMapFile).c_str()))
				{
				}

				if (ImGui::Button(string("Shader : " + shaderFile).c_str()))
				{
					D3DDesc desc;
					D3D::GetDesc(&desc);
					function<void(wstring)> func = bind(&DrawModel::OpenMaterialFile, this, material, placeholders::_1, MaterialFileType::Shader);
					Path::OpenFileDialog(L"", Path::ShaderFilter, Shaders, func, desc.Handle);
				}
			}
			else
			{
				//D3DXVECTOR3 boxCenter = selectedModel->GetBoxCollider()->Center;
				//D3DXVECTOR3 boxSize = selectedModel->GetBoxCollider()->Size;

				if (selectedModels.size() == 1)
				{
					TRANSFORM transform = selectedModels[0]->GetTransform();

					//Transform
					if (ImGui::InputFloat3("Scale", transform.Scale)
						|| ImGui::InputFloat3("Rotate", transform.RotationDeg)
						|| ImGui::InputFloat3("Position", transform.Position))
					{
						cmdManager->AddCommand(new CommandModelTransform(selectedModels[0], transform));
					}
					ImGui::Separator();
				}
			}
		}



		//ImGui::Separator();

		//buttonSaveJsonModels.Clicked = ImGui::Button("SaveJsonModels");
		//buttonLoadJsonModels.Clicked = ImGui::Button("LoadJsonModels");
		ImGui::End();
	}
}

void DrawModel::PostRenderModelMenu(void)
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Model"))
		{
			if (ImGui::MenuItem("Convert", ""))
			{
				OpenConvertDialog();
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Open", ""))
			{
				OpenModelDialog();
			}

			if (ImGui::MenuItem("Save", "") && selectedModels.size() > 0)
			{
				for (BinModel* model : selectedModels)
				{
					D3DDesc desc;
					D3D::GetDesc(&desc);

					function<void(wstring)> func = bind(&DrawModel::SaveModelDialog, this, placeholders::_1, model);
					Path::SaveFileDialog(L"", Path::BinModelFilter, BinModels, func, desc.Handle);
				}
			}

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}


void DrawModel::OpenConvertDialog(wstring file)
{
	D3DDesc desc;
	D3D::GetDesc(&desc);

	if (file.length() == 0)
	{
		function<void(wstring)> f = bind(&DrawModel::OpenConvertDialog, this, placeholders::_1);
		Path::OpenFileDialog(L"", Path::FbxModelFilter, Assets, f, desc.Handle);
	}
	else
	{
		selectedFbxFile = file;
		wstring fileName = Path::GetFileNameWithoutExtension(file);
		function<void(wstring)> f = bind(&DrawModel::Convert, this, placeholders::_1);
		Path::SaveFileDialog(fileName, Path::BinModelFilter, BinModels, f, desc.Handle);
		selectedFbxFile = L"";
	}
}

void DrawModel::Convert(wstring file)
{
	if (file.length() == 0) return;
	if (selectedFbxFile.length() == 0) return;

	wstring saveFile = file + L".model";
	//wstring saveFile = Path::GetFileNameWithoutExtension(file) + L".model";

	Fbx::FbxModels::Convert(selectedFbxFile, saveFile);

	D3DDesc desc;
	D3D::GetDesc(&desc);

	MessageBox(desc.Handle, L"DrawModel::Convert", L"Success", MB_OK);
}

void DrawModel::OpenModelDialog(wstring file)
{
	if (file.length() == 0)
	{
		D3DDesc desc;
		D3D::GetDesc(&desc);

		function<void(wstring)> f = bind(&DrawModel::OpenModelDialog, this, placeholders::_1);
		Path::OpenFileDialog(L"", Path::BinModelFilter, BinModels, f, desc.Handle);
	}
	else
	{
		BinModel* model = NULL;
		Fbx::FbxModels::ReadBinary(file, &model);

		cmdManager->AddCommand(new CommandModelLoad(models, model));
	}
}

void DrawModel::SaveModelDialog(wstring file, BinModel* model)
{
	model->SetFileName(String::WStringToString(Path::GetFileNameWithoutExtension(file)));
	Fbx::FbxModels::WriteBinary(file, model);
}

void DrawModel::OpenMaterialFile(Material* material, wstring File, MaterialFileType type)
{
	wstring oldFile = L"";
	switch (type)
	{
	case MaterialFileType::DiffuseMap:
		oldFile = material->GetDiffuseMap() == NULL ? L"" : material->GetDiffuseMap()->GetFile();
		break;
	case MaterialFileType::SpecularMap:
		oldFile = material->GetSpecularMap() == NULL ? L"" : material->GetSpecularMap()->GetFile();
		break;
	case MaterialFileType::EmissiveMap:
		oldFile = material->GetEmissiveMap() == NULL ? L"" : material->GetEmissiveMap()->GetFile();
		break;
	case MaterialFileType::NormalMap:
		oldFile = material->GetNormalMap() == NULL ? L"" : material->GetNormalMap()->GetFile();
		break;
	case MaterialFileType::Shader:
		oldFile = material->GetShader() == NULL ? L"" : material->GetShader()->GetFile();
		break;
	default:
		assert(false);
		break;
	}

	cmdManager->AddCommand(new CommandMaterialOpenFile(material, type, oldFile, File));

}
