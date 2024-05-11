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
	private:
		CorsVerifier()
		{

		}
		bool CheckHeaders(const std::vector<std::string>& headers);
		bool CheckMethod(HttpMethod method);
		static std::unordered_map<std::string,std::shared_ptr<CorsVerifier>> CorsPolicy;
		static CorsVerifier* Instance;
	public:
		 void AddOrigins(const std::vector<std::string>& origins);
		 void AllowAllOrigin(HttpResponse& response);
		 
		 void AddNotAllowedHeaders(const std::initializer_list<std::string>& headers);
		 void AlloweAllMethods();
		 void AllowMethods(const std::initializer_list<HttpMethod>& methods);
		 void AllowCredentials(HttpResponse& response);
		 static void Check(const HttpRequest& request, HttpResponse& response);
		 static std::shared_ptr<CorsVerifier> Create(const std::string& policyName);
		 static void Use(const std::string& policyName);
	};
}
