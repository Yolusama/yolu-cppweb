#include "../core/HttpRequestHandler.h"

namespace CppWeb
{
	void HttpRequestHandler::AddHandler(const HttpPath& path, std::function<void(HttpRequest&, HttpResponse&)>&& handler)
	{
		if (requestHandlers.find(path)!=requestHandlers.end())
			return;

		requestHandlers[path] = handler;

	}
	const std::function<void(HttpRequest&, HttpResponse&)>& HttpRequestHandler::Handler(const HttpPath& path) const
	{
		return requestHandlers.at(path);
	}

	std::function<void(HttpRequest&, HttpResponse&)>& HttpRequestHandler::operator[](const HttpPath& path)
	{
		return requestHandlers[path];
	}



	/*std::string HttpRequestHandler::ModifyUrl(const std::string& url) const
	{
		std::string _url = url;
		int paramIndex = url.find_first_of('?');
		if (paramIndex > 0)
		{
			int equalIndex = _url.find_first_of('=');
			int andIndex = _url.find_first_of('&');
			while (equalIndex > 0)
			{
				if (andIndex < 0)
				{
					_url.erase(equalIndex, _url.length() - equalIndex);
				}
				else {
					_url.erase(equalIndex, andIndex - equalIndex);
				}
				equalIndex = _url.find_first_of('=');
				andIndex = _url.find_first_of('&', andIndex + 1);
			}
		}
		return _url;
	}
	
	void HttpRequestHandler::AddHandler(const std::string& url, std::function<void(HttpRequest&, HttpResponse&)> handler)
	{
		if (requestHandlers.find(url) != requestHandlers.end())
		{
			return;
		}
		else {
			requestHandlers[url] = handler;
		}
	}

	bool HttpRequestHandler::UrlMathched(const std::string& url) const
	{
		std::string modfiedUrl = ModifyUrl(url);
		return requestHandlers.find(modfiedUrl) != requestHandlers.end();
	}

	const std::function<void(HttpRequest&, HttpResponse&)>& HttpRequestHandler::Handler(const std::string& url) const
	{
		std::string modfiedUrl = ModifyUrl(url);
		return requestHandlers.at(url);
	}

	std::function<void(HttpRequest&, HttpResponse&)>& HttpRequestHandler::operator[](const std::string& url)
	{
		return requestHandlers[url];
	}*/
}