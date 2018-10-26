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

#include <cstdlib>
#include <unistd.h>
#include <time.h>
#include "inc_peer_factory.h"
#include "out_peer_factory.h"

//using namespace std;

int main()
{	
	srand(std::time(NULL));
	wsock_init();
	
	//TODO REMOVE INPUTS PASSED AT START
	//std::cin.ignore();
	
	char inter[128];
	char *interptr = inter;
	std::cout << "Listen Interface(0 for null): ";
	std::cin >> inter;
	
	if (inter[0] == '0')
		interptr = nullptr;

	int port;
	std::cout << "Listen Port: ";
	std::cin >> port;
	inc_peer_factory incfac;
	incfac.start(interptr, port);
	
	sleep(1);
	std::cin.ignore(1024, '\n');
	std::cout << "Press enter to continue..." << std::endl;
	std::cin.get();

	char inter2[128];
	std::cout << "Connect Interface: ";
	std::cin >> inter2;

	//int port2;
	//std::cout << "Connect Port: ";
	//std::cin >> port2;

	out_peer_factory outfac;
	outfac.connect(inter2/*, port2*/);

	sleep(1);
	std::cin.ignore(1024, '\n');
	std::cout << "Press enter to continue..." << std::endl;
	std::cin.get();
	
	incfac.stop();
	incfac.join_accept_thread();

	wsock_cleaup();


	return 0;
}
