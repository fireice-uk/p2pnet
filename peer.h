/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   peer.h
 * Author: benqgg
 *
 * Created on October 25, 2018, 3:21 PM
 */

#ifndef PEER_H
#define PEER_H

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

typedef int SOCKET;

inline void wsock_init() {}

inline void wsock_cleaup() {}

#endif

class peer
{
public:
  
	//REFACTORING
	peer(SOCKET _peer_fd, const sockaddr_in* addr); 
	peer(SOCKET _peer_fd, const sockaddr_in6* addr);
	void closesocket();
	void add_send_data(const char *data, int len);

protected:
	
	//REFACTORING
	bool quit = false;
	SOCKET peer_fd;
	sockaddr_in addr4;
	sockaddr_in6 addr6;
	std::thread t_send;
	std::thread t_recv;
	std::queue<char*> send_data;
	std::queue<char*> recv_data;
	std::mutex send_mutex;
	std::mutex recv_mutex;
	void send_thread();
	void recv_thread();
	void add_send_data_thread(const char *data, int len);

private:
};
#endif /* PEER_H */
