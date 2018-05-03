#pragma once

class Program
{
public:
	Program();
	~Program();

	void Update(void);

	void PreRender(void);
	void Render(void);
	void PostRender(void);

	void ResizeScreen(void);

private:
	struct ExecuteValues* values;
	vector<class Execute*> executes;
};

