#pragma once

namespace Landscape { class Terrain; }

class DrawTerrain : public Execute
{
public:
	DrawTerrain(ExecuteValues* values);
	~DrawTerrain();

	void Update();

	void PreRender() {}
	void Render();
	void PostRender();

	void ResizeScreen() {}

private:
	Landscape::Terrain* terrain;
};