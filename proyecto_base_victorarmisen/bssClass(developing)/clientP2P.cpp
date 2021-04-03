#include "clientP2P.h"

sf::Packet& operator <<(sf::Packet& _packet, const clientP2P::Peer& _struct)
{
	return _packet << _struct.ip << _struct.port;
}
sf::Packet& operator>> (sf::Packet& _packet, clientP2P::Peer& _struct)
{
	return _packet >> _struct.ip >> _struct.port;
}



//Functions
clientP2P::clientP2P()
{
	TcpSocket* socket = new TcpSocket();
	std::string portBSS = "";


}

clientP2P::~clientP2P()
{
}

void clientP2P::Run()
{

}
