#pragma once
#include<WinSock2.h>
#include<Windows.h>
#include<string>



#pragma comment(lib, "ws2_32.lib")
#define KB 1024
#define MB 1024*1024
#define GB 1024*1024*1024

namespace CppWeb {
	using Byte = char;
	using ulong = unsigned long;
	struct SocketAddress:public sockaddr_in
	{
		static ulong host_address(ulong hostlong) {
			return htonl(hostlong);
		}
		static ulong host_port(ulong hostport) {
			return htons(hostport);
		}
	};
	class Socket {
	private:
		SOCKET _socket;
		WSADATA data;
		SocketAddress addr;
		int addr_len;
	public:
		Socket();
		enum AddressFamily
		{
			UNIX = 1, InterNet = 2, ImpLink, Pup, Chaos, IPX, ISO, IPV6 = 23
		}af;
		enum SocketType {
			stream = 1, dgram, raw, packet, seqpacket
		}st;
		enum ProtocolType {
			Default = 0, TCP = 6, UDP = 17, SCTP = 132
		}pt;
		struct SocketInfo
		{
			std::string address;
			ulong port;
			SocketInfo(const std::string& address, ulong port)
			{
				this->address = address;
				this->port = port;
			}
		};
		Socket(AddressFamily af,SocketType st,ProtocolType pt);
		void setAddress(ulong Address);
		void setAddress(const char* addr);
		void setPort(ulong port);
		int Bind(int namelen = sizeof(SocketAddress))const;
		int Listen(int ListenCount)const;
		void Close()const;
		bool SetSocket();
		const Socket& Accept();
		int Send(const Socket& toSend,Byte* buffer, int len, int flags)const;
		int Receive(Byte* buffer, int len, int flags)const;
		const std::string& ReceiveAsString(Byte* buffer, int len, int flags)const;
		int Connect(int namelen = sizeof(SocketAddress));
		void Shutdown()const;
		SocketInfo* GetClientAddressInfo(const Socket& client)const;
		
		bool isValid() const;
	};
}