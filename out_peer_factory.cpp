/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   out_peer_factory.cpp
 * Author: benqgg
 * 
 * Created on October 25, 2018, 3:19 PM
 */

#include "out_peer_factory.h"

out_peer_factory::out_peer_factory()
{
	//CHECK FOR ERROR
}

SOCKET out_peer_factory::connect(const char* full_addr)
{
	SOCKET peer_fd = INVALID_SOCKET;
	
	//RESOLVE DNS
	char _addr[256];
	char *_port;
	size_t len;
	len = strlen(full_addr);
	if (len > sizeof(_addr))
	{
		//ERROR, FULL ADDRESS TOO LONG
		std::cout << "DNS TOO LONG - OUT PEER FAC" << std::endl;
	}
	else
	{
		memcpy(&_addr, full_addr, len);

		if ((_port = strchr(_addr, ':')) == nullptr)
		{
			//ERROR, NO PORT INCLUDED
			std::cout << "DNS LACK PORT OR WRONG FORMAT - OUT PEER FAC" << std::endl;
		}
		else
		{
			_port[0] = '\0';
			_port++;

			struct addrinfo hint;
                        struct addrinfo *res = nullptr;

			hint = { 0 };
			hint.ai_family = AF_UNSPEC;
			hint.ai_socktype = SOCK_STREAM;
			//hint.ai_flags = AI_PASSIVE;
			hint.ai_protocol = 0;

			int err = getaddrinfo(_addr, _port, &hint, &res);
			if (err != 0)
			{
			    std::cout << "GET ADDR INFO ERROR OUTPEER FACE " << _addr << " : " << _port << std::endl; 
			}

			addrinfo *reslist = res;
			std::vector<sockaddr_in> addr;
			std::vector<sockaddr_in6> addr6;
			while (reslist != nullptr)
			{
				switch (reslist->ai_family)
				{
				case AF_INET:
					addr.push_back(*reinterpret_cast<sockaddr_in*>(reslist->ai_addr));
					break;

				case AF_INET6:
					addr6.push_back(*reinterpret_cast<sockaddr_in6*>(reslist->ai_addr));
					break;
				}

				reslist = reslist->ai_next;
			}
			freeaddrinfo(res);
			
			if (addr.empty() && addr6.empty())
			{
			    //ERROR NO IPV4 O IPV6 ADDRESSES
			    std::cout << "DNS CONTAINS NO IPV4 OR IPV6 ADDRESSES - OUT_PEER_FAC" << std::endl;
			}
			else
			{
			    if(!addr.empty())
			    {	
				int n = rand() % addr.size();
				peer_fd = connect(addr[n]);
				if(peer_fd != INVALID_SOCKET)
				{
					std::cout << "CONNECTED" << std::endl;
					peer *p1 = new peer(peer_fd, &addr[n]);
	
					 //TEST
					 char *s = (char*)"TEST MESSAGE";
					 p1->add_send_data(s, strlen(s));
				}
			    }
			    
			    
			    else if(!addr6.empty())
			    {	
				int n = rand() % addr6.size();
				peer_fd = connect(addr6[n]);
				if(peer_fd != INVALID_SOCKET)
				{
					std::cout << "CONNECTED" << std::endl;
					peer *p1 = new peer(peer_fd, &addr6[n]);
	
					 //TEST
					 char *s = (char*)"TEST MESSAGE";
					 p1->add_send_data(s, strlen(s));
				}
			    }
			    
			    if (peer_fd == INVALID_SOCKET)
			    {
				    //ERROR
				    //char ermsg[126];
				    //strerror_s((char*)ermsg, 126, errno);
				    std::cout << "CAN NOT CONNECT - OUT PEER FAC" << /*ermsg << */std::endl;
			    }
			}
		}
	}
	
	return peer_fd;
}

SOCKET out_peer_factory::connect(const char *saddr, uint16_t port)
{
	sockaddr_in6 _addr6 = { 0 };
	sockaddr_in _addr4 = { 0 };
	
	SOCKET peer_fd = INVALID_SOCKET;
	
	if (inet_pton(AF_INET, saddr, &(_addr4.sin_addr)) == 1)
	{
		_addr4.sin_family = AF_INET;
		_addr4.sin_port = htons(port);
		
		peer_fd = connect(_addr4);
		if(peer_fd != INVALID_SOCKET)
		{
			std::cout << "CONNECTED" << std::endl;
			peer *p1 = new peer(peer_fd, &_addr4);
				      
			//TEST
			char *s = (char*)"TEST MESSAGE";
			p1->add_send_data(s, strlen(s));
		}
		else
		{
		      std::cout << "CAN NOT CONNECT TO: " << saddr << " : " << port << std::endl;
		}
	}
	else if (inet_pton(AF_INET6, saddr, &(_addr6.sin6_addr)) == 1)
	{
		_addr6.sin6_family = AF_INET6;
		_addr6.sin6_port = htons(port);
			
		peer_fd = connect(_addr6); 
		if(peer_fd != INVALID_SOCKET)
		{
		      std::cout << "CONNECTED" << std::endl;
		      peer *p1 = new peer(peer_fd, &_addr6);
				      
		      //TEST
		      char *s = (char*)"TEST MESSAGE";
		      p1->add_send_data(s, strlen(s));
		}
		else
		{
		      std::cout << "CAN NOT CONNECT TO: " << saddr << " : " << port << std::endl;
		}
	}
	else
	{
		std::cout << "GIVEN ADDRESS IS NEITHER IPV4 OR IPV6 - OUT PEER FAC" << std::endl;
	}
	
	return peer_fd;
}

SOCKET out_peer_factory::connect(sockaddr_in addr)
{
     SOCKET peer_fd = socket(PF_INET, SOCK_STREAM, 0);
     
     if(peer_fd == INVALID_SOCKET)
     {
	std::cout << "SOCKET ERROR - OUT PEER FAC" << std::endl;
     }
     else
     {	
	if(::connect(peer_fd, (sockaddr*)&addr, sizeof(sockaddr_in)) == 0)
	{
	    return peer_fd;
	}
     }
     
     return INVALID_SOCKET;
}

SOCKET out_peer_factory::connect(sockaddr_in6 addr)
{
     SOCKET peer_fd = socket(PF_INET6, SOCK_STREAM, 0);
     
     if(peer_fd == INVALID_SOCKET)
     {
	std::cout << "SOCKET ERROR - OUT PEER FAC" << std::endl;
     }
     else
     {
	if(::connect(peer_fd, (sockaddr*)&addr, sizeof(sockaddr_in6)) == 0)
	{
	    return peer_fd;
	}
     }
     
     return INVALID_SOCKET;
}