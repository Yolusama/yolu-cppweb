#pragma once
#include "HttpOption.h"
#include<unordered_set>

namespace CppWeb
{
	class HttpRequestInterceptor
	{
	protected:
		std::unordered_set<std::string> excludedUrls;
	protected:
		HttpRequestInterceptor(const std::vector<std::string>& excludedUrls)
		{
			for(const std::string& url:excludedUrls)
			this->excludedUrls.insert(url);
		}
	public:
		virtual bool PreHandle(const HttpPath& path,const HttpRequest& request, HttpResponse& response)const {
			if (excludedUrls.empty()) return false;
			if(excludedUrls.find(path.GetUrl())!=excludedUrls.end())
			     return true; 
			return false;
		}
		virtual void AfterHandle(const HttpRequest& request, HttpResponse& response)const {}
	};
}
