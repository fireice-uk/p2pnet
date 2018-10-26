#pragma once

#ifdef _WIN32

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

//#define _WIN32_WINNT 0x0801
#include "stdafx.h"
#include <windows.h>
#include <WS2tcpip.h>
#include <WinSock2.h>
#include <iphlpapi.h>
#define WINDOWS TRUE
#define LINUX FALSE
#define close(a) closesocket(a)
#define SOCK_OPT char
#define OPT_YES '1'

inline void wsock_init()
{
	static bool bWSAInit = false;
	if (!bWSAInit)
	{
		WSADATA wsaData;
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		{
			//ERROR
			std::cout << "WSAStartup ERROR" << std::endl;
		}
		else
		{
			bWSAInit = true;
		}
	}
}

inline void wsock_cleaup()
{
	if (WSACleanup() != 0)
	{
		//ERROR
	}
	else
	{

	}
}
#endif


#ifdef __linux__ //TODO LINUX LINUX SUPPORT
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#define WINDOWS FALSE
#define LINUX TRUE
#define INVALID_SOCKET -1
#define SOCK_OPT int
#define OPT_YES 1
#define SD_BOTH SHUT_RDWR

typedef int SOCKET;

inline void wsock_init() {}

inline void wsock_cleaup() {}

#endif
