#include <iostream>
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <thread>
#include <mutex>
#include <vector>
#include <Windows.h>
#include <time.h>

using std::cout;
using std::endl;
using namespace sf;

#define BSS_IP "localhost"
#define BSS_PORT 50000
#define MAX_PLAYERS 3

std::vector<TcpSocket*> peerSockets;
unsigned short ownPort;

struct Data {
	sf::String ip;
	Uint16 port;
};

std::vector<Data> peers;

sf::Packet& operator <<(sf::Packet& _packet, const Data& _struct) {
	return _packet << _struct.ip << _struct.port;
}
sf::Packet& operator>> (sf::Packet& _packet, Data& _struct) {
	return _packet >> _struct.ip >> _struct.port;
}

int main()
{
	TcpSocket socket;
	std::string ipSS = BSS_IP;

	if (socket.connect(IpAddress(ipSS), BSS_PORT) != Socket::Done) {
		cout << "Error connecting to BSS server" << endl;
	} else {
		cout << "Connected to BSS SERVER" << endl;
	}

	ownPort = socket.getLocalPort();

	/// <summary>
	/// Recivir el contenido del paquete que nos envia el BSS.
	/// </summary>
	/// <returns></returns>

	sf::Packet packet;
	if (socket.receive(packet) != Socket::Done) 
		cout << "Error en la recepción del paquete " << endl;
	else 
		cout << "Paquete Recivido. " << endl;	
	

	socket.disconnect(); 

	Int8 size = 0;
	packet >> size;
	for (int i = 0; i < size; i++)
	{
		Data d = {};
		packet >> d;
		peers.push_back(d);
	}

	/// <summary>
	/// Conectarse con los jugadores existentes en la partida.
	/// </summary>
	/// <returns></returns>
	/// 
	cout << "Conectando con los jugadores de la sala" << endl;
	for (int i = 0; i < peers.size(); i++)
	{
		sf::TcpSocket* newSocket = new sf::TcpSocket();
		Socket::Status status = newSocket->connect(sf::IpAddress(peers[i].ip), (unsigned short)peers[i].port);
		if (status != Socket::Status::Done) {
			cout << "Error al conectarse con el peer: " << peers[i].ip.toAnsiString() << ":" << peers[i].port << endl;
		}
		else {
			cout << "Conexion completada con el peer:  " << peers[i].ip.toAnsiString() << ":" << peers[i].port << endl;
		}
		peerSockets.push_back(newSocket);
	}

	/// <summary>
	/// Escucha de nuevos clientes por venir a la partida.
	/// </summary>
	/// <returns></returns>

	TcpListener listener;

	cout << "Escuchando a nuevos clientes" << endl;

	if (listener.listen(ownPort) != Socket::Done) //ERROR ESTAVA AQUI!!!!
		cout << "Error al hacer el bind al puerto" << endl;
	else {
		cout << "Bind completado!" << endl;
		for (int i = peers.size(); i < MAX_PLAYERS - 1; i++)
		{
			TcpSocket* incoming = new TcpSocket;
			if (listener.accept(*incoming) != Socket::Done)
				cout << "Error al aceptar el nuevo cliente" << endl;
			else {
				cout << "Nuevo cliente aceptado con ip y puerto: " << incoming->getRemoteAddress() << incoming->getRemotePort() <<  endl;
				peerSockets.push_back(incoming);
			}
		}
	}

	return 0;
}

