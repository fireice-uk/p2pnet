#include "p2p.h"

void p2p::do_invoke()
{
	out_peer_fac.connect_peers(1);
}


void p2p::close()
{
	out_peer_fac.stop_peers();
	inc_peer_fac.stop_peers();
	inc_peer_fac.stop();
}
