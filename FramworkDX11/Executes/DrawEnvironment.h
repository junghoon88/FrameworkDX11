#pragma once

namespace Environment { class Sky; }

class DrawEnvironment : public Execute
{
public:
	DrawEnvironment(ExecuteValues* values);
	~DrawEnvironment();

	void Update();

	void PreRender() {}
	void Render();
	void PostRender();

	void ResizeScreen() {}

private:
	Environment::Sky* sky;
};