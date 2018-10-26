/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   inc_peer_factory.cpp
 * Author: benqgg
//  * 
 * Created on October 25, 2018, 3:17 PM
 */

#include "inc_peer_factory.h"

inc_peer_factory::inc_peer_factory(const char *saddr, uint16_t port, int backlog)
{
	std::thread t1 = std::thread([&](const char *_saddr, uint16_t _port, int _backlog) {this->call_listen(_saddr, _port, _backlog); }, saddr, port, backlog);
	if (t1.joinable()) t1.detach();
}

inc_peer_factory::~inc_peer_factory()
{

}

void inc_peer_factory::call_listen(const char *saddr, uint16_t port, int backlog)
{	
	int domain;

	sockaddr_in6 laddr6 = { 0 };
	laddr6.sin6_port = htons(port);	
	
	int addrlen = sizeof(laddr6);

	if (saddr == nullptr || strlen(saddr) == 0)
	{
		laddr6.sin6_family = AF_INET6;
		laddr6.sin6_addr = in6addr_any;

		domain = PF_INET6;
	}
	else if (inet_pton(AF_INET6, saddr, &(laddr6.sin6_addr)) == 1)
	{
		laddr6.sin6_family = AF_INET6;
		domain = PF_INET6;
	}
	else if (inet_pton(AF_INET, saddr, &(laddr6.sin6_addr)) == 1)
	{	
		laddr6.sin6_family = AF_INET;
		domain = PF_INET;
	}
	else
	{
		//ERROR
		std::cout << "UNRECOGNIZED INTERFACE - INC PEER FAC" << std::endl;
	}

	if ( (listen_fd = socket(domain, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		//ERROR
		std::cout << "CAN NOT OPEN SOCKET - INC PEER FAC" << std::endl;
	}
	else
	{

		SOCK_OPT on = OPT_YES;
		if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) != 0)
		{
			std::cout << "SETSOCKOPT ERROR - INC PEER FAC" /*<< strerror(errno)*/  << std::endl;
		}		
	
		if (bind(listen_fd, (sockaddr*)&laddr6, addrlen) != 0)
		{
			std::cout << "CAN NOT BIND SOCKET - INC PEER FAC" << std::endl;
			close(listen_fd);
			listen_fd = INVALID_SOCKET;
		}
		else
		{
			if (listen(listen_fd, backlog) == 0)
			{	
				//DISPLAY LISTEN INTERFACE
				socklen_t tmplen = sizeof(laddr6);
				char ipstr[INET6_ADDRSTRLEN];
				inet_ntop(AF_INET, &(laddr6.sin6_addr), ipstr, INET6_ADDRSTRLEN);
                                getsockname(listen_fd, (sockaddr*)&laddr6, &tmplen);
				std::cout << "LISTENING ON: INTERFACE: " << ipstr << " : PORT: " << ntohs(laddr6.sin6_port) << std::endl;
				
				
				accept_thread = std::thread([&]() {this->thread_accept(); });
				if (accept_thread.joinable()) accept_thread.detach();
			}
			else
			{
				//ERROR
				std::cout << "CAN NOT LISTEN - INC PEER FAC" << std::endl;
			}
		}
	}
}

void inc_peer_factory::thread_accept()
{
	std::cout << "Waitng for peer" << std::endl;
	
	sockaddr_in6 tmpaddr = { 0 }; 
	socklen_t tmpsize = sizeof(tmpaddr);
	SOCKET peer_fd = accept(listen_fd, (sockaddr*)&tmpaddr, &tmpsize);
	if ( peer_fd == INVALID_SOCKET)
	{
		std::cout << "CAN NOT ACCEPT - INC PEER FAC" << std::endl;
	}
	else
	{
		std::cout << "ACCEPTED" << std::endl;
		
		std::thread create_peer_thread = std::thread([&](int _peer_fd, sockaddr_in6 _saddr) 
		{this->thread_create_peer(_peer_fd, _saddr); }, peer_fd, tmpaddr);
		if (create_peer_thread.joinable()) create_peer_thread.detach();
	}


	if (!_stop)
		thread_accept();
}

void inc_peer_factory::thread_create_peer(int peer_fd, sockaddr_in6 saddr)
{
	//TO DO STORE IT SOMWHERE
	new peer(peer_fd, &saddr);
}


void inc_peer_factory::stop()
{
	_stop = true;
	close(listen_fd);
}