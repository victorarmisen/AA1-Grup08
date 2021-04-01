#pragma once
#include <iostream>
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <thread>
#include <mutex>
#include <vector>
#include <time.h>


class BSS
{
private:
	struct Peer {
		std::string ip;
		unsigned short port;
	};

	std::vector<Peer> jugadores;

	sf::Packet& operator <<(sf::Packet& _packet, const Peer& _struct)
	{
		return _packet << _struct.ip << _struct.port;
	}
	sf::Packet& operator>> (sf::Packet& _packet, Peer& _struct)
	{
		return _packet >> _struct.ip >> _struct.port;
	}


};

