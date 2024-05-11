#include "../include/CorsVerifier.h"

namespace CppWeb
{
	std::unordered_map<std::string, std::shared_ptr<CorsVerifier>> CorsVerifier::CorsPolicy;
	Ref<CorsVerifier> CorsVerifier::Instance = nullptr;

	bool CorsVerifier::CheckHeaders(const std::vector<std::string>& headers)
	{
		for (const std::string& header : headers)
		{
			if (notAllowedHeaders.find(header) != notAllowedHeaders.end())
			   return false;
		}
		return true;
	}

	bool CorsVerifier::CheckMethod(HttpMethod method)
	{
		return allowedMethods.find(method) != allowedMethods.end();
	}

	CorsVerifier& CorsVerifier::AddOrigins(const std::vector<std::string>& origins)
	{
		for(const std::string& origin : origins)
			this->origins.insert(origin);
		return *this;
	}

	CorsVerifier& CorsVerifier::AddNotAllowedHeaders(const std::initializer_list<std::string>& headers)
	{
		notAllowedHeaders = headers;
		return *this;
	}

	CorsVerifier& CorsVerifier::AlloweAllMethods()
	{
		allowedMethods.insert({
			HttpMethod::GET,HttpMethod::POST,HttpMethod::HEAD,
			HttpMethod::PATCH,HttpMethod::_DELETE,HttpMethod::PUT
			});
		return *this;
	}

	CorsVerifier& CorsVerifier::AllowMethods(const std::initializer_list<HttpMethod>& methods)
	{
		allowedMethods.insert(methods);
		return *this;
	}

	CorsVerifier& CorsVerifier::AllowAllOrigin()
	{
		origins.insert("*");
		return *this;
	}

	void CorsVerifier::Check(const HttpRequest& request,HttpResponse& response)
	{
		if (!Instance)return;
		std::string origin = request.GetHeader("Origin");
		if (origin == "")
			return;
		auto it = Instance->origins.find(origin);
		if (it != Instance->origins.end())
		{
			if (!Instance->CheckMethod(request.GetMethod()))
			{
				return;
			}
			if (!Instance->CheckHeaders(request.GetHeaders()))
			{
				return;
			}
			if(Instance->allowCredentials)
			   response.SetHeader("Access-Control-Allow-Credentials", "true");
			response.SetHeader("Access-Control-Allow-Origin", *it);
		}
		else {
			if (Instance->origins.find("*") == Instance->origins.end())return;
			if (!Instance->CheckMethod(request.GetMethod()))
			{
				return;
			}
			if (!Instance->CheckHeaders(request.GetHeaders()))
			{
				return;
			}
			if (Instance->allowCredentials)
				response.SetHeader("Access-Control-Allow-Credentials", "true");
			response.SetHeader("Access-Control-Allow-Origin", "*");
		}
	}

	void CorsVerifier::Create(const std::string& policyName, const std::function<void(CorsVerifier&)>& corsFunc)
	{
		Ref<CorsVerifier>instance(new CorsVerifier());
		corsFunc(*instance);
		CorsPolicy[policyName] = instance;
	}

	CorsVerifier& CorsVerifier::AllowCredentials()
	{
		allowCredentials = true;
		return *this;
	}


	void CorsVerifier::Use(const std::string& policyName)
	{
		if (CorsPolicy.find(policyName) != CorsPolicy.end())
			Instance = CorsPolicy[policyName];
		else return;
	}


}