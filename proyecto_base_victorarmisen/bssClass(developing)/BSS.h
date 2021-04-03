#pragma once
#include <iostream>
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <thread>
#include <mutex>
#include <vector>
#include <time.h>

using std::cout;
using std::endl;

class BSS
{

public:
	BSS();
	~BSS();
	void Run();

	struct Peer {
		std::string ip;
		unsigned short port;
	};

private:
	
	std::vector<Peer> jugadores;

	//Network Variables
	sf::TcpListener dispacher;
	sf::TcpSocket incoming;

	//Server control variables
	bool server_running;


	


	

};

