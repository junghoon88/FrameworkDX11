#pragma once

#include <Windows.h>
#include <assert.h>

//STL
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <functional>
#include <memory>	//shared_ptr<int> pTest(new int(10));
using namespace std;

//Direct3D
#include <dxgi1_2.h>
#include <d3dcommon.h>
#include <d3dcompiler.h>
#include <d3d11shader.h>
#include <d3d11.h>
#include <D3DX10math.h>
#include <D3DX11async.h>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3dx10.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

//ImGui
#include <imgui.h>
#include <imguiDx11.h>
#pragma comment(lib, "imgui.lib")

//FBX SDK
#define FBXSDK_SHARED
#include <fbxsdk.h>
#pragma comment(lib, "libfbxsdk.lib")
using namespace fbxsdk;



#define SAFE_RELEASE(p)			{ if(p) { p->Release(); p = NULL; } }
#define SAFE_DELETE(p)			{ if(p) { delete p; p = NULL; } }
#define SAFE_DELETE_ARRAY(p)	{ if(p) { delete[] p; p = NULL; } }

const wstring Assets    = L"../_Assets/";
const wstring Contents  = L"../_Contents/";
const wstring Textures  = L"../_Contents/Textures/";
const wstring BinModels = L"../_Contents/Models/";
const wstring FbxModels = L"../_Contents/FbxModels/";
const wstring Shaders   = L"./Shaders/";

#include "./Systems/D3D.h"
#include "./Systems/Time.h"
#include "./Systems/Mouse.h"
#include "./Systems/Keyboard.h"

#include "./Renders/VertexLayouts.h"
#include "./Renders/States.h"
#include "./Renders/Shader.h"
#include "./Renders/ShaderBuffer.h"
#include "./Renders/GlobalBuffers.h"
#include "./Renders/Textures.h"
#include "./Renders/Material.h"

#include "./Model/Component.h"
#include "./Model/BinModel.h"

#include "./Executes/GuiSettings.h"
#include "./Executes/Execute.h"

