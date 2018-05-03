#pragma once

#include "./Viewer/Camera.h"
#include "../Viewer/Perspective.h"
#include "../Viewer/Viewport.h"

struct ExecuteValues
{
	class ViewProjectionBuffer* ViewProjection;
	class Perspective*			Perspective;
	class Viewport*				Viewport;
	class Camera*				MainCamera;
	class LightBuffer*			GlobalLight;

	struct GuiSettings*			GuiSettings;

	ExecuteValues()
	{
		ViewProjection	= NULL;
		Perspective		= NULL;
		Viewport		= NULL;
		MainCamera		= NULL;
		GlobalLight		= NULL;
		GuiSettings		= NULL;
	}
};

class Execute
{
public:
	Execute(ExecuteValues* settings)
	{
		this->values = settings;
	}
	virtual ~Execute() {}

	virtual void Update(void) = 0;
	
	virtual void PreRender(void)  = 0;
	virtual void Render(void)     = 0;
	virtual void PostRender(void) = 0;

	virtual void ResizeScreen(void) = 0;

protected:
	ExecuteValues* values;
};