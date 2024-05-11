#include "../include/HttpOption.h"
#include<iostream>


namespace CppWeb
{
	static HttpMethod ParseStrAsHttpMethod(const std::string& method)
	{
		std::regex reg("GET", std::regex_constants::icase);
		if (std::regex_search(method, reg))
			return HttpMethod::GET;

		std::regex reg1("POST", std::regex_constants::icase);
		if (std::regex_search(method, reg1))
			return HttpMethod::POST;

		std::regex reg2("PUT", std::regex_constants::icase);
		if (std::regex_search(method, reg2))
			return HttpMethod::PUT;

		std::regex reg3("PATCH", std::regex_constants::icase);
		if (std::regex_search(method, reg3))
			return HttpMethod::PATCH;

		std::regex reg4("DELETE", std::regex_constants::icase);
		if (std::regex_search(method, reg4))
			return HttpMethod::_DELETE;

		std::regex reg5("HEAD", std::regex_constants::icase);
		if (std::regex_search(method, reg5))
			return HttpMethod::HEAD;
	}

	static std::vector<std::string> SplitString(const std::string& regexExp,const std::string& str)
	{
		std::regex regexDelimiter(regexExp);
		return {
			std::sregex_token_iterator(str.begin(), str.end(), regexDelimiter, -1),
			std::sregex_token_iterator()
		};
	}

	HttpRequest::HttpRequest()
	{
	}

	HttpRequest::HttpRequest(const std::string& requestOption)
	{
		std::string str = requestOption.substr(requestOption.find_first_of(' ') + 1);
		url = str.substr(0, str.find_first_of(' '));
		std::string method = requestOption.substr(0, requestOption.find_first_of(' '));
		SetMethod(method);
		int bodySign = requestOption.find_first_of('{', requestOption.find_first_of("Content-Length"));
		if (bodySign > 0)
		{
			int lastBodySign = requestOption.find_last_of('}');
			std::string jsonStr = requestOption.substr(bodySign, lastBodySign - bodySign + 1);
			requestBody = Json::parse(jsonStr);
			headers.SetHeaders(requestOption.substr(0, lastBodySign));
		}
		headers.SetHeaders(requestOption);
		//InitParams();
	}

	void HttpRequest::SetMethod(const std::string& method)
	{
		this->method = ParseStrAsHttpMethod(method);
	}

	void HttpRequest::SetMethod(HttpMethod method)
	{
		this->method = method;
	}

	void HttpRequest::SetUrl(const std::string& url)
	{
		this->url = url;
	}

	const std::string& HttpRequest::GetUrl() const
	{
		return url;
	}

	void HttpRequest::AddQueryParam(const std::string& name, const std::string& value)
	{
		queryString[name] = value;
	}

	void HttpRequest::AddPathVarible(const std::string& name, const std::string& value)
	{
		pathVaribles[name] = value;
	}

	void HttpRequest::SetHeader(const std::string& header, const std::string& value)
	{
		if (value.length() > 0)
			headers.Set(header, value);
		else
			headers.Remove(header);
	} 

	const std::string& HttpRequest::GetQueryParam(const std::string& name) const
	{
		if (queryString.find(name) == queryString.end())
			return "";
		return queryString.at(name);
	}

	const std::string& HttpRequest::GetPathVarible(const std::string& name) const
	{
		if (pathVaribles.find(name) == pathVaribles.end())
			return "";
		return pathVaribles.at(name);
	}

	const std::map<std::string,std::string>& HttpRequest::QueryString() const
	{
		return queryString;
	}

	const std::unordered_map<std::string, std::string>& HttpRequest::PathVaribles() const
	{
		return pathVaribles;
	}

	const std::string& HttpRequest::GetHeader(const std::string& header)const
	{
		return headers.Get(header);
	}

	std::vector<std::string> HttpRequest::GetHeaders() const
	{
		return headers.HeaderKeys();
	}

	const Json& HttpRequest::RequestBody() const
	{
		return requestBody;
	}

	HttpMethod HttpRequest::GetMethod() const
	{
		return method;
	}

	void HttpRequest::InitParams()
	{
		int paramIndex = url.find_first_of('?');
		if (paramIndex > 0)
		{
			std::string param = url.substr(paramIndex + 1);
			std::regex regexDelimiter("[&]+");
			std::vector<std::string> result{
				std::sregex_token_iterator(param.begin(), param.end(), regexDelimiter, -1),
				std::sregex_token_iterator()
			};
			for (const std::string& str : result)
			{
				int equalIndex = str.find_first_of('=');
				queryString[str.substr(0, equalIndex)] = str.substr(equalIndex + 1, str.length() - equalIndex - 1);
			}
		}
	}

