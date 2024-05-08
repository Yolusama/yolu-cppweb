#pragma once
#include <Socket.h>
#include "Core.h"
#include "HttpRequestHandler.h"


namespace CppWeb {
	

	class Application
	{
	private:
		ScopeRef<Socket> server;
		Json configuration;
		HttpRequestHandler requestHandler;
	public:
		static Application* Instance;
		Application();
		void Run();
		void InitRequstUrls();
		~Application();
	};
}
