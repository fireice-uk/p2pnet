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
#include <iostream> 
#include <iomanip> 
#include <string> 
#include "p2p/p2p_protocol_defs.h"
#include "serialization/serialization.h"
#include "serialization/binary_archive.h"
#include "cryptonote_basic/blobdatatype.h"
#include "epee/storages/portable_storage_template_helper.h"
#include <boost/uuid/uuid_generators.hpp>
#include "cryptonote_basic/blobdatatype.h"

int main()
{

	srand(std::time(NULL));
	wsock_init();
	
	char inter[128];
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
	//outfac.connect_peers(1);
	
	sleep(0.5f);
	std::cin.ignore(1024, '\n');
	std::cout << "Press enter to continue..." << std::endl;
	std::cin.get();
	
	incfac.stop_peers();
	incfac.stop();
	outfac.stop_peers();
   	
	wsock_cleaup();

	return 0;
}
