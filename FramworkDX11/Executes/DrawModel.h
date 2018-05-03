#pragma once

namespace Model { class MeshPart; class BinModel; }

class CommandManager;

class DrawModel : public Execute
{
public:
	DrawModel(ExecuteValues* values);
	~DrawModel();

	void Update(void);

	void PreRender(void) {}
	void Render(void);
	void PostRender(void);

	void ResizeScreen(void) {}

private:
	void LoadModel(wstring filename);
	void SaveJsonModels(wstring filename);
	void LoadJsonModels(wstring filename);

	void loadPaths(void);
	void loadFbxFileLists(void);
	
	void UpdateKeyboard(void);
	void UpdateMouse(void);
	void UpdateCommands(void);

	void PostRenderModelProp(void);
	void PostRenderModelMenu(void);


	void OpenConvertDialog(wstring file = L"");
	void Convert(wstring file);
	void OpenModelDialog(wstring file = L"");
	void SaveModelDialog(wstring file, Model::BinModel* model);

	void OpenMaterialFile(Material* material, wstring File, enum class MaterialFileType type);

private:
	vector<Model::BinModel*>	models;
	vector<Model::BinModel*>	selectedModels;
	Model::MeshPart*			selectedPart;
	Model::BinModel*			hoverModel;
	Model::MeshPart*			hoverModelPart;
	bool						bSelectPart;

	struct MeshColorBackup
	{
		D3DXCOLOR BufferColor;
		D3DXCOLOR Ambient;
		D3DXCOLOR Diffuse;
		D3DXCOLOR Specular;
		D3DXCOLOR Emissive;
		float Shininess;
		float DetailFactor;

		MeshColorBackup()
			: BufferColor(1, 1, 1, 1)
			, Ambient(0, 0, 0, 0)
			, Diffuse(0, 0, 0, 0)
			, Specular(0, 0, 0, 0)
			, Emissive(0, 0, 0, 0)
			, Shininess(0.0f)
			, DetailFactor(0.0f)
		{
		}
	};

	MeshColorBackup				selectedColor; //Color 변경 history 기록용

	bool						bMovedOld;
	vector<TRANSFORM>			vecTransformOld;


	struct button
	{
		bool Clicked;
		function<void(wstring)> Func;

		button() : Clicked(false) {}
	};

	button			buttonSaveJsonModels;
	button			buttonLoadJsonModels;

	vector<wstring> paths;
	//FBX 이름
	vector<wstring> fbxFileListName;
	//FBX 실제 경로
	vector<wstring> fbxFileList;

	//Command
	CommandManager* cmdManager;

	wstring			selectedFbxFile;
};



