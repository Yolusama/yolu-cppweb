#include "TestInterceptor.h"
#include <iostream>

namespace CppWeb
{
	bool TestInterceptor::PreHandle(const HttpPath& path, const HttpRequest& request, HttpResponse& response) const
	{
		std::cout << "�յ�������" << request.GetUrl() << ",���ǲ�����\n";
		return true;
	}
}