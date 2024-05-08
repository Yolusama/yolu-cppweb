#pragma once
#include<string>
#include<regex>
#include<Socket.h>
#include<sstream>
#include <unordered_map>
#include<functional>
#include<set>
#include<map>
#include<nlohmann/json.hpp>

#define OK 200
#define OKNORES 204
#define UNAUTHORIZED 401
#define NOTFOUND 404
#define BADREQUEST 405
#define NOTACCEPTABLE 406
#define SERVERERROR 500

namespace CppWeb
{
	namespace CppJson = nlohmann;
	using Json = CppJson::json;
	enum class HttpMethod
	{
		GET = 1, POST = 3, _DELETE = 6, PUT = 8, PATCH = 11, HEAD = 12
	};

	class HttpHeaders
	{
	private:
		std::string version;
		int statusCode;
		std::unordered_map<std::string, std::string> headers;
	public:
		HttpHeaders() {}
		void SetHeaders(const std::string& headers);
		void Set(const std::string& key,const std::string& value);
		void Remove(const std::string& key);
		void SetStatusCode(int code);
		int GetStatusCode()const;
		const std::string& Get(const std::string& key)const;
		const std::string& GetVersion() const;
		friend class HttpResponse;
	};

	class HttpRequest
	{
	private:		
		std::string url;
		HttpHeaders headers;
		std::map<std::string, std::string> queryString;
		std::unordered_map<std::string, std::string> pathVaribles;
		Json requestBody;
		HttpMethod method;
	private:
		void InitParams();
	public:
		HttpRequest();
		HttpRequest(const std::string& requestOption);
		void SetMethod(const std::string& method);
		void SetMethod(HttpMethod method);
		void SetUrl(const std::string& url);
		const std::string& GetUrl()const;
		void AddQueryParam(const std::string& name,const std::string& value);
		void AddPathVarible(const std::string& name,const std::string& value);
		void SetHeader(const std::string& header, const std::string& value);
		const std::string& GetQueryParam(const std::string& name) const;
		const std::string& GetPathVarible(const std::string& name)const;
		const std::map<std::string,std::string>& QueryString()const;
		const std::unordered_map<std::string,std::string>& PathVaribles()const;
		const std::string& GetHeader(const std::string& header)const;
		const Json& RequestBody()const;
		HttpMethod GetMethod()const;
	};

	class HttpResponse
	{
	private:
		HttpHeaders headers;
		std::string result;
		std::string url;
		const char* GetStatusText() const;
	public:
		HttpResponse(const std::string& contentType="text/html",const std::string& version = "HTTP/1.1");
		void SetHeader(const std::string& header,const std::string& value);
 	    const std::string& GetUrl()const;
		int GetStatusCode()const;
		void SetUrl(const std::string& url);
		void SetStatusCode(int code);
		std::string GetResult()const;
		void SetResult();
		void SetResult(const std::string& result);
	};

	class HttpPath
	{
	private:
		std::string url;
		std::map<std::string,std::string> query;
		std::unordered_map<std::string,std::string> varibles;
		HttpMethod method;
		HttpPath() {}
		void InitalPathVarbile(const std::vector<std::string>& varibleStrs);
		void VeryifyPathVarible();
		bool CompareQuery(const std::map<std::string, std::string>& query)const;
		bool ComparePathVarible(const std::unordered_map<std::string, std::string>& pathVaribles)const;
	public:
		HttpPath(const std::string& url, const std::initializer_list<std::string>& queryParamNames, const std::initializer_list<std::string>& pathVaribleNames,const std::string& method);
		HttpPath(const std::string& url, const std::initializer_list<std::string>& queryParamNames, const std::initializer_list<std::string>& pathVaribleNames, HttpMethod method = HttpMethod::GET);
		static HttpPath* Of(const std::string& url);
		static std::set<HttpPath> Paths;
		static void VerifyRequest(const HttpPath& path,const HttpRequest& request, HttpResponse& response);
		void InitialRequestParams(HttpRequest& request);
		bool operator <(const HttpPath& path)const;
		bool operator ==(const HttpPath& path)const;
		bool operator !=(const HttpPath& path)const;
		const std::string& GetUrl()const;
		const std::map<std::string,std::string>& QueryValues()const;
		const std::unordered_map<std::string, std::string>& Varibles()const;
		static bool PathExists(const HttpPath& path);
    };
}

