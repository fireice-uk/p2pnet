//
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
#include <random>
#include <algorithm>
#include <future>

out_peer_factory::sock_data* out_peer_factory::connect_dns(sock_data &out, const char *full_addr)
{
	const char* delim = strrchr(full_addr, ':');
	
	if(delim == nullptr)
		return &out;

	int64_t dlen = delim-full_addr;
	std::string ip(full_addr, dlen);
	std::string port(full_addr+dlen+1);

	struct addrinfo hint;
	struct addrinfo *res = nullptr;
	hint = {0};

	hint.ai_family = AF_UNSPEC;
	hint.ai_socktype = SOCK_STREAM;
	hint.ai_protocol = 0;

	if(getaddrinfo(ip.c_str(), port.c_str(), &hint, &res) != 0)
		return &out;

	addrinfo *reslist = res;
	std::vector<sockaddr_in> addr;
	std::vector<sockaddr_in6> addr6;
	while(reslist != nullptr)
	{
		switch(reslist->ai_family)
		{
		case AF_INET:
			addr.push_back(*reinterpret_cast<sockaddr_in *>(reslist->ai_addr));
			break;

		case AF_INET6:
			addr6.push_back(*reinterpret_cast<sockaddr_in6 *>(reslist->ai_addr));
			break;
		}
		reslist = reslist->ai_next;
	}	
	freeaddrinfo(res);

	if(addr.empty() && addr6.empty())
		return &out;
	
	std::random_device rd;
	std::mt19937 gen(rd());

	if(addr.size() >= 2)
		std::shuffle(addr.begin(), addr.end(), gen);

	if(addr6.size() >= 2)
		std::shuffle(addr6.begin(), addr6.end(), gen);

	while(!addr.empty())
	{
		ip_port_addr ip4;
		ip4.is_ip4 = true;
		ip4.ip4 = addr.back();

		connect(out, ip4);

		if(out.sock != INVALID_SOCKET)
			return &out;

		addr.pop_back();
	}

	while(!addr6.empty())
	{
		ip_port_addr ip6;
		ip6.is_ip4 = false;
		ip6.ip4 = addr.back();

		connect(out, ip6);

		if(out.sock != INVALID_SOCKET)
			return &out;

		addr6.pop_back();
	}
	return &out;
}

out_peer_factory::sock_data* out_peer_factory::connect_ip(sock_data &out, const char *saddr, uint16_t port)
{
	ip_port_addr addr4, addr6;

	if(inet_pton(AF_INET, saddr, &(addr4.ip4.sin_addr)) == 1)
	{
		addr4.ip4.sin_family = AF_INET;
		addr4.ip4.sin_port = htons(port);
		connect(out, addr4);
	}
	else if(inet_pton(AF_INET6, saddr, &(addr6.ip6.sin6_addr)) == 1)
	{
		addr6.ip6.sin6_family = AF_INET6;
		addr6.ip6.sin6_port = htons(port);
		connect(out, addr6);
	}
	return &out;
}

out_peer_factory::sock_data* out_peer_factory::connect(sock_data &out, ip_port_addr addr)
{
	SOCKET peer_fd = socket(addr.is_ip4 ? PF_INET : PF_INET6, SOCK_STREAM, 0);

	if(peer_fd == INVALID_SOCKET)
		return &out;

	if(addr.is_ip4)
	{
		if(::connect(peer_fd, (sockaddr *)&addr.ip4, sizeof(sockaddr_in)) == 0)
		{
			out.sock = peer_fd;
			out.addr = addr;
			return &out;
		}
	}
	else
	{
		if(::connect(peer_fd, (sockaddr *)&addr.ip6, sizeof(sockaddr_in6)) == 0)
		{
			out.sock = peer_fd;
			out.addr = addr;
			
			return &out;
		}
	}

	//connect failed
	close(peer_fd);
	return &out;
}

void out_peer_factory::connect_peers(size_t n)
{
	sock_data s;
	connect_dns(s, "127.0.0.1:12210");
	if(s.sock != INVALID_SOCKET)
	{	
		peers.emplace_back(s.sock, s.addr);
		peers.back().send_handshake();
	}
}

void out_peer_factory::async_connect_wait(std::list<std::future<sock_data*>>& thds)
{

	auto it = thds.begin();
	while (it != thds.end())
	{
		if(it->wait_for(std::chrono::milliseconds(50)) == std::future_status::ready)
		{
			sock_data* sd = it->get();
			if(sd->sock != INVALID_SOCKET)
				peers.emplace_back(sd->sock, sd->addr);

			thds.erase(it);
			break;
		}
		else
			it++;
	}
}

void out_peer_factory::connect_seeds()
{
	std::list<std::future<sock_data*>> conthd;
	std::vector<sock_data> dat;
	size_t done_seeds = 0;

	dat.reserve(countof(dns_seeds));
	while(done_seeds < countof(dns_seeds))
	{
		dat.emplace_back();
		conthd.emplace_back(std::async(std::launch::async, &out_peer_factory::connect_dns, std::ref(dat.back()), dns_seeds[done_seeds]));

		if(conthd.size() >= MAX_HALFOPEN)
			async_connect_wait(conthd);
	}

	while(conthd.size() > 0)
		async_connect_wait(conthd);
}

void out_peer_factory::stop_peers()
{
	for(auto &a : peers)
		a.close();
}
