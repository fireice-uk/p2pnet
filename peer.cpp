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
	std::string buff_to_rec;
	size_t current_data_len = 0;

 	while(peer_fd != INVALID_SOCKET)
	{	
		int reclen = recv(peer_fd, (char *)buffer.get() + bufpos, buflen - bufpos, 0);
		std::cout << "RECV: " << reclen << std::endl;
		bufpos += reclen;

		if(reclen <= 0)
		{
			//CONNECTION LOST
			std::cout << "PEER: " << peer_fd << " DISCONNECTEDzzz" << std::endl;
			::close(peer_fd);
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
				  
				current_data_len = headptr.m_datal_len;
			}
			
			if(current_data_len != 0 && bufpos >= current_data_len)
			{	
				//COPY DATA TO BUFFER
				buff_to_rec.clear();
				buff_to_rec.assign((const char*)buffer.get(), current_data_len);
				
				//DISPATCH STRUCT DATA AND COPY WHATS BEHIND TO FRONT OF BUFFER
				if(bufpos > current_data_len)
					memcpy(buffer.get(), buffer.get() + current_data_len,  (bufpos -= current_data_len));
				else
					bufpos -= current_data_len;
				
				switch(headptr.m_command)
				{
					case nodetool::COMMAND_REQUEST_SUPPORT_FLAGS::ID:
					{			  
						if(headptr.m_flags == LEVIN_PACKET_RESPONSE)
						{
							call_map_mutex.lock();
							if(call_map.empty() || call_map.begin()->second->m_command != headptr.m_command)
							{
								//WE ARE NOT WAITING FOR RESPONSE, OR WE GOT WRONG COMMAND AS RESPONSE
								std::cout << "PEER: " << peer_fd << " DISCONNECTEDaaa" << std::endl;
								::close(peer_fd);
								peer_fd = INVALID_SOCKET;
							}
							else
							{	
								//PASS DATA TO A PROMISE
								call_map.begin()->second->prom.set_value(buff_to_rec);
								call_map.erase(call_map.begin());
							}
							call_map_mutex.unlock();
						}
						else if(headptr.m_flags == LEVIN_PACKET_REQUEST)
						{
							//SEND RESPONSE
							if(headptr.m_have_to_return_data)
							{
								get_invoke(headptr.m_command, buff_to_rec);
							}
						}
					}
					break;
					
					case nodetool::COMMAND_HANDSHAKE_T<void>::ID:
					{						  
						if(headptr.m_flags == LEVIN_PACKET_RESPONSE)
						{	
							call_map_mutex.lock();
							if(call_map.empty() || call_map.begin()->second->m_command != headptr.m_command)
							{
								//WE ARE NOT WAITING FOR RESPONSE, OR WE GOT WRONG COMMAND AS RESPONSE
								std::cout << "PEER: " << peer_fd << " DISCONNECTEDaaa" << std::endl;
								::close(peer_fd);
								peer_fd = INVALID_SOCKET;
							}
							else
							{	
								//PASS DATA TO A PROMISE
								call_map.begin()->second->prom.set_value(buff_to_rec);
								call_map.erase(call_map.begin());
							}
							call_map_mutex.unlock();
						}
						else if(headptr.m_flags == LEVIN_PACKET_REQUEST)
						{
							//SEND RESPONSE
							if(headptr.m_have_to_return_data)
							{
								get_invoke(headptr.m_command, buff_to_rec);
							}
						}
						else
						{
							//UNKNOWN FLAG
							std::cout << "PEER: " << peer_fd << " DISCONNECTEDxxx" << std::endl;
							::close(peer_fd);
							peer_fd = INVALID_SOCKET;
						}
					}
					break;
					
					default:
						if(headptr.m_flags == LEVIN_PACKET_REQUEST && !headptr.m_have_to_return_data)
						{
							get_notify(buff_to_rec);
						}
						else
						{
							std::cout << "UNKNOWN COMMAND TYPE: " << headptr.m_command << " - DISPATCHING " << buff_to_rec.length() << " OF DATA" << std::endl;
							//std::cout << "PEER: " << peer_fd << " DISCONNECTED" << std::endl;
							//::close(peer_fd);
							//peer_fd = INVALID_SOCKET;
						}
					break;
				}
				current_data_len = 0;
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

void peer::receive_handshake(std::string req)
{	
	proto_header sendh;
	//const unsigned char* sendd;
	std::string sendd;
	std::vector<uint8_t> sendv;
	
	nodetool::COMMAND_HANDSHAKE_T<CORE_SYNC_DATA>::request arg;
	nodetool::COMMAND_HANDSHAKE_T<CORE_SYNC_DATA>::response rsp;
	
	epee::serialization::portable_storage stg_req;
	epee::serialization::portable_storage stg_resp;
	
	if(!stg_req.load_from_binary(req))
	{
		std::cout << "FAILED TO LOAD BINARY ON COMMAND: " << nodetool::COMMAND_HANDSHAKE_T<void>::ID << std::endl;
	}
	arg.load(stg_req);
	
	time_t local_time;
	time(&local_time);
	rsp.node_data.local_time = local_time;
	rsp.node_data.peer_id = rand() %  100;
	rsp.node_data.my_port = 0;
	rsp.node_data.network_id =  { { 0xcd, 0xac, 0x50, 0x2e, 0xb3, 0x74, 0x8f, 0xf2, 0x0f, 0xb7, 0x72, 0x18, 0x0f, 0x73, 0x24, 0x13 } }; 
	
	//GET PAYLOAD SYNC DATA
	rsp.payload_data.top_id = boost::value_initialized<key>();
	rsp.payload_data.top_version = 0;
	rsp.payload_data.cumulative_difficulty = 1;
	rsp.payload_data.current_height = 0;
	
	rsp.local_peerlist_new.clear();
	//TODO PASS PEER LIST TO RSP	
	
	rsp.store(stg_resp);
	stg_resp.store_to_binary(sendd);
	int sendd_size = sendd.length();
	
	sendh.m_signature = LEVIN_SIGNATURE;
	sendh.m_datal_len = sendd_size;
	sendh.m_have_to_return_data = false;
	sendh.m_command = nodetool::COMMAND_HANDSHAKE_T<CORE_SYNC_DATA>::ID;
	sendh.m_return_code = LEVIN_OK;
	sendh.m_flags = LEVIN_PACKET_RESPONSE;
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

void peer::receive_support_flag_request(std::string req)
{
	proto_header sendh;
	//const unsigned char* sendd;
	std::string sendd;
	std::vector<uint8_t> sendv;
	
	nodetool::COMMAND_REQUEST_SUPPORT_FLAGS::request arg;
	nodetool::COMMAND_REQUEST_SUPPORT_FLAGS::response rsp;
	
	epee::serialization::portable_storage stg_req;
	epee::serialization::portable_storage stg_resp;
	
	if(!stg_req.load_from_binary(req))
	{
		std::cout << "FAILED TO LOAD BINARY ON COMMAND: " << nodetool::COMMAND_REQUEST_SUPPORT_FLAGS::ID << std::endl;
	}
	arg.load(stg_req);
	
	time_t local_time;
	time(&local_time);
	rsp.support_flags = rand() % 100000;	//TODO PASS SUPOPRT FLAGS
	
	rsp.store(stg_resp);
	stg_resp.store_to_binary(sendd);
	int sendd_size = sendd.length();
	
	sendh.m_signature = LEVIN_SIGNATURE;
	sendh.m_datal_len = sendd_size;
	sendh.m_have_to_return_data = false;
	sendh.m_command = nodetool::COMMAND_REQUEST_SUPPORT_FLAGS::ID;
	sendh.m_return_code = LEVIN_OK;
	sendh.m_flags = LEVIN_PACKET_RESPONSE;
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
	if(peer_fd != INVALID_SOCKET)
		::close(peer_fd);
	peer_fd = INVALID_SOCKET;
}

void peer::do_invoke(int command)
{	
	std::string response;
	std::promise<std::string> prom;
	std::future<std::string> future_resp;
	epee::serialization::portable_storage stg_ret;
	
	call_id++;
		
	promise_resp pr(command);
	call_map_mutex.lock();
	call_map.insert(call_map.end(), std::pair<int, promise_resp*>(call_id, &pr) );
	future_resp = call_map.rbegin()->second->prom.get_future();
	call_map_mutex.unlock();
	
	if(command == nodetool::COMMAND_HANDSHAKE_T<void>::ID)
	{	
		std::cout << "INVOKING COMMAND HANDSHAKE" << std::endl;
		nodetool::COMMAND_HANDSHAKE_T<CORE_SYNC_DATA>::response res;
	  
		send_handshake();
		
		response = future_resp.get();
		
		//UNSERIALIZE BINARY DATA
		if(!stg_ret.load_from_binary(response))
		{
			std::cout << "FAILED TO LOAD BINARY ON COMMAND: " << command << std::endl;
			return;
		}
		
		res.load(stg_ret);
		
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
			//std::cout << "PEER ID: " << a.id << std::endl;
			//std::cout << "LAST SEEN: " << a.last_seen << std::endl;
			//std::cout << "PEER IP:PORT: " << a.adr.str() << std::endl;
		}
	}
	else if(command == nodetool::COMMAND_REQUEST_SUPPORT_FLAGS::ID)
	{
		std::cout << "INVOKING COMMAND REQUEST SUPPORT FLAGS" << std::endl;
		nodetool::COMMAND_REQUEST_SUPPORT_FLAGS::response res;
	  
		send_support_flags_request();
		
		response = future_resp.get();
		
		//UNSERIALIZE BINARY DATA
		if(!stg_ret.load_from_binary(response))
		{
			std::cout << "FAILED TO LOAD BINARY ON COMMAND: " << command << std::endl;
			return;
		}
		res.load(stg_ret);
		std::cout << "RECEIVED RESPONSE SUPPORT FLAGS: " << res.support_flags << std::endl;
	}
}

void peer::do_notify(int command, std::string req)
{
	proto_header sendh;
	std::string sendd;
	std::vector<uint8_t> sendv;	
	epee::serialization::portable_storage stg;
	
	//TO DO SERIALIZE REQ BASED ON COMMAND
	switch(command)
	{
		case 0:
			/*req.store(stg);
			stg.store_to_binary(sendd);*/
		break;
		  
		case 1:
		break;
	}
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

void peer::get_notify(std::string req)
{
	std::cout << "INCOMING NOTIFY OF: " << req.length() << " bytes" << std::endl;
}

void peer::send_support_flags_request()
{	
	proto_header sendh;
	//const unsigned char* sendd;
	std::string sendd;
	std::vector<uint8_t> sendv;
	
	nodetool::COMMAND_REQUEST_SUPPORT_FLAGS::request arg;
	
	epee::serialization::portable_storage stg;
	
	arg.store(stg);
	stg.store_to_binary(sendd);
	//sendd = (unsigned char *)&stg;
	int sendd_size = sendd.length();
	
	sendh.m_signature = LEVIN_SIGNATURE;
	sendh.m_datal_len = sendd_size;
	sendh.m_have_to_return_data = true;
	sendh.m_command = nodetool::COMMAND_REQUEST_SUPPORT_FLAGS::ID;
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

void peer::get_invoke(int command, std::string req)
{	
	if(command == nodetool::COMMAND_HANDSHAKE_T<void>::ID)
	{
		std::cout << "RECEIVED HANDSHAKE REQUEST" << std::endl;
		receive_handshake(req);
	}
	else if(command == nodetool::COMMAND_REQUEST_SUPPORT_FLAGS::ID)
	{
		std::cout << "RECEIVED SUPPORT FLAGS REQUEST" << std::endl;
		receive_support_flag_request(req);
	}
}