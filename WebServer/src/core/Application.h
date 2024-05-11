#pragma once
#include <Socket.h>
#include <CorsVerifier.h>
#include "Core.h"
#include "HttpRequestHandler.h"
#include <HttpRequestInterceptor.h>


namespace CppWeb {
	

	class Application
	{
	private:
		ScopeRef<Socket> server;
		Json configuration;
		HttpRequestHandler requestHandler;
		Ref<CorsVerifier> corsConfig;
		Ref<HttpRequestInterceptor> intercptor;
	public:
		static Application* Instance;
		Application();
		void Run();
		void InitRequstUrls();
		~Application();
	};
}
