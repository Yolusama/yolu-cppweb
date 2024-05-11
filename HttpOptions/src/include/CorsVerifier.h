#pragma once
#include "HttpOption.h"
#include <unordered_set>
#include<iostream>
#include<memory>

namespace CppWeb
{

	class CorsVerifier
	{
	private:
		std::unordered_set<std::string> origins;
		std::unordered_set<std::string> notAllowedHeaders;
		std::unordered_set<HttpMethod> allowedMethods;
		bool allowCredentials;
	private:
		CorsVerifier()
		{
			allowCredentials = false;
		}
		bool CheckHeaders(const std::vector<std::string>& headers);
		bool CheckMethod(HttpMethod method);
		static std::unordered_map<std::string,std::shared_ptr<CorsVerifier>> CorsPolicy;
		static Ref<CorsVerifier> Instance;
	public:
		 CorsVerifier& AddOrigins(const std::vector<std::string>& origins);
		 CorsVerifier& AllowAllOrigin();
		 CorsVerifier& AddNotAllowedHeaders(const std::initializer_list<std::string>& headers);
		 CorsVerifier& AlloweAllMethods();
		 CorsVerifier& AllowMethods(const std::initializer_list<HttpMethod>& methods);
		 CorsVerifier& AllowCredentials();
		 static void Check(const HttpRequest& request, HttpResponse& response);
		 static void Create(const std::string& policyName,const std::function<void(CorsVerifier&)>& corsFunc);
		 static void Use(const std::string& policyName);
	};
}
