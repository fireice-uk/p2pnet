/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   peer.cpp
 * Author: benqgg
 * 
 * Created on October 25, 2018, 3:21 PM
 */

#include "peer.h"

peer::peer(SOCKET peer_fd, const ip_port_addr& addr) : peer_fd(peer_fd), addr(addr)
{
	t_send = std::thread(&peer::send_thread, this);
	t_recv = std::thread(&peer::recv_thread, this);
}

void peer::send_thread()
{
	while(sendq.wait_for_pop())
	{
		std::vector<uint8_t> data = sendq.pop();

		if(peer_fd == INVALID_SOCKET)
			return;

		size_t sent = 0;
		while(sent < data.size())
		{
			int sdlen = send(peer_fd, (char *)(&data.front() + sent), data.size() - sent, 0);

			if(sdlen <= 0)
				return;

			sent += sdlen;
		}
	}
}

void peer::recv_thread()
{
	constexpr size_t buflen = 256 * 1024;
	std::unique_ptr<uint8_t[]> buffer(new uint8_t[buflen]);
	size_t bufpos = 0;
	proto_header *headptr = nullptr;

	while(peer_fd != INVALID_SOCKET)
	{
		int reclen = recv(peer_fd, (char *)buffer.get() + bufpos, buflen - bufpos, 0);
		bufpos += reclen;

		if(reclen <= 0)
		{
			//CONNECTION LOST
			std::cout << "PEER: " << peer_fd << " DISCONNECTED" << std::endl;
			::close(peer_fd);
			peer_fd = INVALID_SOCKET;
			break;
		}
		
		
		if(bufpos >= sizeof(proto_header) && headptr == nullptr)
		{	
			
			headptr = new proto_header;
			memcpy(headptr, buffer.get(), sizeof(proto_header));
			std::cout << "DATA LEN: " << headptr->m_datal_len << std::endl;

			memcpy(buffer.get(), buffer.get() + sizeof(proto_header),  (bufpos -= sizeof(proto_header)));
		}
		
		if(headptr != nullptr)
		{	
			if(bufpos >= headptr->m_datal_len)
			{
				char msg[headptr->m_datal_len+1];
				memcpy(&msg, buffer.get(), headptr->m_datal_len);
				msg[headptr->m_datal_len] = '\0';
				std::cout << "RECEIVED DATA: " << msg << std::endl;
				
				memcpy(buffer.get(), buffer.get() + headptr->m_datal_len,  (bufpos -= headptr->m_datal_len));
				
				delete headptr;
				headptr = nullptr;
			}
		}
	}
}

void peer::send_handshake()
{
	proto_header sendh;
	char sendd[10] = "123456789";
	std::vector<uint8_t> sendv;
	
	sendh.m_signature = 0;
	sendh.m_datal_len = 10;
	sendh.m_have_to_return_data = false;
	sendh.m_command = 0;
	sendh.m_return_code;
	sendh.m_flags = 0;
	sendh.m_protocol_version = 0;
	
	//PACK HEADER
	uint8_t *data_p = reinterpret_cast<uint8_t *>(&sendh);
	while(sendv.size() < sizeof(sendh))
	{
		sendv.push_back(*data_p);
		data_p++;
	}
	
	//PACK DATA
	while(sendv.size() < sizeof(sendh) + sendh.m_datal_len)
	{
		sendv.push_back(sendd[sendv.size() - sizeof(sendh)]);
	}
	
	send_data(std::move(sendv));
}

void peer::close()
{
	shutdown(peer_fd, SD_BOTH);
	sendq.set_finish_flag();
	if(t_send.joinable())
		t_send.join();
	if(t_recv.joinable())
		t_recv.join();
	if(peer_fd != INVALID_SOCKET)
		::close(peer_fd);
	peer_fd = INVALID_SOCKET;
}