	const char* HttpResponse::GetStatusText()const
	{
		switch (headers.GetStatusCode())
		{
		case OK:return "OK";
		case OKNORES:return "OK but no response";
		case UNAUTHORIZED:return "Unauthorized";
		case NOTFOUND:return "Not Found";
		case BADREQUEST:return "Bad Request";
		case NOTACCEPTABLE:return "Not Acceptable";
		case SERVERERROR:return "Server Error";
		default:return "";
		}
	}

	HttpResponse::HttpResponse(const std::string& contentType,const std::string& version)
	{
		headers.version = version;
		headers.Set("Content-Type", contentType);
	}

	void HttpResponse::SetHeader(const std::string& header, const std::string& value)
	{
		if (value.length() > 0)
			headers.Set(header, value);
		else
			headers.Remove(header);
	}


	const std::string& HttpResponse::GetUrl() const
	{
		return url;
	}

	int HttpResponse::GetStatusCode() const
	{
		return headers.GetStatusCode();
	}

	void HttpResponse::SetUrl(const std::string& url)
	{
		this->url = url;
	}

	void HttpResponse::SetStatusCode(int code)
	{
		headers.SetStatusCode(code);
	}


	std::string HttpResponse::GetResult() const
	{
		std::stringstream res;
		res << headers.GetVersion() << ' ' << 
			headers.GetStatusCode() << ' '<< this->GetStatusText() << "\r\n";
		
		for (auto it = headers.headers.begin(); it != headers.headers.end(); it++)
		{
			res << it->first << ':' << it->second << "\r\n";
		}
		res << "\r\n" << result;

		return res.str(); 
	}

	void HttpResponse::SetResult()
	{
		result = GetStatusText();
	}

	void HttpResponse::SetResult(const std::string& result)
	{
		this->result = result;
	}


	void HttpHeaders::SetHeaders(const std::string& headers)
	{
		std::stringstream ss(headers);
		std::string line;
		bool flag = false;
		std::regex pattern("\\s+");
		while (std::getline(ss, line))
		{	
			if (!flag)
			{
				version = SplitString("\\s+",line).back();
				flag = true;
			}
			else
			{
				line = std::regex_replace(line, pattern, "");
				int splitIndex = line.find_first_of(':');
				Set(line.substr(0, splitIndex), line.substr(splitIndex + 1));
			}
		}
	}

	void HttpHeaders::Set(const std::string& key,const std::string& value)
	{
		headers[key] = value;
	}

	void HttpHeaders::Remove(const std::string& key)
	{
		headers.erase(key);
	}

	const std::string& HttpHeaders::Get(const std::string& key) const
	{
		if (Has(key))
			return headers.at(key);
		else return "";
	}

	bool HttpHeaders::Has(const std::string& key)const
	{
		return headers.contains(key);
	}

	int HttpHeaders::GetStatusCode() const
	{
		return statusCode;
	}

	void HttpHeaders::SetStatusCode(int code)
	{
		statusCode = code;
	}

	const std::string& HttpHeaders::GetVersion() const
	{
		return version;
	}

	std::vector<std::string> HttpHeaders::HeaderKeys() const
	{
		std::vector<std::string> res;
		for (auto it = headers.begin(); it != headers.end(); it++)
			res.push_back(it->first);
			return res;
	}

	std::set<HttpPath> HttpPath::Paths;

	void HttpPath::InitalPathVarbile(const std::vector<std::string>& varibleStrs)
	{
		int i = 0;
		for (auto it = varibles.begin(); it != varibles.end(); i++)
		{
			it->second = varibleStrs[i++];
		}
	}

	void HttpPath::VeryifyPathVarible()
	{
		bool usePathVarible = false;
		std::vector<std::string> values = SplitString("[/]+",url);
		for (auto it = Paths.begin(); it != Paths.end(); it++)
		{
			std::stringstream ss;
			ss << '^' << it->GetUrl() << "/.*";
			std::regex pattern(ss.str());
			if (std::regex_search(url,pattern))
			{
				url = it->GetUrl();
				varibles = it->Varibles();
				usePathVarible = true;
				break;
			}
		}
		if (!usePathVarible) return;
		int count = varibles.size();
		for (auto it = varibles.begin(); it != varibles.end(); it++)
		{
			it->second = values[values.size() - count];
			count--;
		}
	}

