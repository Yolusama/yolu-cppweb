#pragma once
#include"Core.h"
#include <HttpOption.h>

namespace CppWeb
{
	static struct HttpPathHash {
		std::size_t operator()(const HttpPath& path) const {
			return std::hash<std::string>{}(path.GetUrl());
		}
	};

	class HttpRequestHandler
	{
	private:
		std::unordered_map<HttpPath,std::function<void(HttpRequest&, HttpResponse&)>,HttpPathHash> requestHandlers;
	//private:std::string ModifyUrl(const std::string& url)const;	
	public:
		HttpRequestHandler() {}
		void AddHandler(const HttpPath& path, std::function<void(HttpRequest&, HttpResponse&)>&& handler);
		const std::function<void(HttpRequest&, HttpResponse&)>& Handler(const HttpPath& path) const;
		std::function<void(HttpRequest&, HttpResponse&)>& operator[](const HttpPath& path);
		/*void AddHandler(const std::string& url, std::function<void(HttpRequest&, HttpResponse&)> handler);
		bool UrlMathched(const std::string& url)const;
		const std::function<void(HttpRequest&, HttpResponse&)>& Handler(const std::string& url) const;
		std::function<void(HttpRequest&, HttpResponse&)>& operator[](const std::string& url);*/
		~HttpRequestHandler() {}
	};
}
