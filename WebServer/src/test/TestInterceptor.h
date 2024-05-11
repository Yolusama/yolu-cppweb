#pragma once
#include<HttpRequestInterceptor.h>

namespace CppWeb
{
	class TestInterceptor : public HttpRequestInterceptor
	{
	public:
		TestInterceptor(const std::vector<std::string>& excludedUrls) :HttpRequestInterceptor(excludedUrls) {}
		bool PreHandle(const HttpPath& path, const HttpRequest& request, HttpResponse& response) const override;
	};
}
