#pragma once

#ifdef _WIN32

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

//#define _WIN32_WINNT 0x0801
#include "stdafx.h"
#include <WS2tcpip.h>
#include <WinSock2.h>
#include <iphlpapi.h>
#include <windows.h>
#define close(a) closesocket(a)
#define SOCK_OPT char
#define OPT_YES '1'

inline void wsock_init()
{
	static bool bWSAInit = false;
	if(!bWSAInit)
	{
		WSADATA wsaData;
		if(WSAStartup(MAKEWORD(2, 2), &wsaData) == 0)
			bWSAInit = true;
		else
			std::cout << "WSAStartup ERROR" << std::endl;
	}
}

inline void wsock_cleaup()
{
	WSACleanup();
}
#endif

#ifdef __linux__ //TODO LINUX LINUX SUPPORT
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <unistd.h>
#define INVALID_SOCKET -1
#define SOCK_OPT int
#define OPT_YES 1
#define SD_BOTH SHUT_RDWR

typedef int SOCKET;
inline void wsock_init() {}
inline void wsock_cleaup() {}
#endif

struct ip_port_addr
{
	bool is_ip4;
	union
	{
		sockaddr_in ip4;
		sockaddr_in6 ip6;
	};
};

constexpr size_t MAX_HALFOPEN = 8;
