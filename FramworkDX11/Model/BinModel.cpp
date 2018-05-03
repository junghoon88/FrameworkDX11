#include "stdafx.h"
#include "BinModel.h"

#include "MeshPart.h"

#include "../Utilities/Math.h"

Model::BinModel::BinModel()
	: isSelected(false), isHovering(false), boxCollider(NULL)
{
	D3DXMatrixIdentity(&world);

	boxCollider = new BOX_COLLIDER(Shaders + L"BoxCollider.hlsl");
	//ScanPointMinMax 는 Fbx::FbxModel::Read 에서 part 를 다 생성한 다음에 한다.
}

Model::BinModel::BinModel(const BinModel& model)
	: isSelected(false), isHovering(false), boxCollider(NULL)
{
	D3DXMatrixIdentity(&world);

	name = model.name;
	
	UINT size = model.meshParts.size();
	meshParts.resize(size);
	for (UINT i = 0; i < size; i++)
	{
		meshParts[i] = new MeshPart(*model.meshParts[i]);
		meshParts[i]->SetParentModel(this);
	}

	world = model.world;
	transform = model.transform;

	boxCollider = new BOX_COLLIDER(*model.boxCollider);
	ScanPointMinMax();
	boxCollider->CreateBuffer();
}

Model::BinModel::~BinModel()
{
	SAFE_DELETE(boxCollider);

	for (MeshPart* part : meshParts)
	{
		SAFE_DELETE(part);
	}
}

void Model::BinModel::Update(void)
{
	D3DXMATRIX matS, matR, matT;
	D3DXMatrixScaling(&matS, transform.Scale.x, transform.Scale.y, transform.Scale.z);
	D3DXVECTOR3 rot = Math::ToRadian(transform.RotationDeg);
	D3DXQUATERNION q;
	D3DXQuaternionRotationYawPitchRoll(&q, rot.y, rot.x, rot.z);

	D3DXMatrixRotationQuaternion(&matR, &q);

	D3DXMatrixTranslation(&matT, transform.Position.x, transform.Position.y, transform.Position.z);

	world = matS * matR * matT;

	for (MeshPart* part : meshParts)
	{
		part->Update();
	}

	boxCollider->Update(world);
}

void Model::BinModel::Render(void)
{
	for (MeshPart* part : meshParts)
	{
		part->Render(isSelected, isHovering);
	}

	if(isSelected)
		boxCollider->Render();

	isSelected = false;
	isHovering = false;
}

bool Model::BinModel::MousePickked(OUT map<float, MeshPart*>& pickkedParts, D3DXVECTOR3 start, D3DXVECTOR3 direction)
{
	float dist = 0.0f;

	for (MeshPart* part : meshParts)
	{
		if (part->MousePickked(start, direction, dist))
		{
			pickkedParts[dist] = part;
		}
	}

	if (pickkedParts.size() > 0)
		return true;

	return false;
}

void Model::BinModel::ScanPointMinMax(void)
{
	assert(meshParts.size() > 0);

	D3DXVECTOR3 pointMin = D3DXVECTOR3(999999, 999999, 999999);
	D3DXVECTOR3 pointMax = D3DXVECTOR3(-999999, -999999, -999999);

	for (MeshPart* part : meshParts)
	{
		if (pointMin.x > part->GetBoxCollider()->PointMin.x)
			pointMin.x = part->GetBoxCollider()->PointMin.x;
		if (pointMax.x < part->GetBoxCollider()->PointMax.x)
			pointMax.x = part->GetBoxCollider()->PointMax.x;

		if (pointMin.y > part->GetBoxCollider()->PointMin.y)
			pointMin.y = part->GetBoxCollider()->PointMin.y;
		if (pointMax.y < part->GetBoxCollider()->PointMax.y)
			pointMax.y = part->GetBoxCollider()->PointMax.y;

		if (pointMin.z > part->GetBoxCollider()->PointMin.z)
			pointMin.z = part->GetBoxCollider()->PointMin.z;
		if (pointMax.z < part->GetBoxCollider()->PointMax.z)
			pointMax.z = part->GetBoxCollider()->PointMax.z;
	}

	assert(boxCollider != NULL);
	boxCollider->PointMin = pointMin;
	boxCollider->PointMax = pointMax;
}

