#include "stdafx.h"
#include "Camera.h"
#include "Viewport.h"
#include "Perspective.h"

Camera::Camera()
	: position(0, 0, 0), rotation(0, 0)
	, forward(0, 0, 1), right(1, 0, 0), up(0, 1, 0)
{
	D3DXMatrixIdentity(&matRotation);
	D3DXMatrixIdentity(&matView);

	UpdateRotation();
	UpdateView();
}

void Camera::UpdateMove(void)
{
	UpdateView();
}

void Camera::UpdateRotation(void)
{
	D3DXMATRIX x, y;
	D3DXMatrixRotationX(&x, rotation.x);
	D3DXMatrixRotationY(&y, rotation.y);

	matRotation = x * y;

	D3DXVec3TransformNormal(&forward, &D3DXVECTOR3(0, 0, 1), &matRotation);
	D3DXVec3TransformNormal(&right,	  &D3DXVECTOR3(1, 0, 0), &matRotation);
	D3DXVec3TransformNormal(&up,	  &D3DXVECTOR3(0, 1, 0), &matRotation);
}

void Camera::UpdateView(void)
{
	D3DXMatrixLookAtLH(&matView, &position, &(position + forward), &up);
}

D3DXVECTOR3 Camera::GetDirection(Viewport* vp, Perspective* perspective)
{
	D3DXVECTOR2 screenSize;
	screenSize.x = vp->GetWidth();
	screenSize.y = vp->GetHeight();

	D3DXVECTOR3 mouse = gMouse->GetClickPosition();
	D3DXVECTOR2 point;
	D3DXVECTOR3 dir;

	//Viewport
	{	
		point.x = ((2.0f * mouse.x) / screenSize.x) - 1.0f;
		point.y = ((2.0f * mouse.y) / screenSize.y) - 1.0f;
		point.y *= -1.0f;
	}
	
	//Projection
	{
		D3DXMATRIX projection;
		perspective->GetMatrix(&projection);

		point.x = point.x / projection._11;
		point.y = point.y / projection._22;
	}
	
	//View
	{
		D3DXMATRIX invView;
		D3DXMatrixInverse(&invView, NULL, &matView);

		dir.x = (point.x * invView._11) + (point.y * invView._21) + invView._31;
		dir.y = (point.x * invView._12) + (point.y * invView._22) + invView._32;
		dir.z = (point.x * invView._13) + (point.y * invView._23) + invView._33;
		//dir.z = 1.0f;
		//D3DXVec3TransformNormal(&dir, &dir, &invView);

		D3DXVec3Normalize(&dir, &dir);
	}

	return dir;
}