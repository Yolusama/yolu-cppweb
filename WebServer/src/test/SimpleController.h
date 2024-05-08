#pragma once
#include<string>
#include<sstream>
#include<HttpOption.h>
#include "../core/Core.h"

namespace CppWeb
{
	class SimpleController
	{
	public:
		std::string Test();
		std::string Test1(const std::string& num);
		std::string Test2(const std::string& num1, const std::string& num2);
		std::string Test3(const std::string& num1, const std::string& num2, const std::string& num3);
		std::string Test4(const std::string& a);
		std::string Test5(const std::string& a, const std::string& b);
		std::string Test6(const std::string& q, const std::string& a, const std::string& b);
		std::string Test7(const Json& user);
	};
}
