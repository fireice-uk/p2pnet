/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   inc_peer_facotry.h
 * Author: benqgg
 *
 * Created on October 25, 2018, 3:17 PM
 */

#ifndef INC_PEER_FACOTRY_H
#define INC_PEER_FACOTRY_H

#pragma once

#include "peer.h"


class inc_peer_factory
{
public:
	
	//Refactoring
	inc_peer_factory();
	~inc_peer_factory();
	void start(const char *saddr = nullptr, uint16_t port = 0, int backlog = 10);
	void stop();


protected:
	//Refactoring
	SOCKET listen_fd = INVALID_SOCKET;
	std::thread accept_thread;

	void thread_accept();
	void thread_create_peer(int peer_fd, sockaddr_in6 *saddr);

private:
	
};

#endif /* INC_PEER_FACOTRY_H */
