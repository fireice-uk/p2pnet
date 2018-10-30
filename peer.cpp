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

		if(reclen <= 0)
		{
			//CONNECTION LOST
			std::cout << "PEER: " << peer_fd << " DISCONNECTED" << std::endl;
			::close(peer_fd);
			peer_fd = INVALID_SOCKET;
			break;
		}

		std::cout << "RECEIVED DATA: " << (char *)buffer.get() << " LENGTH: " << reclen << std::endl;
	}
  constexpr size_t buflen = 256 * 1024;
  constexpr size_t msglen = 1024;
  std::unique_ptr<uint8_t[]> buffer(new uint8_t[buflen]);
  std::unique_ptr<uint8_t[]> msg(new uint8_t[msglen]);
  size_t bufpos = 0;

  while(peer_fd != INVALID_SOCKET)
  {
    //int reclen = recv(peer_fd, (char*)buffer.get()+bufpos, buflen-bufpos, 0);
    int reclen = recv(peer_fd, (char*)msg.get(), msglen, 0);
 

    if(reclen == 0)
    {
      //CONNECTION LOST
      std::cout << "PEER: " << peer_fd << " DISCONNECTED" << std::endl;
      ::close(peer_fd);
      peer_fd = INVALID_SOCKET;
      break;
    }
    else if(reclen < 0)
    {
      //ERROR
      std::cout << "RECV ERROR - PEER: " << peer_fd << std::endl;
      ::close(peer_fd);
      peer_fd = INVALID_SOCKET;
      break;
    }
    else
    {	
      while(false /*serialzie(msg) == false*/)
      {	
	//Read more data
	reclen += recv(peer_fd, (char*)msg.get()+reclen, msglen-reclen, 0);
      }
           
      if(reclen + bufpos < buflen)
      {
	std::cout << "RECEIVED MESSAGE: " << (char*)msg.get() << " LENGTH: " << reclen << std::endl;
	memcpy((char*)buffer.get()+bufpos, (char*)msg.get(), reclen);
	bufpos += reclen;
      }
      else
	std::cout << "CAN NOT APPEND MESSAGE: " << (char*)msg.get() << " BUFFER OVERFLOW" << std::endl;
    }
  }
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
