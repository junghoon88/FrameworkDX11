#include "stdafx.h"
#include "FirstPerson.h"

FirstPerson::FirstPerson(float moveSpeed, float rotationSpeed)
	: moveSpeed(moveSpeed), rotationSpeed(rotationSpeed)
{

}

FirstPerson::~FirstPerson()
{

}

void FirstPerson::Update()
{
	D3DXVECTOR3 forward, right, up;
	Camera::GetForward(&forward);
	Camera::GetRight(&right);
	Camera::GetUp(&up);

	//Translation
	{
		D3DXVECTOR3 position;
		Camera::GetPosition(&position);

		if (gKeyboard->IsPress('W'))
			position += forward * moveSpeed * Time::Delta();
		else if (gKeyboard->IsPress('S'))
			position -= forward * moveSpeed * Time::Delta();

		if (gKeyboard->IsPress('A'))
			position -= right * moveSpeed * Time::Delta();
		else if (gKeyboard->IsPress('D'))
			position += right * moveSpeed * Time::Delta();

		if (gKeyboard->IsPress('E'))
			position += up * moveSpeed * Time::Delta();
		else if (gKeyboard->IsPress('Q'))
			position -= up * moveSpeed * Time::Delta();

		position += forward * moveSpeed * Time::Delta() * gMouse->GetMoveValue().z * 0.3f;

		Camera::SetPosition(position.x, position.y, position.z);
	}

	//Rotation
	{
		D3DXVECTOR2 rotation;
		Camera::GetRotation(&rotation);

		if (gMouse->IsPress(1))
		{
			D3DXVECTOR3 move = gMouse->GetMoveValue();

			rotation.x += move.y * rotationSpeed * Time::Delta();
			rotation.y += move.x * rotationSpeed * Time::Delta();
		}

		Camera::SetRotation(rotation.x, rotation.y);
	}
}
