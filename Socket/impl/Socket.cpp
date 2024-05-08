#include "Socket.h"
#include<WS2tcpip.h>

using namespace CppWeb;

Socket::Socket(AddressFamily af, SocketType st, ProtocolType pt):addr_len(sizeof(sockaddr))
{
	WSAStartup(MAKEWORD(2, 2), &data);
	_socket = socket(af, st, pt);
	addr.sin_family = af;
	this->af = af;
	this->st = st;
	this->pt = pt;
}

Socket::Socket() {
	addr_len = sizeof(sockaddr);
	WSAStartup(MAKEWORD(2, 2), &data);
}
void Socket::setAddress(ulong address)
{
	addr.sin_addr.S_un.S_addr = SocketAddress::host_address(address);
}

void Socket::setAddress(const char* addr)
{
	inet_pton(this->addr.sin_family, addr, &this->addr.sin_addr);
}

void Socket::setPort(ulong port)
{
	addr.sin_port = SocketAddress::host_port(port);
}

int Socket::Bind(int namelen)const
{
	return bind(_socket,(sockaddr*)&addr,namelen);
}

int Socket::Listen(int ListenCount)const
{
	return listen(_socket, ListenCount);
}

void Socket::Close()const
{
	closesocket(_socket);
	WSACleanup();
}

bool Socket::SetSocket()
{
	this->addr.sin_family = af;
	_socket = socket(af, st, pt);
	return isValid();
}

const Socket& Socket::Accept()
{
	 Socket _socket;
	 _socket._socket=accept(this->_socket, (sockaddr*)&_socket.addr, &addr_len);
	 return _socket;
}

int Socket::Send(const Socket& toSend,Byte* buffer, int len, int flags)const
{
	return send(toSend._socket,buffer,len,flags);
}

int Socket::Receive(Byte* buffer, int len, int flags)const
{
	return recv(_socket,buffer,len,flags);
}

const std::string& Socket::ReceiveAsString(Byte* buffer, int len, int flags) const
{
	int receivdeBytes = Receive(buffer, len, flags);
	if (receivdeBytes < 0)
	{
		return "";
	}
	return std::string(buffer, buffer + receivdeBytes);
}

int Socket::Connect(int namelen)
{
	return connect(_socket, (sockaddr*)&addr, namelen);
}

void Socket::Shutdown()const
{
	shutdown(_socket, 2);
}

Socket::SocketInfo* Socket::GetClientAddressInfo(const Socket& client)const
{
	int addr_len =sizeof(sockaddr);
	int sign = getpeername(client._socket, (sockaddr*)&client.addr, &addr_len);
	if (sign == -1)
	{
		return {  };
	}

	const int len = INET_ADDRSTRLEN;
	char ip[len];
	inet_ntop(af, &client.addr.sin_addr, ip, len);
	return new SocketInfo(std::string(ip) ,ntohs(client.addr.sin_port));
}


bool Socket::isValid()const
{
	return _socket!=INVALID_SOCKET;
}
