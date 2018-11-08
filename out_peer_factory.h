/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   out_peer_factory.h
 * Author: benqgg
 *
 * Created on October 25, 2018, 3:19 PM
 */

#ifndef OUT_PEER_FACTORY_H
#define OUT_PEER_FACTORY_H

#pragma once

#include "peer.h"
#include <future>

template <typename T, std::size_t N>
constexpr std::size_t countof(T const (&)[N]) noexcept
{
	return N;
}

static const char *dns_seeds[] = {
	"127.0.0.1:3333"};

class out_peer_factory
{
public:
	out_peer_factory() {};

	void connect_peers(size_t n);
	void connect_seeds();
	void stop_peers();

private:
	struct sock_data
	{
		SOCKET sock = INVALID_SOCKET;
		ip_port_addr addr;
	};

	static sock_data* connect_dns(sock_data &out, const char *full_addr);
	static sock_data* connect_ip(sock_data &out, const char *saddr, uint16_t port);
	static sock_data* connect(sock_data &out, ip_port_addr addr);

	void async_connect_wait(std::list<std::future<sock_data*>>& thds);
	std::list<peer> peers;
};

#endif /* OUT_PEER_FACTORY_H */
