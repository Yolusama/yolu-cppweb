#include "core/Application.h"
#include<iostream>

class LaunchedApp :public CppWeb::Application
{
public:
	LaunchedApp()
	{
		std::cout <<"���������������߳�IdΪ"<<std::this_thread::get_id()<<'\n';
	}
};

CppWeb::Application* CreateApplication()
{
	return new LaunchedApp();
}