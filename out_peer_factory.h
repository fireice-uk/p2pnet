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


class out_peer_factory
{
private:
	struct sock_data
	{
		SOCKET sock = INVALID_SOCKET;
		sockaddr_in addr4;
		sockaddr_in6 addr6;
		bool ip4;
	};

public:
	
	//REFACTORING
	out_peer_factory();
	static void connect(sock_data& out, const char* full_addr);
	static void connect(sock_data& out, const char *saddr, uint16_t port);
	static void connect(sock_data& out, sockaddr_in addr);
	static void connect(sock_data& out, sockaddr_in6 addr);
	
	void connect_peers(size_t n);
	void stop_peers();
	
protected:

	std::vector<peer> peers;
};

#endif /* OUT_PEER_FACTORY_H */
