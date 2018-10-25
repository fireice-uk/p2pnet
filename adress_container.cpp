/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   address_container.cpp
 * Author: benqgg
 * 
 * Created on October 25, 2018, 3:24 PM
 */

#include "address_container.h"

address_container::address_container(const char* full_addr)
{   
    
	char _addr[256];
	char *_port;
	size_t len;
	len = strlen(full_addr);
	if (len > sizeof(_addr))
	{
		//ERROR, FULL ADDRESS TOO LONG
	}
	else
	{
		memcpy(&_addr, full_addr, len);

		if ((_port = strchr(_addr, ':')) == nullptr)
		{
			//ERROR, NO PORT INCLUDED
		}
		else
		{
			_port[0] = '\0';
			_port++;

			struct addrinfo hint;
                        struct addrinfo *res = nullptr;

			hint = { 0 };
			hint.ai_family = AF_INET6;
			hint.ai_socktype = SOCK_STREAM;
			//hint.ai_flags = AI_PASSIVE;
			hint.ai_protocol = 0;

			int err = getaddrinfo(_addr, _port, &hint, &res);
			if (err != 0)
			{
				//ERROR
			}

			addrinfo *reslist = res;
			while (reslist != nullptr)
			{
				switch (reslist->ai_flags)
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
		}
	}

	if (addr.empty() && addr6.empty())
	{
		//ERROR NO IPV4 O IPV6 ADDRESSES
	}

}

address_container::address_container(const char* saddr, uint16_t port)
{
	sockaddr_in6 _addr6 = { 0 };
	sockaddr_in _addr4 = { 0 };
	

	if (inet_pton(AF_INET, saddr, &(_addr4.sin_addr)) == 1)
	{
		_addr4.sin_family = AF_INET;
		_addr4.sin_port = htons(port);

		addr.push_back(_addr4);
	}
	else if (inet_pton(AF_INET6, saddr, &(_addr6.sin6_addr)) == 1)
	{
		_addr6.sin6_family = AF_INET6;
		_addr6.sin6_port = htons(port);

		addr6.push_back(_addr6);
	}
}





sockaddr_in* address_container::get_addr(int n)
{
	if (!addr.empty() && (n < addr.size()))
	{
		return &addr[n];
	}
	else
	{
		//ERROR, NO IPV4 ADDRES
		return nullptr;
	}
}

sockaddr_in6* address_container::get_addr6(int n)
{
	if (!addr6.empty() && (n < addr6.size()))
	{
		return &addr6[n];
	}
	else
	{
		//ERROR, NO IPV4 ADDRES
		return nullptr;
	}
}
