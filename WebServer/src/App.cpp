#include "core/Application.h"
#include<iostream>

class LaunchedApp :public CppWeb::Application
{
public:
	LaunchedApp()
	{
		std::cout <<"服务器已启动，线程Id为"<<std::this_thread::get_id()<<'\n';
	}
};

CppWeb::Application* CreateApplication()
{
	return new LaunchedApp();
}