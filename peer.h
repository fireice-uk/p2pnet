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

#include <sys/types.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <queue>
#include <stdio.h>
#include <inttypes.h>
#include <thread>
#include <mutex>

#include "sock.h"
#include "thdq.hpp"

class peer
{
public:
  
	//REFACTORING
	peer(SOCKET _peer_fd, const sockaddr_in* addr); 
	peer(SOCKET _peer_fd, const sockaddr_in6* addr);
	void close();
	void send_data(std::vector<uint8_t>&& data) { sendq.push(std::move(data)); }

protected:
	
	//REFACTORING
	SOCKET peer_fd;
	sockaddr_in ip4_addr;
	sockaddr_in6 ip6_addr;
	std::thread t_send;
	std::thread t_recv;

	thdq<std::vector<uint8_t>> sendq;

	void send_thread();
	void recv_thread();
	
private:
};
#endif /* PEER_H */
