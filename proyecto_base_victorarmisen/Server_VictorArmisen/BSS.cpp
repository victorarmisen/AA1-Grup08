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

#define MAX_PLAYERS 2
#define LISTEN_PORT 5001

struct Data {
	sf::String ip;
	sf::Uint16 port;
};

std::vector<Data> peers;

sf::Packet& operator <<(sf::Packet& _packet, const Data& _struct) {
	return _packet << _struct.ip << _struct.port;
}
sf::Packet& operator>> (sf::Packet& _packet, Data& _struct) {
	return _packet >> _struct.ip >> _struct.port;
}

int main() {

	std::vector<Data> direcciones;

	sf::TcpListener listener;
	std::cout << "Escuchando a la espera de jugadores..." << endl;

	sf::Socket::Status status = listener.listen(LISTEN_PORT);

	if (status != sf::Socket::Status::Done)
	{
		std::cout << "No se ha podido escuchar por el puerto" << LISTEN_PORT << std::endl;
	}
	else
	{
		for (int i = 0; i < MAX_PLAYERS; i++)
		{
			sf::TcpSocket sock;
			if (listener.accept(sock) != sf::Socket::Status::Done) {
				std::cout << "No se ha podido aceptar la conexión " << std::endl;
			}
			else
			{
				std::cout << "Conectado con: " << sock.getRemoteAddress() << ":" << sock.getRemotePort() << std::endl;
				sf::Packet packet;
				packet << (sf::Int8)direcciones.size();
				for (int dir = 0; dir < direcciones.size(); dir++)
				{
					packet << direcciones[dir];
				}
				sock.send(packet);
				direcciones.push_back({ sock.getRemoteAddress().toString(), sock.getRemotePort() });
				sock.disconnect();
			}
		}

	}
	listener.close();
	system("pause");
	return 0;
}