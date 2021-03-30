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

struct Peer {
	std::string ip;
	unsigned short port;
	TcpSocket* socket;
};

std::vector<Peer> peers;

sf::Packet& operator <<(sf::Packet& _packet, const Peer& _struct)
{
	return _packet << _struct.ip << _struct.port;
}
sf::Packet& operator>> (sf::Packet& _packet, Peer& _struct)
{
	return _packet >> _struct.ip >> _struct.port;
}

int main()
{
	TcpSocket* socket = new TcpSocket();
	std::string portBSS = "";

	Socket::Status status = socket->connect("127.0.0.1", 5000);

	if (status != Socket::Done) {
		cout << "Error connecting to BSS server" << endl;
	}
	else {
		cout << "Connected to the BSS server" << endl;
		portBSS = std::to_string(socket->getLocalPort());
		cout << "The port: " << portBSS << endl;
		//Recivir todos los datos, de los clientes que hay en partida. 
		//Desconectarse del BSS. 
		//socket->disconnect();
		//TcpSocket* clientNuevo = new TcpSocket();
		//Socket::Status status = clientNuevo->connect("192.168.1.61", 5001);
		//peers.push_back(clientNuevo);
	}
	Packet packet;
	Socket::Status status3 = socket->receive(packet);
	if (status3 != Socket::Done) {
		cout << "Fuck no resivoh." << endl;
	}
	else {
		cout << "RECIBIDO" << endl;
		int size;
		packet >> size;
		cout << "Tamaño de conexiones: " << size << endl;
		for (size_t i = 0; i < size; i++)
		{
			Peer peer;
			packet >> peer;
			cout << "PEER: " << "IP: " << peer.ip << "PORT: " << peer.port << endl;
			peers.push_back(peer);
		}
		cout << "Numero de conexiones: " << peers.size() << endl;
	}
	for (size_t i = 0; i < peers.size(); i++)
	{
		peers[i].socket = new TcpSocket();
		if (peers[i].socket->connect(peers[i].ip, peers[i].port) != Socket::Done) {
			cout << "Conexion fallada" << endl;
		}
		else {
			cout << "Conexion completada" << endl;
		}
	}
	/// <summary>
	/// El cliente ya esta conectado y espera a nuevas entradas. 
	/// </summary>
	/// <returns></returns>
	TcpListener listener;		
	if (listener.listen(listener.getLocalPort()) != Socket::Done) {
		cout << "Puerto no disponible" << endl;
	}
	else {
		cout << "Puerto abierto" << endl;
	}
	while (true) {
		TcpSocket* socket = new TcpSocket();
		if (listener.accept(*socket) != Socket::Done) {
			cout << "Error al conectarse" << endl;
		}
		else {
			Peer peerNuevo;
			peerNuevo.ip = socket->getRemoteAddress().toString();
			peerNuevo.port = socket->getRemotePort();
			peerNuevo.socket = socket;
			peers.push_back(peerNuevo);
			cout << "Nuevo peer entrante: " << "IP: " << peerNuevo.ip << endl;
		}
	}
	return 0;
}
