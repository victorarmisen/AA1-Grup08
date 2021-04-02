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
using namespace sf;


class clientP2P
{
public:
	struct Peer {
		std::string ip;
		unsigned short port;
		TcpSocket* socket;
	};

	clientP2P();
	~clientP2P();

	void Run();

private:
	std::vector<Peer> peers;

	TcpSocket* socket;
	std::string portBSS;
};

