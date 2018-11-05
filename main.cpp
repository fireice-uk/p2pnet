/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: benqgg
 *
 * Created on October 25, 2018, 3:13 PM
 */

#include "inc_peer_factory.h"
#include "out_peer_factory.h"
#include <cstdlib>
#include <time.h>
#include <unistd.h>
#include <sstream>
#include "p2p/p2p_protocol_defs.h"
#include "serialization/serialization.h"
#include "serialization/binary_archive.h"
#include "cryptonote_basic/blobdatatype.h"
#include "epee/storages/portable_storage_template_helper.h"
#include <boost/uuid/uuid_generators.hpp>
#include "cryptonote_basic/blobdatatype.h"



//using namespace std;
struct key
{
	unsigned char &operator[](int i)
	{
		return bytes[i];
	}
	unsigned char operator[](int i) const
	{
		return bytes[i];
	}
	bool operator==(const key &k) const { return !memcmp(bytes, k.bytes, sizeof(bytes)); }
	unsigned char bytes[32];
};

struct CORE_SYNC_DATA
{
	uint64_t current_height;
	uint64_t cumulative_difficulty;
	key top_id;
	uint8_t top_version;

	BEGIN_KV_SERIALIZE_MAP()
	KV_SERIALIZE(current_height)
	KV_SERIALIZE(cumulative_difficulty)
	KV_SERIALIZE_VAL_POD_AS_BLOB(top_id)
	KV_SERIALIZE_OPT(top_version, (uint8_t)0)
	END_KV_SERIALIZE_MAP()
};


int main()
{

	srand(std::time(NULL));
	wsock_init();
	
	/*
	nodetool::COMMAND_HANDSHAKE_T<std::string> cmhs;
	std::stringstream ss;
	binary_archive<true> ba(ss);
	bool r = ::serialization::serialize(ba, const_cast<nodetool::COMMAND_HANDSHAKE_T<std::string> &>(cmhs));
	if(r)
	{
		std::cout  <<  ss.str() << std::endl;; 
	}*/
	
	nodetool::COMMAND_HANDSHAKE_T<CORE_SYNC_DATA>::request arg;
	//nodetool::COMMAND_HANDSHAKE_T::response rsp;
	
	time_t local_time;
	time(&local_time);
	arg.node_data.local_time = local_time;
	arg.node_data.peer_id = 0;
	arg.node_data.my_port = 1111;
	arg.node_data.network_id = boost::uuids::random_generator()();
	
	//GET PAYLOAD SYNC DATA
	CORE_SYNC_DATA hshd = boost::value_initialized<CORE_SYNC_DATA>();
	//NULL HASH
	arg.payload_data.top_id = boost::value_initialized<key>();
	arg.payload_data.top_version = 0;
	arg.payload_data.cumulative_difficulty = 1;
	arg.payload_data.current_height = 1;
	
	/*char inter[128];
	char *interptr = inter;
	std::cout << "Listen Interface(0 for null): ";
	std::cin >> inter;

	if(inter[0] == '0')
		interptr = nullptr;

	int port;
	std::cout << "Listen Port: ";
	std::cin >> port;
	inc_peer_factory incfac;
	//incfac.start(interptr, port);


	sleep(0.5f);
	std::cin.ignore(1024, '\n');
	std::cout << "Press enter to continue..." << std::endl;
	std::cin.get();
	
	char inter2[128];
	std::cout << "Connect Interface: ";
	std::cin >> inter2;
	
	int port2;
	std::cout << "Connect Port: ";
	std::cin >> port2;
	
	out_peer_factory outfac;
	outfac.connect_peers(1);
	
	sleep(0.5f);
	std::cin.ignore(1024, '\n');
	std::cout << "Press enter to continue..." << std::endl;
	std::cin.get();
	
	incfac.stop_peers();
	incfac.stop();
	outfac.stop_peers();
    */
	wsock_cleaup();

	return 0;
}
