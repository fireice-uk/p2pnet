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
		std::cout << "RECV: " << reclen << std::endl;
		bufpos += reclen;

		if(reclen <= 0)
		{
			//CONNECTION LOST
			std::cout << "PEER: " << peer_fd << " DISCONNECTED" << std::endl;
			::close(peer_fd);
			peer_fd = INVALID_SOCKET;
			break;
		}
		
		while(bufpos > 0)
		{
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
}

void peer::send_handshake()
{
	proto_header sendh;
	const unsigned char* sendd;
	std::vector<uint8_t> sendv;
	
	nodetool::COMMAND_HANDSHAKE_T<CORE_SYNC_DATA>::request arg;
	//nodetool::COMMAND_HANDSHAKE_T::response rsp;
	
	time_t local_time;
	time(&local_time);
	arg.node_data.local_time = local_time;
	arg.node_data.peer_id = rand() %  100;
	arg.node_data.my_port = 0;
	arg.node_data.network_id =  { { 0xcd, 0xac, 0x50, 0x2e, 0xb3, 0x74, 0x8f, 0xf2, 0x0f, 0xb7, 0x72, 0x18, 0x0f, 0x73, 0x24, 0x13 } }; 
	
	//GET PAYLOAD SYNC DATA
	arg.payload_data.top_id = boost::value_initialized<key>();
	arg.payload_data.top_version = 0;
	arg.payload_data.cumulative_difficulty = 1;
	arg.payload_data.current_height = 0;
	
	
	sendd = (unsigned char *)&arg;
	int sendd_size = sizeof(arg);
	
	sendh.m_signature = LEVIN_SIGNATURE;
	sendh.m_datal_len = sendd_size;
	sendh.m_have_to_return_data = true;
	sendh.m_command = nodetool::COMMAND_HANDSHAKE_T<CORE_SYNC_DATA>::ID;
	sendh.m_return_code = LEVIN_OK;
	sendh.m_flags = LEVIN_PACKET_REQUEST;
	sendh.m_protocol_version = LEVIN_PROTOCOL_VER_1;
	
	
	// Fill characters 
	std::ostringstream op;
	std::ostream::fmtflags old_flags = op.flags();  
	char old_fill  = op.fill(); 
	op << std::hex << std::setfill('0'); 
	
	for (int i = 0; i < sendd_size; i++) 
	{ 
	    // Give space between two hex values 
	    if (i>0) 
		op << ' '; 
	    if(i % 8 == 0)
	      op << "\n";
	    // force output to use hex version of ascii code 
	    op << std::setw(2) << static_cast<int>(sendd[i]); 
	} 
      
	op.flags(old_flags); 
	op.fill(old_fill);
	
	std::cout << "DATA LENGTH: " << sendd_size << " : " << std::endl << op.str() << std::endl << std::endl;
	
	//PACK HEADER
	uint8_t *data_p = reinterpret_cast<uint8_t *>(&sendh);
	while(sendv.size() < sizeof(sendh))
	{
		sendv.push_back(*data_p);
		data_p++;
	}
	
	//PACK DATA
	while(sendv.size() < sizeof(sendh) + sendd_size)
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
