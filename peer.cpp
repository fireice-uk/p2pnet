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
		
		//RECV HEADER
		while(reclen < sizeof(proto_header))
		{
			reclen += recv(peer_fd, (char *)buffer.get()+bufpos, sizeof(proto_header)-reclen, 0);
			bufpos += reclen;
		} 
		
		//TODO UNSERIALIZE
		proto_header recvh;
		memcpy(&recvh, (buffer.get() + bufpos - sizeof(recvh)), sizeof(recvh));
				
		if(recvh.m_datal_len + bufpos >= buflen)
		{
			std::cout << "ERROR RECEIVE DATA OVERFLOW: " << recvh.m_datal_len << std::endl;
			//drop current message
			break;
		}
		
		//READ DATA
		reclen = 0;
		while(reclen < recvh.m_datal_len)
		{
			reclen += recv(peer_fd, (char *)buffer.get() + bufpos, recvh.m_datal_len-reclen, 0);
			bufpos += reclen;
		}
		
		char cmsg[recvh.m_datal_len+1];
		memcpy(&cmsg, buffer.get() + bufpos - recvh.m_datal_len, recvh.m_datal_len);
		cmsg[recvh.m_datal_len] = '\0';
		std::cout << "RECEIVED DATA: " << cmsg << std::endl;
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
