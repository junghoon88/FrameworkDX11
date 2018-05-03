#include "stdafx.h"
#include "MeshPart.h"
#include "FbxData.h"
#include "BinModel.h"

#include "../Utilities/Math.h"

using namespace Fbx;

Model::MeshPart::MeshPart(MeshData& data)
	: parentModel(NULL), material(NULL)
	, rasterizerDefault(NULL), rasterizerWireframe(NULL)
	, isSelected(false), isHovering(false)
	, boxCollider(NULL)
{
	D3DXMatrixIdentity(&world);

	name = data.Name;
	world = data.World;

	worldBuffer = new WorldBuffer();
	meshBuffer = new MeshBuffer();

	vertexData.assign(data.Vertices.begin(), data.Vertices.end());
	indexData.assign(data.Indices.begin(), data.Indices.end());

	CreateBuffer();
	CreateRasterizer();

	boxCollider = new BOX_COLLIDER(Shaders + L"BoxCollider.hlsl");
	ScanPointMinMax();
	boxCollider->CreateBuffer();
}

Model::MeshPart::MeshPart(const MeshPart& part)
	: parentModel(NULL), material(NULL)
	, rasterizerDefault(NULL), rasterizerWireframe(NULL)
	, isSelected(false), isHovering(false)
	, boxCollider(NULL)
{
	name = part.name;
	material = new Material(*part.material);
	world = part.world;

	worldBuffer = new WorldBuffer(*part.worldBuffer);
	meshBuffer  = new MeshBuffer(*part.meshBuffer);

	vertexData = part.vertexData;
	indexData  = part.indexData;

	CreateBuffer();
	CreateRasterizer();

	transform = part.transform;

	boxCollider = new BOX_COLLIDER(*part.boxCollider);
}

Model::MeshPart::~MeshPart()
{
	SAFE_DELETE(boxCollider);

	SAFE_RELEASE(rasterizerDefault);
	SAFE_RELEASE(rasterizerWireframe);

	SAFE_DELETE(meshBuffer);
	SAFE_DELETE(worldBuffer);
	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(indexBuffer);
}

void Model::MeshPart::Update(void)
{
	D3DXMATRIX matS, matR, matT;
	D3DXMatrixScaling(&matS, transform.Scale.x, transform.Scale.y, transform.Scale.z);
	D3DXVECTOR3 rot = Math::ToRadian(transform.RotationDeg);
	D3DXQUATERNION q;
	D3DXQuaternionRotationYawPitchRoll(&q, rot.y, rot.x, rot.z);

	D3DXMatrixRotationQuaternion(&matR, &q);

	D3DXMatrixTranslation(&matT, transform.Position.x, transform.Position.y, transform.Position.z);

	matLocal = world * matS * matR * matT;


	if (parentModel)
	{
		matLocal *= parentModel->GetWorld();
	}

	worldBuffer->SetMatrix(matLocal);

	boxCollider->Update(matLocal);
}

void Model::MeshPart::Render(void)
{
	UINT stride = sizeof(VertexTextureNormal);
	UINT offset = 0;


	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	worldBuffer->SetVSBuffer(1);
	material->SetBuffer();

	D3D::GetDC()->DrawIndexed(indexData.size(), 0, 0);
}

void Model::MeshPart::Render(bool bSelected, bool bHovered)
{
	UINT stride = sizeof(VertexTextureNormal);
	UINT offset = 0;

	meshBuffer->Data.viewMode = 0;

	if (isHovering || bHovered)
	{
		D3D::GetDC()->RSSetState(rasterizerWireframe);
		meshBuffer->Data.viewMode = 1;
	}
	else if (isSelected || bSelected)
	{
		D3D::GetDC()->RSSetState(rasterizerWireframe);
	}
	else
	{
		D3D::GetDC()->RSSetState(rasterizerDefault);
		meshBuffer->Data.viewMode = 0;
	}


	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	worldBuffer->SetVSBuffer(1);
	meshBuffer->SetPSBuffer(2);
	material->SetBuffer();



	D3D::GetDC()->DrawIndexed(indexData.size(), 0, 0);
	D3D::GetDC()->RSSetState(rasterizerDefault);

	if (isSelected && !bSelected)
	{
		boxCollider->Render();
	}

	isSelected = false;
	isHovering = false;
}

void Model::MeshPart::CreateBuffer(void)
{
	UINT vertexCount = vertexData.size();
	UINT indexCount  = indexData.size();

	HRESULT hr;
	D3D11_BUFFER_DESC desc;
	D3D11_SUBRESOURCE_DATA data;


	//1. Vertex Buffer
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(VertexTextureNormal) * vertexCount;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = &vertexData[0];

	hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &vertexBuffer);
	assert(SUCCEEDED(hr));


	//2. Index Buffer
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(UINT) * indexCount;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = &indexData[0];

	hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &indexBuffer);
	assert(SUCCEEDED(hr));
}

void Model::MeshPart::CreateRasterizer(void)
{
	D3D11_RASTERIZER_DESC rasterizerDesc;
	States::GetRasterizerDesc(&rasterizerDesc);
	States::CreateRasterizer(&rasterizerDesc, &rasterizerDefault);

	rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
	States::CreateRasterizer(&rasterizerDesc, &rasterizerWireframe);
}

void Model::MeshPart::ScanPointMinMax(void)
{
	D3DXVECTOR3 pointMin = D3DXVECTOR3(999999, 999999, 999999);
	D3DXVECTOR3 pointMax = D3DXVECTOR3(-999999, -999999, -999999);

	for (UINT i = 0; i < vertexData.size(); i++)
	{
		if (pointMin.x > vertexData[i].position.x)
			pointMin.x = vertexData[i].position.x;
		if (pointMax.x < vertexData[i].position.x)
			pointMax.x = vertexData[i].position.x;

		if (pointMin.y > vertexData[i].position.y)
			pointMin.y = vertexData[i].position.y;
		if (pointMax.y < vertexData[i].position.y)
			pointMax.y = vertexData[i].position.y;

		if (pointMin.z > vertexData[i].position.z)
			pointMin.z = vertexData[i].position.z;
		if (pointMax.z < vertexData[i].position.z)
			pointMax.z = vertexData[i].position.z;
	}

	assert(boxCollider != NULL);
	boxCollider->PointMin = pointMin;
	boxCollider->PointMax = pointMax;
}

bool Model::MeshPart::MousePickked(D3DXVECTOR3 start, D3DXVECTOR3 direction, OUT float& dist)
{
	//D3DXMATRIX world, invWorld;
	//parentModel->GetWorld(&world);

	D3DXMATRIX invWorld;
	D3DXMatrixInverse(&invWorld, NULL, &matLocal);

	D3DXVec3TransformCoord(&start, &start, &invWorld);
	D3DXVec3TransformNormal(&direction, &direction, &invWorld);

	//마우스 피킹
	for (UINT i = 0; i < vertexData.size(); i += 3)
	{
		float u, v, distance;
		if (D3DXIntersectTri(&vertexData[i].position,
			&vertexData[i + 1].position,
			&vertexData[i + 2].position,
			&start, &direction, &u, &v, &distance) == TRUE)
		{
			dist = distance;
			return true;
		}
	}
	return false;
}
