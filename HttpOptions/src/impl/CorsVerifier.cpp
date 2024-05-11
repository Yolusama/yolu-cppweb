#include "../include/CorsVerifier.h"

namespace CppWeb
{
	std::unordered_map<std::string, std::shared_ptr<CorsVerifier>> CorsVerifier::CorsPolicy;
	CorsVerifier* CorsVerifier::Instance = nullptr;

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

	void CorsVerifier::AddOrigins(const std::vector<std::string>& origins)
	{
		for(const std::string& origin : origins)
			this->origins.insert(origin);
	}

	void CorsVerifier::AddNotAllowedHeaders(const std::initializer_list<std::string>& headers)
	{
		notAllowedHeaders = headers;
	}

	void CorsVerifier::AlloweAllMethods()
	{
		allowedMethods.insert({
			HttpMethod::GET,HttpMethod::POST,HttpMethod::HEAD,
			HttpMethod::PATCH,HttpMethod::_DELETE,HttpMethod::PUT
			});
	}

	void CorsVerifier::AllowMethods(const std::initializer_list<HttpMethod>& methods)
	{
		allowedMethods.insert(methods);
	}

	void CorsVerifier::AllowAllOrigin(HttpResponse& response)
	{
		response.SetHeader("Access-Control-Allow-Origin", "*");
	}

	void CorsVerifier::Check(const HttpRequest& request,HttpResponse& response)
	{
		if (!Instance)return;
		auto it = Instance->origins.find(request.GetHeader("Origin"));
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
			response.SetHeader("Access-Control-Allow-Origin", *it);
		}
	}

	void CorsVerifier::AllowCredentials(HttpResponse& response)
	{
		response.SetHeader("Access-Control-Allow-Credentials", "true");
	}

	std::shared_ptr<CorsVerifier> CorsVerifier::Create(const std::string& policyName)
	{
		std::shared_ptr<CorsVerifier>instance(new CorsVerifier());
		CorsPolicy[policyName] = instance;
		return instance;
	}

	void CorsVerifier::Use(const std::string& policyName)
	{
		if (CorsPolicy.find(policyName) != CorsPolicy.end())
			Instance = CorsPolicy[policyName].get();
		else return;
	}


}