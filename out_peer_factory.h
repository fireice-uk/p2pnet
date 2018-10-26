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
public:
	
	//REFACTORING
	out_peer_factory();
	static SOCKET connect(const char* full_addr);
	static SOCKET connect(const char *saddr, uint16_t port);
	static SOCKET connect(sockaddr_in addr);
	static SOCKET  connect(sockaddr_in6 addr);
	
	void connect_peers(size_t n) {}
	
protected:
      
	
private:
};

#endif /* OUT_PEER_FACTORY_H */
