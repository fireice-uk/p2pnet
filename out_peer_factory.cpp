// 
/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   out_peer_factory.cpp
 * Author: benqgg
 * 
 * Created on October 25, 2018, 3:19 PM
 */

#include "out_peer_factory.h"

out_peer_factory::out_peer_factory()
{
	//CHECK FOR ERROR
}

void out_peer_factory::connect_dns(sock_data& out, const char* full_addr)
{	
//RESOLVE DNS
  char _addr[256];
  char *_port;
  size_t len;
  len = strlen(full_addr);
  if (len > sizeof(_addr))
  {
    //ERROR, FULL ADDRESS TOO LONG
    std::cout << "DNS TOO LONG - OUT PEER FAC" << std::endl;
    
  }
  else
  {
    memcpy(&_addr, full_addr, len);
    
    if ((_port = strchr(_addr, ':')) == nullptr)
    {
      //ERROR, NO PORT INCLUDED

      std::cout << "DNS LACK PORT OR WRONG FORMAT - OUT PEER FAC" << std::endl;
    }
    else
    {
      _port[0] = '\0';
      
      _port++;
      struct addrinfo hint;
      struct addrinfo *res = nullptr;
      hint = { 0 };
	
      hint.ai_family = AF_UNSPEC;			
      hint.ai_socktype = SOCK_STREAM;		
      //hint.ai_flags = AI_PASSIVE;
      hint.ai_protocol = 0;	
      int err = getaddrinfo(_addr, _port, &hint, &res);
      if (err != 0)	
      {
	std::cout << "GET ADDR INFO ERROR OUTPEER FACE " << _addr << " : " << _port << std::endl; 
      }

			
      addrinfo *reslist = res;
      std::vector<sockaddr_in> addr;
      std::vector<sockaddr_in6> addr6;
      while (reslist != nullptr)		
      {		
	switch (reslist->ai_family)			
	{		
	  case AF_INET:				
	    addr.push_back(*reinterpret_cast<sockaddr_in*>(reslist->ai_addr));				
	    break;
	    
	  case AF_INET6:			
	    addr6.push_back(*reinterpret_cast<sockaddr_in6*>(reslist->ai_addr));				
	    break;				
	}
	reslist = reslist->ai_next;
      }
      freeaddrinfo(res);
			
      if (addr.empty() && addr6.empty())	
      {
	//ERROR NO IPV4 O IPV6 ADDRESSES
	std::cout << "DNS CONTAINS NO IPV4 OR IPV6 ADDRESSES - OUT_PEER_FAC" << std::endl;		
      }
      
      else
      {
	if(!addr.empty())
	{	
	  int n = rand() % addr.size();
	  connect4(out, addr[n]);
	}
	else if(!addr6.empty())
	{	
	  int n = rand() % addr6.size();
	  connect6(out, addr6[n]);
	}
      }
    }
  }
}

void out_peer_factory::connect_ip(sock_data& out, const char *saddr, uint16_t port)
{
  sockaddr_in6 _addr6 = { 0 };
  sockaddr_in _addr4 = { 0 };

  if (inet_pton(AF_INET, saddr, &(_addr4.sin_addr)) == 1)
  {
    _addr4.sin_family = AF_INET;
    _addr4.sin_port = htons(port);

    connect4(out, _addr4);

  }
  else if (inet_pton(AF_INET6, saddr, &(_addr6.sin6_addr)) == 1)
  {
    _addr6.sin6_family = AF_INET6;
    _addr6.sin6_port = htons(port);
	    
    connect6(out, _addr6); 
  }
  else
  {
    std::cout << "GIVEN ADDRESS IS NEITHER IPV4 OR IPV6 - OUT PEER FAC" << std::endl;
  }
}

void out_peer_factory::connect4(sock_data& out, sockaddr_in addr)
{
  SOCKET peer_fd = socket(PF_INET, SOCK_STREAM, 0);

  if(peer_fd == INVALID_SOCKET)
  {
    std::cout << "SOCKET ERROR - OUT PEER FAC" << std::endl;
  }
  else
  {	
    if(::connect(peer_fd, (sockaddr*)&addr, sizeof(sockaddr_in)) == 0)
    {
      std::cout << "CONNECTED" << std::endl;
      out.ip4 = true;
      out.addr4 = addr;
      out.sock = peer_fd;
    }
    else
    {	
      std::cout << "CAN NOT CONNECT" << std::endl;
    }
  }
}

void out_peer_factory::connect6(sock_data& out, sockaddr_in6 addr)
{
  SOCKET peer_fd = socket(PF_INET6, SOCK_STREAM, 0);

  if(peer_fd == INVALID_SOCKET)
  {
    std::cout << "SOCKET ERROR - OUT PEER FAC" << std::endl;
  }
  else
  {
    if(::connect(peer_fd, (sockaddr*)&addr, sizeof(sockaddr_in6)) == 0)
    {
      std::cout << "CONNECTED" << std::endl;
      out.ip4 = false;
      out.addr6 = addr;
      out.sock = peer_fd;
    }
    else
    {	
      std::cout << "CAN NOT CONNECT" << std::endl;
    }
  }
}

void out_peer_factory::connect_peers(size_t n)
{
  sock_data s;
  connect_dns(s, "localhost:1111");
  if(s.sock != INVALID_SOCKET)
  {
    if(s.ip4)
      peers.emplace_back(s.sock, &s.addr4);
    else
      peers.emplace_back(s.sock, &s.addr6);
  }
}

void out_peer_factory::connect_seeds()
{
  std::list<std::thread> conthd;
  std::list<sock_data> dat;
  size_t done_seeds = 0;

  while(done_seeds < countof(dns_seeds))
  {
    dat.emplace_back();
    conthd.emplace_back(&out_peer_factory::connect_dns, dat.back(), dns_seeds[done_seeds]);
    
    if(conthd.size() >= MAX_HALFOPEN)
    {
      conthd.front().join();
      const sock_data& res = dat.front();

      if(res.sock != INVALID_SOCKET)
      {
	if(res.ip4)
	  peers.emplace_back(res.sock, &res.addr4);
	else
	  peers.emplace_back(res.sock, &res.addr6);
      }
      
      conthd.pop_front();
      dat.pop_front();
    }
  }

  while(conthd.size() > 0)
  {
    conthd.front().join();
    const sock_data& res = dat.front();

    if(res.sock != INVALID_SOCKET)
    {
      if(res.ip4)
	peers.emplace_back(res.sock, &res.addr4);
      else
	peers.emplace_back(res.sock, &res.addr6);
    }
    
    conthd.pop_front();
    dat.pop_front();
  }
}
void out_peer_factory::stop_peers()
{
  for(auto &a : peers)
    a.close();
}
