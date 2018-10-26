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


peer::peer(SOCKET _peer_fd, const sockaddr_in *addr) : peer_fd(_peer_fd), addr4(*addr)
{
    t_send = std::thread([&]() {this->send_thread(); });
    t_recv = std::thread([&]() {this->recv_thread(); });

    if (t_send.joinable()) t_send.detach();
    if (t_recv.joinable()) t_recv.detach();
}

peer::peer(SOCKET _peer_fd, const sockaddr_in6 *addr) : peer_fd(_peer_fd), addr6(*addr)
{
    t_send = std::thread([&]() {this->send_thread(); });
    t_recv = std::thread([&]() {this->recv_thread(); });

    if (t_send.joinable()) t_send.detach();
    if (t_recv.joinable()) t_recv.detach();
}

void peer::send_thread()
{
	//TODO SEND DATA

	while (!quit)
	{
		if (!send_data.empty())
		{
			send_mutex.lock();

			int len, sdlen;
			char *buff = send_data.front();

			memcpy(&len, buff, sizeof(len));

			sdlen =  send(peer_fd, buff, (len + sizeof(len)), 0);

			while(sdlen < len+sizeof(len))
			{
			  //TODO SOME DATA LEFT UNSEND
			  sdlen += send(peer_fd, (char*)buff[sdlen], (len + sizeof(len) - sdlen), 0);
			}
			send_data.pop();

			send_mutex.unlock();
		}
	}
}

void peer::recv_thread()
{
	//TODO RECV DATA
	int reclen;
	char buffer[1024];
	while (!quit)
	{
		reclen = recv(peer_fd, buffer, 1024, 0);
		if (reclen > 0)
		{
			recv_mutex.lock();

			char* tmpbuffer = new char[reclen];

			memcpy(tmpbuffer, buffer, reclen);
			recv_data.push(tmpbuffer);

			//EXAMPLE TO DISPLAY AND DISPATCH DATA
			char *readbuffer = recv_data.back();
			int readlen;
			memcpy(&readlen, readbuffer, sizeof(readlen));
			char *readdata = new char[readlen+1];
			memcpy(readdata, &readbuffer[sizeof(readlen)], readlen);
			readdata[readlen] = '\0';
			std::cout << "RECEIVED DATA: " << readdata << " LENGTH: " << readlen << std::endl;
			//END OF EXAMPLE

			recv_mutex.unlock();
		}
		else if (reclen == 0)
		{
			//CONNECTION LOST
			std::cout << "PEER: " << peer_fd << " DISCONNECTED" << std::endl;
		}
		else
		{
			//ERROR
			std::cout << "RECV ERROR - PEER: " << peer_fd << std::endl;
		}
	}
}

void peer::add_send_data(const char *data, int len)
{	
	
	std::thread t1 = std::thread([&](const char *_data, int _len) {this->add_send_data_thread(_data, _len); }, data, len);
	if (t1.joinable()) t1.detach();
}

void peer::add_send_data_thread(const char *data, int len)
{
	send_mutex.lock();

	char *buff = new char[len + sizeof(len)];

	memcpy(buff, &len, sizeof(len)); //PASS DATA LENGTH SO WE DON NEED TO STORY IT EXTERNALLY
	memcpy(&buff[sizeof(len)], data, len); //PASS DATA

	send_data.push(buff);

	send_mutex.unlock();
}

void peer::closesocket()
{	
      quit = true;
      close(peer_fd);
}