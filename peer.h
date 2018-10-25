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

#include "smart_socket.h"

class peer
{
public:
  
	//REFACTORING
	peer(SOCKET _peer_fd, const sockaddr_in* addr); 
	peer(SOCKET _peer_fd, const sockaddr_in6* addr); 
	void closesocket();
	void add_send_data(const char *data, int len);
  
	peer(smart_socket *_ssocket);
	void stop();

protected:
	
	//REFACTORING
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

	
	smart_socket* ssocket;

	bool quit = false;

private:
};
#endif /* PEER_H */
