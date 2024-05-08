#include "SimpleController.h"

namespace CppWeb
{
	std::string SimpleController::Test()
	{
		std::stringstream ss;
		ss<<"<html><body><h1>Hello,world!"<<"</h1>"<<"</body></html>";
		return ss.str();
	}

	std::string SimpleController::Test1(const std::string& num)
	{
		std::stringstream ss;
		ss << "<html><body><h1>" << "»ñµÃÊý×Önum = " << std::atoi(num.data()) <<
			"</h1><p>" << "</p></body></html>";
		return ss.str();
	}

	std::string SimpleController::Test2(const std::string& num1, const std::string& num2)
	{
		std::stringstream ss;
		ss << "<html><body><h1>" << "num1+num2 = " << (std::atoi(num1.data()) + std::atoi(num2.data())) <<
			"</h1><p>" << "</p></body></html>";
		return ss.str();
	}

	std::string SimpleController::Test3(const std::string& num1, const std::string& num2, const std::string& num3)
	{
		std::stringstream ss;
		ss << "<html><body><h1>" << "num1*num2*num3 = " << (std::atoi(num1.data())*std::atoi(num2.data())*std::atoi(num3.data())) <<"</h1><p>" << "</p></body></html>";
		return ss.str();
	}

	std::string SimpleController::Test4(const std::string& a)
	{
		std::stringstream ss;
		ss << "<html><body><h1>" << "PathVarible a= " << a <<
			"</h1><p>" << "</p></body></html>";
		return ss.str();
	}

	std::string SimpleController::Test5(const std::string& a, const std::string& b)
	{
		std::stringstream ss;
		ss << "<html><body><h1>" << "PathVarible£ºa= " << a <<",b="<<b<<
			",a+b="<<a+b<<"</h1><p>" << "</p></body></html>";
		return ss.str();
	}

	std::string SimpleController::Test6(const std::string& q, const std::string& a, const std::string& b)
	{
		std::stringstream ss;
		ss << "<html><body><h1>" << "queryString q=" << q << "<br>PathVarible£ºa= " << a << ",b=" << b << "</h1><p>" << "</p></body></html>";
		return ss.str();
	}
	std::string SimpleController::Test7(const Json& user)
	{
		std::stringstream ss;
		ss << user.dump(4) ;
		return ss.str();
	}
}