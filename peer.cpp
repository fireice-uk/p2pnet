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
	proto_header headptr;
	
	int current_command = 0;
	size_t current_data_len = 0;

 	while(peer_fd != INVALID_SOCKET)
	{	
		int reclen = recv(peer_fd, (char *)buffer.get() + bufpos, buflen - bufpos, 0);
		std::cout << "RECV: " << reclen << std::endl;
		bufpos += reclen;

		if(reclen <= 0)
		{
			//CONNECTION LOST
			std::cout << "PEER: " << peer_fd << " DISCONNECTED" << std::endl;
			close();
			peer_fd = INVALID_SOCKET;
			break;
		}
		
		while(bufpos > 0)
		{	
			if(current_data_len == 0 && bufpos >= sizeof(proto_header))
			{	
				//COPY DATA TO HEADER
				memcpy(&headptr, buffer.get(), sizeof(proto_header));		
				//DISPATCH HEADER DATA AND COPY WHATS BEHIND TO FRONT OF BUFFER
				if(bufpos > sizeof(proto_header))
					memcpy(buffer.get(), buffer.get() + sizeof(proto_header),  (bufpos -= sizeof(proto_header)));
				else
					bufpos -= sizeof(proto_header);
				  
				current_command = headptr.m_command;
				current_data_len = headptr.m_datal_len;
				
				call_map_mutex.lock();
				if(current_command != call_map.begin()->second.m_command)
				{
					//WRONG COMMAND IGNORE FOR NOW 
					//close();
				}
				call_map_mutex.unlock();
			}
			
			if(current_data_len != 0 && bufpos >= current_data_len)
			{	
				switch(current_command)
				{
					case nodetool::COMMAND_REQUEST_SUPPORT_FLAGS::ID:
					{
						nodetool::COMMAND_REQUEST_SUPPORT_FLAGS::response res;
							
						//COPY DATA TO STRUCT
						memcpy(&res, buffer.get(), current_data_len);	
						//DISPATCH STRUCT DATA AND COPY WHATS BEHIND TO FRONT OF BUFFER
						if(bufpos > current_data_len)
							memcpy(buffer.get(), buffer.get() + current_data_len,  (bufpos -= current_data_len));
						else
							bufpos -= current_data_len;
						
						std::cout << "RECEIVED SUPPORT FLAG: " << res.support_flags << std::endl;
						
						current_data_len = 0;
					}
					break;
					
					case nodetool::COMMAND_HANDSHAKE_T<CORE_SYNC_DATA>::ID:
					{
						//COPY BINARY DATA TO BUFFER
						std::string buff_to_rec((const char*)buffer.get(), current_data_len);
						
						call_map_mutex.lock();
						call_map.begin()->second.promise->set_value(buff_to_rec);
						call_map.erase(call_map.begin());
						call_map_mutex.unlock();
						
						//DISPATCH STRUCT DATA AND COPY WHATS BEHIND TO FRONT OF BUFFER//COPY DATA TO STRUCT
						if(bufpos > current_data_len)
							memcpy(buffer.get(), buffer.get() + current_data_len,  (bufpos -= current_data_len));
						else
							bufpos -= current_data_len;
						
						current_data_len = 0;
					}
					break;
					
					default:
					{
						bufpos -= current_data_len;
						std::cout << "UNKNOWN COMMAND TYPE: " << current_command << " - DISPATCHING " << current_data_len << " OF DATA" << std::endl;
						current_data_len = 0;
					}	
					break;
				}
			}
		}
	}
}

void peer::send_handshake()
{
	proto_header sendh;
	//const unsigned char* sendd;
	std::string sendd;
	std::vector<uint8_t> sendv;
	
	nodetool::COMMAND_HANDSHAKE_T<CORE_SYNC_DATA>::request arg;
	//nodetool::COMMAND_HANDSHAKE_T::response rsp;
	
	epee::serialization::portable_storage stg;
	
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
	
	arg.store(stg);
	stg.store_to_binary(sendd);
	//sendd = (unsigned char *)&stg;
	int sendd_size = sendd.length();
	
	sendh.m_signature = LEVIN_SIGNATURE;
	sendh.m_datal_len = sendd_size;
	sendh.m_have_to_return_data = true;
	sendh.m_command = nodetool::COMMAND_HANDSHAKE_T<CORE_SYNC_DATA>::ID;
	sendh.m_return_code = LEVIN_OK;
	sendh.m_flags = LEVIN_PACKET_REQUEST;
	sendh.m_protocol_version = LEVIN_PROTOCOL_VER_1;	
	
	
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
	if(t_invoke.joinable())
		t_invoke.join();
	if(peer_fd != INVALID_SOCKET)
		::close(peer_fd);
	peer_fd = INVALID_SOCKET;
}

void peer::do_invoke()
{	
	t_invoke = std::thread(&peer::invoke_thread, this);
}

void peer::invoke_thread()
{		
	std::string response;
	
	std::promise<std::string>* recv_promise = new std::promise<std::string>;//(call_map.end()->second.promise); 
	call_id++;
	call_map_mutex.lock();
	future_resp fr;
	fr.m_command = nodetool::COMMAND_HANDSHAKE_T<void>::ID;
	fr.promise = recv_promise;
	call_map.insert(call_map.end(), std::pair<int, future_resp>(call_id, fr) );
	call_map_mutex.unlock();
	
	
	//TODO OTHER COMMANDS
	send_handshake();
	
	std::future<std::string> recv_future = recv_promise->get_future();
	response = recv_future.get();
	delete recv_promise;
	
	nodetool::COMMAND_HANDSHAKE_T<CORE_SYNC_DATA>::response res;
	epee::serialization::portable_storage stg_ret;
	
	//UNSERIALIZE BINARY DATA
	if(!stg_ret.load_from_binary(response))
	{
		std::cout << "FAILED TO LOAD BINARY ON COMMAND: " << nodetool::COMMAND_HANDSHAKE_T<void>::ID << std::endl;
	}
	
	res.load(stg_ret);
	
	std::cout << "RECEIVED HANDSHAKE: " << std::endl;
	std::cout << "PEER ID: " << res.node_data.peer_id << std::endl;
	std::cout << "PORT: " << res.node_data.my_port << std::endl;
	std::cout << "LOCAL TIME: " << res.node_data.local_time << std::endl;
	std::cout << "NETWORK ID: " << res.node_data.network_id << std::endl;
	
	std::cout << "CURRENT HEIGHT: " << res.payload_data.current_height << std::endl;
	std::cout << "CUM DIFF: " << res.payload_data.cumulative_difficulty << std::endl;
	std::cout << "TOP ID: " << res.payload_data.top_id.bytes << std::endl;
	std::cout << "TOP VERSION: " << res.payload_data.top_version << std::endl;
	
	
	std::cout << "LOCAL PEERLIST: " << std::endl;
	for(auto a: res.local_peerlist_new)
	{	
		std::cout << "PEER ID: " << a.id << std::endl;
		std::cout << "LAST SEEN: " << a.last_seen << std::endl;
		std::cout << "PEER IP:PORT: " << a.adr.str() << std::endl;
	}
}
