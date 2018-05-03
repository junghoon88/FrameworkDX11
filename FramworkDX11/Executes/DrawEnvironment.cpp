#include "stdafx.h"
#include "DrawEnvironment.h"
#include "../Environment/Sky.h"

using namespace Environment;

DrawEnvironment::DrawEnvironment(ExecuteValues* values)
	: Execute(values)
{
	sky = new Sky(values);
}

DrawEnvironment::~DrawEnvironment()
{
	SAFE_DELETE(sky);
}

void DrawEnvironment::Update()
{
	sky->Update();
}

void DrawEnvironment::Render()
{
	sky->Render();
}

void DrawEnvironment::PostRender()
{
	if (values->GuiSettings->bShowEnvironmentWindow)
	{
		sky->PostRender();
	}
}