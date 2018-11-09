#ifndef P2P_H
#define P2P_H

#include "inc_peer_factory.h"
#include "out_peer_factory.h"
#include "p2p/p2p_protocol_defs.h"

#pragma once

class p2p
{
	public:
	
		p2p() {};
	  
		inline static p2p& inst()
		{
			static p2p inst;
			return inst;
		}
		
		inline static const p2p& cinst()
		{
			static const p2p& inst = p2p::inst();
			return inst;
		}
		
		void do_invoke();
		void close();
		
	protected:
		
		out_peer_factory out_peer_fac;
		inc_peer_factory inc_peer_fac;  
		
	private:
};

#endif //P2P_H