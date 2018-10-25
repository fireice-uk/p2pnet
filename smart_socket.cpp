/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   smart_socket.cpp
 * Author: benqgg
 * 
 * Created on October 25, 2018, 3:22 PM
 */

#include "smart_socket.h"


smart_socket::smart_socket() : sockid(-1)
{
	
}

smart_socket::smart_socket(SOCKET _sockdid, address_container *_addr, address_container *_peer_addr) :
	sockid(_sockdid), addr(_addr), peer_addr(_peer_addr)
{
	if (_sockdid < 0)
	{
		//ERROR, NO SOCK ID
	}
}

bool smart_socket::smart_listen(const char *_ip, uint16_t port, int backlog)
{	
	int domain;

	sockaddr_in6 laddr6 = { 0 };
	laddr6.sin6_port = htons(port);	
	
	int addrlen = sizeof(laddr6);

	if (_ip == nullptr || strlen(_ip) == 0)
	{
		laddr6.sin6_family = AF_INET6;
		laddr6.sin6_addr = in6addr_any;

		domain = PF_INET6;
	}
	else if (inet_pton(AF_INET6, _ip, &(laddr6.sin6_addr)) == 1)
	{
		laddr6.sin6_family = AF_INET6;
		domain = PF_INET6;
	}
	else if (inet_pton(AF_INET, _ip, &(laddr6.sin6_addr)) == 1)
	{	
		laddr6.sin6_family = AF_INET;
		domain = PF_INET;
	}
	else
	{
		//ERROR
		std::cout << "UNRECOGNIZED INTERFACE" << std::endl; return false;
	}

	if ((sockid = socket(domain, SOCK_STREAM, 0)) == -1)
	{
		//ERROR
		std::cout << "CAN NOT OPEN SOCKET - SMART LISTEN" << std::endl;
		return false;
	}
	else
	{

		SOPT on = 1;
		if (setsockopt(sockid, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) != 0)
		{
			std::cout << "SETSOCKOPT ERROR " /*<< strerror(errno)*/  << std::endl;
		}

		/*on = '1';
		if ( setsockopt(sockid, SOL_SOCKET, IPV6_V6ONLY, &on, sizeof(on)) != 0)
		{
			char ermsg[126];
			strerror_s((char*)ermsg, 126, errno);
			std::cout << "SETSOCKOPT ERROR " << ermsg << std::endl;
		}*/			
	
		if (bind(sockid, (sockaddr*)&laddr6, addrlen) != 0)
		{
			//ERROR, CLOSE SOCKET
			//char ermsg[126];
			//strerror_s((char*)ermsg, 126, errno);
			std::cout << "CAN NOT BIND SOCKET IN SMART LISTEN " << /*ermsg << */ std::endl;
			close(sockid);
			sockid = -1;
			return false;
		}
		else
		{
			if (listen(sockid, backlog) == 0)
			{
				socklen_t tmplen = sizeof(laddr6);
				char ipstr[INET6_ADDRSTRLEN];
				inet_ntop(AF_INET, &(laddr6.sin6_addr), ipstr, INET6_ADDRSTRLEN);
                                getsockname(sockid, (sockaddr*)&laddr6, &tmplen);
				std::cout << "LISTEN ON: INTERFACE: " << ipstr << " : PORT: " << ntohs(laddr6.sin6_port) << std::endl;
				return true;
			}
			else
			{
				//ERROR
				std::cout << "CAN NOT LISTEN SOCKET IN SMART LISTEN" << std::endl;
				return false;
			}
		}
	}
}

smart_socket* smart_socket::smart_accept()
{
	if (sockid < 0)
	{
		//ERROR NO SOCK ID
		return nullptr;
	}
	else
	{
		sockaddr_in6 tmpaddr = { 0 };
		socklen_t tmpsize = sizeof(tmpaddr);
		int peer_sockid = accept(sockid, (sockaddr*)&tmpaddr, &tmpsize);
		if (peer_sockid < 0)
		{
			//ERROR
			return nullptr;
		}
		else
		{
			std::cout << "ACCEPTED" << std::endl;
			// peer_addr = new address_container(&tmpaddr); TODO STORE PEER ADDRESS
			return new smart_socket(peer_sockid, addr, addr);
		}
	}
}

smart_socket *smart_socket::smart_connect(const char *_addr, uint16_t port)
{
	
	if (sockid == -1)
	{
		peer_addr = new address_container(_addr, port);
		
		if (peer_addr->get_addr(0) != nullptr)
		{
			try_to_connect((sockaddr*)peer_addr->get_addr(0), sizeof(sockaddr_in));
		}

		if ((sockid == -1) && (peer_addr->get_addr6(0) != nullptr))
		{
			try_to_connect((sockaddr*)peer_addr->get_addr6(0), sizeof(sockaddr_in6));
		}

		if (sockid == -1)
		{
			//ERROR
			char ermsg[126];
//			strerror_s((char*)ermsg, 126, errno);
			std::cout << "CAN NOT CONNECT " /*<< ermsg*/ << std::endl;
			return nullptr;
		}
		else
		{
			//SUCCES
			std::cout << "CONNECTED" << std::endl;
			return this;
		}
	}
	else
	{
		//ERROR SOCKET ALREADY IN USE
		return nullptr;
	}
}

smart_socket* smart_socket::smart_connect(const char *full_addr)
{
	if (sockid == -1)
	{
		peer_addr = new address_container(full_addr);

		int n = 0;
		sockaddr_in* _addr4;
		while ((sockid == -1) && ((_addr4 = peer_addr->get_addr(n)) != nullptr))
		{
			std::cout << "TRY TO CONNECT V4 " << std::endl;
			try_to_connect((sockaddr*)_addr4, sizeof(sockaddr_in));
			n++;
		}

		if (sockid == -1 && (peer_addr->get_addr6(0) != nullptr))
		{
			std::cout << "TRY TO CONNECT V6" << std::endl;
			n = 0;
			sockaddr_in6* _addr6;
			while ((sockid == -1) && ((_addr6 = peer_addr->get_addr6(n)) != nullptr))
			{
				try_to_connect((sockaddr*)_addr6, sizeof(sockaddr_in6));
				n++;
			}
		}


		if (sockid == -1)
		{
			//ERROR
			char ermsg[126];
			//strerror_s((char*)ermsg, 126, errno);
			std::cout << "CAN NOT CONNECT " << /*ermsg << */std::endl;
			return nullptr;
		}
		else
		{
			//SUCCES
			std::cout << "CONNECTED" << std::endl;
			return this;
		}
	}
	else
	{
		//ERROR SOCKET ALREADY IN USE
		return nullptr;
	}
}

int smart_socket::try_to_connect(sockaddr *_addr, size_t len)
{
	if ((sockid = socket(_addr->sa_family, SOCK_STREAM, 0)) == -1)
	{
		//ERROR
		return -1;
	}
	else
	{
		if (connect(sockid, _addr, len) != 0)
		{
			//ERROR, CLOSE SOCKET
			close(sockid);
			sockid = -1;
			return -1;
		}
		else
		{
			//TODO STORE WHICH ADDRESS DID WE BOUND ON
			return sockid;
		}
	}
}

void smart_socket::smart_close()
{
	if (sockid >= 0)
	{
		close(sockid);
	}
	else
	{
		//ERROR
	}

}

int smart_socket::smart_send(const char* data, int len)
{
	return send(sockid, data, len, 0);

}

int smart_socket::smart_recv(char *buffer, int len)
{
	return recv(sockid, buffer, len, 0);
}