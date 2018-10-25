/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   address_container.h
 * Author: benqgg
 *
 * Created on October 25, 2018, 3:24 PM
 */

#ifndef ADDRESS_CONTAINER_H
#define ADDRESS_CONTAINER_H

#pragma once

#include <sys/types.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <queue>
#include <stdio.h>
#include <inttypes.h>
#include <thread>
#include <mutex>


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
#define SOPT char

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
#define SOPT int
#define INVALID_SOCKET -1

typedef int SOCKET;

inline void wsock_init() {}

inline void wsock_cleaup() {}

#endif




class address_container
{
public:
	address_container(const char* full_addr);
	address_container(const char* naddr, uint16_t port); 

	//RETURN NULLPTR IF NO MORE ADRESSES
	struct sockaddr_in* get_addr(int n = 0);
	struct sockaddr_in6* get_addr6(int n = 0);


protected:

	std::vector<sockaddr_in> addr;
	std::vector<sockaddr_in6> addr6;

private:
};

#endif /* ADDRESS_CONTAINER_H */