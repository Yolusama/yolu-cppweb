#include "../core/Application.h"
#include <string>
#include <iostream>
#include<fstream>
#include<chrono>
#include "../core/EntryPoint.h"
#include "../test/SimpleController.h"
#include"../core/ThreadPool.hpp"

namespace CppWeb
{

	Application* Application::Instance = nullptr;

	Application::Application()
	{
		std::ifstream configFile("../WebServer/src/resource/application.json");
		configuration = Json::parse(configFile);
		Json data = configuration["Server"];
		server.reset(new Socket(Socket::AddressFamily::InterNet,Socket::SocketType::stream,Socket::ProtocolType::TCP));
		server->setAddress(((std::string)data["Host"]).data());
		server->setPort(data["Port"]);
		server->Bind();
		server->Listen(data["ListenCount"]);
		std::cout << "服务器访问地址：" << data["Host"] << ",端口号：" << data["Port"] << '\n';
		ThreadPool::Init();
		if (Instance == nullptr)
		{
			Instance = this;
		}
		else 
		{
			std::cerr << "Application has initialized!\n";
		}
		InitRequstUrls();
	}
	using namespace std::chrono;
	void Application::Run()
	{
		int len = sizeof(sockaddr);
		Byte buffer[1024];
		while (true)
		{
			Socket client = server->Accept();
			if (!client.isValid())
			{
				continue;
			}
			int recvBytes = client.Receive(buffer, 1024, 0);
			std::string _res;
			ThreadPool::Add([&] {
				if (recvBytes < 0)
					_res = "";
				else
					_res.assign(buffer, buffer + recvBytes);
				HttpRequest request(_res);
				HttpResponse response;
				std::cout << _res << "\n";
				std::string res;
				if (recvBytes > 0 && _res.find("/favicon.ico") == std::string::npos)
				{
					Ref<HttpPath> path;
					path.reset(HttpPath::Of(request.GetUrl()));
					if (path)
					{
						if (HttpPath::PathExists(*path))
						{
							HttpPath::VerifyRequest(*path,request,response);
							if (response.GetStatusCode() == OK)
							{
								response.SetUrl(request.GetUrl());
								path->InitialRequestParams(request);
								requestHandler.Handler(*path)(request, response);	
							}
							else
							{
								response.SetHeader("Content-Type", "text/html");
								response.SetResult();
							}  
							res = response.GetResult();
							server->Send(client, (Byte*)res.c_str(), res.length(), 0);
							std::cout << "收到访问请求...\n";
						}
						else
						{
							res = "HTTP/1.1 404 Not Found \r\nContent-Type: text/html\r\n\r\n <html><body><h1>404 Not Found</h1></body></html>";
							server->Send(client, (Byte*)res.c_str(), res.length(), 0);
							std::cout << "不存在的请求地址...\n";
						}
					}
				}
				else
				{
					res = "HTTP/1.1 404 Found \r\nContent-Type: text/html\r\n\r\n";
					server->Send(client, (Byte*)res.c_str(), res.length(), 0);
					std::cout << "收到冗余访问请求...\n";
				}
				client.Close();
				    });
			/*auto res = Ref<Socket::SocketInfo>(server->GetClientAddressInfo(client));
			std::cout << res->address<<":"<<res->port << '\n';*/
			std::this_thread::sleep_for(10us);
		}
	}

	void Application::InitRequstUrls()
	{
		SimpleController sc;
		
		HttpPath path1("/Test", {}, {});
		requestHandler.AddHandler(path1, [&](HttpRequest& req, HttpResponse& res) {
			res.SetResult(sc.Test());
			});
		HttpPath path2("/Test1", {"num"}, {});
		requestHandler.AddHandler(path2, [&](HttpRequest& req, HttpResponse& res) {
			res.SetResult(sc.Test1(req.GetQueryParam("num")));
			});
		HttpPath path3("/Test2", { "num1","num2"}, {});
		requestHandler.AddHandler(path3, [&](HttpRequest& req, HttpResponse& res) {
			res.SetResult(sc.Test2(req.GetQueryParam("num1"),req.GetQueryParam("num2")));
			});
		HttpPath path4("/Test3", { "num1","num2","num3"}, {});
		requestHandler[path4] = [&](HttpRequest& req, HttpResponse& res)
			{
				res.SetResult(sc.Test3(req.GetQueryParam("num1"), req.GetQueryParam("num2"),req.GetQueryParam("num3")));
			};
		HttpPath path5("/Test4", {}, {"a"});
		requestHandler[path5] = [&](HttpRequest& req, HttpResponse& res)
			{
				res.SetResult(sc.Test4(req.GetPathVarible("a")));
			};
		HttpPath path6("/Test5", {}, { "a","b" });
		requestHandler[path6] = [&](HttpRequest& req, HttpResponse& res) {
			res.SetResult(sc.Test5(req.GetPathVarible("a"),req.GetPathVarible("b")));
			};
		HttpPath path7("/Test6", { "q" }, { "a","b" });
		requestHandler[path7] = [&](HttpRequest& req, HttpResponse& res) {
			res.SetResult(sc.Test6(req.GetQueryParam("q"),req.GetPathVarible("a"), req.GetPathVarible("b")));
			};
		HttpPath path8("/Test7", {}, {}, "Post");
		requestHandler[path8] = [&](HttpRequest& req, HttpResponse& res)
			{
				res.SetHeader("Content-Type", "application/json");
				res.SetResult(sc.Test7(req.RequestBody()));
			};
		HttpPath::Paths.insert({
			path1,path2,path3,path4,path5,path6,path7,path8
			});	
	}

	Application::~Application()
	{
		server->Close();
	}
}