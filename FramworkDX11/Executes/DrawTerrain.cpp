#include "stdafx.h"
#include "DrawTerrain.h"
#include "../Landscape/Terrain.h"

using namespace Landscape;

DrawTerrain::DrawTerrain(ExecuteValues* values)
	: Execute(values)
{
	terrain = new Terrain(values);
}

DrawTerrain::~DrawTerrain()
{
	SAFE_DELETE(terrain);
}

void DrawTerrain::Update()
{
	terrain->Update();
}

void DrawTerrain::Render()
{
	terrain->Render();
}

void DrawTerrain::PostRender()
{
	terrain->PostRender();
}