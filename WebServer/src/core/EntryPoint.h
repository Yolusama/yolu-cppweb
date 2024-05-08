#pragma once
#include "Application.h"

extern CppWeb::Application* CreateApplication();

int main(int args, char** argv)
{
	CppWeb::Application* app = CreateApplication();
	app->Run();

	delete app;
	return 0;
}