	bool HttpPath::CompareQuery(const std::map<std::string, std::string>& query) const
	{
		if (this->query.empty())return false;
		bool res = true;
		for (auto it1 = this->query.begin(), it2 = query.begin(); it1 != this->query.end(); it1++, it2++)
		{
			res =res && it1->first == it2->first;
		}
		return res;
	}

	bool HttpPath::ComparePathVarible(const std::unordered_map<std::string, std::string>& pathVaribles) const
	{
		if (varibles.empty())return false;
		bool res = true;
		for (auto it1 = varibles.begin(), it2 = pathVaribles.begin(); it1 != varibles.end(); it1++, it2++)
		{
			res = res && it1->first == it2->first;
		}
		return res;
	}

	HttpPath::HttpPath(const std::string& url, const std::initializer_list<std::string>& queryParamNames, const std::initializer_list<std::string>& pathVaribleNames,const std::string& method):url(url)
	{
		for (const std::string& paramName : queryParamNames)
			query[paramName] = "";
		for (const std::string& pathvaribleName : pathVaribleNames)
			varibles[pathvaribleName] = "";
		this->method = ParseStrAsHttpMethod(method);
	}

	HttpPath::HttpPath(const std::string& url, const std::initializer_list<std::string>& queryParamNames, const std::initializer_list<std::string>& pathVaribleNames, HttpMethod method):url(url)
	{
		for (const std::string & paramName : queryParamNames)
			query[paramName] = "";
		for (const std::string& pathvaribleName : pathVaribleNames)
			varibles[pathvaribleName] = "";
		this->method = method;
	}

	HttpPath* HttpPath::Of(const std::string& url)
	{
		HttpPath* path = new HttpPath();
		int paramIndex = url.find_first_of('?');
		if (paramIndex > 0)
		{
			std::string param = url.substr(paramIndex+1, url.length() - paramIndex - 1);
			std::string unDealPathUrl = url.substr(0, paramIndex);
			std::vector<std::string> params = SplitString("[&]+", param);
			for (const std::string& str : params)
			{
				int equalIndex = str.find_first_of('=');
				if (equalIndex < 0)
				{
					delete path;
					return nullptr;
				}
				path->query.insert(std::make_pair(str.substr(0,equalIndex),str.substr(equalIndex+1,str.length()-equalIndex-1)));
			}
			path->url = unDealPathUrl;
			path->VeryifyPathVarible();
		}
		else 
		{
			path->url = url;
			path->VeryifyPathVarible();
		}
		return path;
	}

	void HttpPath::VerifyRequest(const HttpPath& path,const HttpRequest& request,HttpResponse& response)
	{
		auto it = Paths.find(path);
		if (it->method != request.GetMethod())
		{
			response.SetStatusCode(NOTACCEPTABLE);
			return;
		}
		if (request.GetMethod() == HttpMethod::GET)
		{
			if (!request.RequestBody().empty())
			{
				response.SetStatusCode(SERVERERROR);
				return;
			}
		}
		bool res;
		if (it->query.size() > 0 && it->varibles.empty())
		{
				res = path.CompareQuery(it->query);
		}

		if (!it->varibles.empty() && it->query.empty())
		{
			res = path.ComparePathVarible(it->varibles);
		}

		if (it->varibles.empty() && it->query.empty())
		{
			res = true;
		}

		if (!it->varibles.empty() && !it->query.empty())
		{
			res = path.CompareQuery(it->query) && path.ComparePathVarible(it->varibles);
		}

		if (!res)
			response.SetStatusCode(BADREQUEST);
		else response.SetStatusCode(OK);
	}

	void HttpPath::InitialRequestParams(HttpRequest& request)
	{
		for (auto it = query.begin(); it != query.end(); it++)
			request.AddQueryParam(it->first,it->second);
		for (auto it = varibles.begin();it!=varibles.end();it++)
			request.AddPathVarible(it->first,it->second);
	}

	bool HttpPath::operator <(const HttpPath& path) const
	{
		return this->url < path.url;
	}

	bool HttpPath::operator ==(const HttpPath& path) const
	{
		return url == path.url;
	}

	bool HttpPath::operator!=(const HttpPath& path) const
	{
		return url != path.url;
	}

	const std::string& HttpPath::GetUrl() const
	{
		return url;
	}

	const std::map<std::string, std::string>& HttpPath::QueryValues() const
	{
		return query;
	}

	const std::unordered_map<std::string,std::string>& HttpPath::Varibles() const
	{
		return varibles;
	}

	bool HttpPath::PathExists(const HttpPath& path)
	{
		return Paths.contains(path);
	}

}