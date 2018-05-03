#include "stdafx.h"
#include "./Systems/Window.h"


//새로운 콘솔창을 띄워서 printf, cout 으로 출력가능함
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR param, int command)
{
	D3DDesc desc;
	desc.Instance = instance;
	D3D::SetDesc(desc);

	Window* window = new Window();
	WPARAM wParam = window->Run();
	SAFE_DELETE(window);

	return wParam;
}