#pragma once

#include "../Viewer/Perspective.h"
#include "../Viewer/Viewport.h"

struct ExecuteValues
{
	class ViewProjectionBuffer* ViewProjection;
	class Perspective* Perspective;
	class Viewport* Viewport;
};

class Execute
{
protected:
	ExecuteValues* values;

public:
	Execute(ExecuteValues* settings)
	{
		this->values = settings;
	}

	virtual void Update(void) = 0;
	
	virtual void PreRender(void)  = 0;
	virtual void Render(void)     = 0;
	virtual void PostRender(void) = 0;

	virtual void ResizeScreen(void) = 0;
};