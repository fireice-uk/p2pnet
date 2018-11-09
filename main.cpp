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

#include "p2p.h"
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
	
	std::cout << "Press enter to connect to ryod" << std::endl;
	//std::cin.ignore(1024, '\n');
	std::cin.get();
	
	p2p::inst().do_invoke();
	
	std::cout << "Press enter to quit" << std::endl;
	//std::cin.ignore(1024, '\n');
	std::cin.get();
	
	p2p::inst().close();
	
	wsock_cleaup();
	
	std::cin.get();

	return 0;
}
