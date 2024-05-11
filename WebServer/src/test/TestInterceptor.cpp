#include "TestInterceptor.h"
#include <iostream>

namespace CppWeb
{
	bool TestInterceptor::PreHandle(const HttpPath& path, const HttpRequest& request, HttpResponse& response) const
	{
		std::cout << "收到了请求：" << request.GetUrl() << ",但是不拦截\n";
		return true;
	}
}