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
	inc_peer_factory(const char *ip = nullptr, uint16_t port = 0, int backlog = 10);
	~inc_peer_factory();
	inline void join_accept_thread() { if (accept_thread.joinable()) accept_thread.join(); };
	void stop();


protected:
  
	//Refactoring
	SOCKET listen_fd;
	std::thread accept_thread;
	bool _stop = false; 
	void call_listen(const char *_ip = nullptr, uint16_t port = 0, int backlog = 10);
	void thread_accept();
	void thread_create_peer(int peer_fd);
	
	void thread_create_peer(smart_socket* ss);
	smart_socket *listen_socket;

private:
};

#endif /* INC_PEER_FACOTRY_H */
