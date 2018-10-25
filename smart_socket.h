/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   smart_socket.h
 * Author: benqgg
 *
 * Created on October 25, 2018, 3:22 PM
 */

#ifndef SMART_SOCKET_H
#define SMART_SOCKET_H

#pragma once

#include "address_container.h"


class smart_socket
{
public:
	smart_socket();
	smart_socket(SOCKET _sockdid, address_container *_addr, address_container *_peer_addr);

	void smart_close();
	bool smart_listen(const char *_ip = nullptr, uint16_t port = 0, int backlog = 10);
	smart_socket* smart_accept();
	smart_socket* smart_connect(const char *ip, uint16_t port);
	smart_socket* smart_connect(const char *full_addr);
    
	//TODO SEND I RECV
	int smart_send(const char* data, int len);
	int smart_recv(char *buffer, int len);

protected:

	int sockid = -1;
	address_container *addr = nullptr;
	address_container *peer_addr = nullptr;

	int try_to_connect(sockaddr *_addr, size_t len); //

private:
};
#endif /* SMART_SOCKET_H */
