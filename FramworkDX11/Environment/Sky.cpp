#include "stdafx.h"
#include "Sky.h"
#include "../Model/FbxModels.h"
#include "../Model/MeshPart.h"


Environment::Sky::Sky(ExecuteValues* values)
	: modelFile(BinModels + L"Meshes/Sphere.model")
	, shaderFile(Shaders + L"Sky.hlsl")
	, values(values)
{
	Fbx::FbxModels::ReadBinary(modelFile, &model);

	assert(model->GetMeshPartSize() > 0);
	Material* material = model->GetMeshPart(0)->GetMaterial();
	material->SetShader(shaderFile);

	buffer = new Buffer();

	{
		D3D11_RASTERIZER_DESC desc;
		States::GetRasterizerDesc(&desc);
		States::CreateRasterizer(&desc, &rasterizer[0]);

		desc.FrontCounterClockwise = true;
		States::CreateRasterizer(&desc, &rasterizer[1]);
	}

	{
		D3D11_DEPTH_STENCIL_DESC desc;
		States::GetDepthStencilDesc(&desc);
		States::CreateDepthStencil(&desc, &depthStencilState[0]);

		desc.DepthEnable = false;
		States::CreateDepthStencil(&desc, &depthStencilState[1]);
	}
}

Environment::Sky::~Sky()
{
	SAFE_DELETE(model);

	SAFE_DELETE(buffer);

	SAFE_RELEASE(rasterizer[0]);
	SAFE_RELEASE(rasterizer[1]);

	SAFE_RELEASE(depthStencilState[0]);
	SAFE_RELEASE(depthStencilState[1]);
}

void Environment::Sky::Update(void)
{
	//항상 카메라를 따라다니도록
	D3DXVECTOR3 position;
	values->MainCamera->GetPosition(&position);

	D3DXMATRIX world;
	D3DXMatrixTranslation(&world, position.x, position.y, position.z);

	model->GetMeshPart(0)->SetWorld(world);
	model->GetMeshPart(0)->Update();
}

void Environment::Sky::Render(void)
{
	D3D::GetDC()->OMSetDepthStencilState(depthStencilState[1], 1);
	D3D::GetDC()->RSSetState(rasterizer[1]);

	buffer->SetPSBuffer(3);
	model->GetMeshPart(0)->Render();

	D3D::GetDC()->RSSetState(rasterizer[0]);
	D3D::GetDC()->OMSetDepthStencilState(depthStencilState[0], 1);
}

void Environment::Sky::PostRender(void)
{
	ImGui::Begin("Environment");

	ImGui::ColorEdit4("Center", buffer->Data.Center);
	ImGui::ColorEdit4("Apex", buffer->Data.Apex);
	ImGui::SliderFloat("Height", &buffer->Data.Height, 0.0f, 5.0f);

	ImGui::End();
}
