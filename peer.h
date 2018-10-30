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

#include <inttypes.h>
#include <iostream>
#include <mutex>
#include <queue>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <thread>
#include <vector>

#include "sock.h"
#include "thdq.hpp"

class peer
{
public:
	peer(SOCKET peer_fd, const ip_port_addr& addr);
	void close();
	void send_data(std::vector<uint8_t> &&data) { sendq.push(std::move(data)); }

protected:
#pragma pack(push)
#pragma pack(1)
	struct proto_header
	{
		uint64_t m_signature;
		uint64_t m_datal_len;
		bool m_have_to_return_data;
		uint32_t m_command;
		int32_t m_return_code;
		uint32_t m_flags;
		uint32_t m_protocol_version;
	};
#pragma pack(pop)

	static constexpr uint64_t LEVIN_SIGNATURE = 0x0101010101012101LL;

	SOCKET peer_fd;
	ip_port_addr addr;
	std::thread t_send;
	std::thread t_recv;

	thdq<std::vector<uint8_t>> sendq;

	void send_thread();
	void recv_thread();

  private:
};
#endif /* PEER_H */
