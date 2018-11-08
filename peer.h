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


#include <boost/uuid/uuid_io.hpp> 
#include <inttypes.h>
#include <iostream>
#include <mutex>
#include <queue>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <thread>
#include <vector>
#include <cstdlib>
#include <time.h>
#include <unistd.h>
#include <sstream>
#include <iomanip> 
#include <string> 
#include <bitset>
#include <future>
#include "p2p/p2p_protocol_defs.h"
#include "serialization/serialization.h"
#include "serialization/binary_archive.h"
#include "cryptonote_basic/blobdatatype.h"
#include "epee/storages/portable_storage_template_helper.h"
#include <boost/uuid/uuid_generators.hpp>
#include "cryptonote_basic/blobdatatype.h"
#include "epee/storages/portable_storage_template_helper.h"
#include "sock.h"
#include "thdq.hpp"

//Bender's nightmare
#define LEVIN_OK 0
#define LEVIN_PROTOCOL_VER_1 1


//using namespace std;
struct key
{
	unsigned char &operator[](int i)
	{
		return bytes[i];
	}
	unsigned char operator[](int i) const
	{
		return bytes[i];
	}
	bool operator==(const key &k) const { return !memcmp(bytes, k.bytes, sizeof(bytes)); }
	unsigned char bytes[32];
};

struct CORE_SYNC_DATA
{
	uint64_t current_height;
	uint64_t cumulative_difficulty;
	key top_id;
	uint8_t top_version;

	BEGIN_KV_SERIALIZE_MAP()
	KV_SERIALIZE(current_height)
	KV_SERIALIZE(cumulative_difficulty)
	KV_SERIALIZE_VAL_POD_AS_BLOB(top_id)
	KV_SERIALIZE_OPT(top_version, (uint8_t)0)
	END_KV_SERIALIZE_MAP()
};


class peer
{
public:

	peer(SOCKET peer_fd, const ip_port_addr& addr);
	void close();
	void send_data(std::vector<uint8_t> &&data) { sendq.push(std::move(data)); }
	void send_handshake();
	void do_invoke();

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
	
	struct future_resp
	{	
		uint32_t m_command;
		std::promise<std::string>* promise; 
	};
#pragma pack(pop)

	static constexpr uint64_t LEVIN_SIGNATURE = 0x0101010101012101LL;
	static constexpr uint8_t LEVIN_PACKET_REQUEST =  0x00000001;
	//static constexpr uint64_t LEVIN_SIGNATURE = 0x01011101;

	SOCKET peer_fd;
	ip_port_addr addr;
	std::thread t_send;
	std::thread t_recv;
	std::thread t_invoke;
	
	thdq<std::vector<uint8_t>> sendq;
	
	int call_id = 0;
	std::map<int, future_resp> call_map;
	
	std::mutex call_map_mutex;

	void send_thread();
	void recv_thread();
	void invoke_thread();

  private: 
};
#endif /* PEER_H */
