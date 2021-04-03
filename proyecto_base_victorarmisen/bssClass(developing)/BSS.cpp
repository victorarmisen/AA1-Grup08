#include "BSS.h"



sf::Packet& operator <<(sf::Packet& _packet, const BSS::Peer& _struct)
{
	return _packet << _struct.ip << _struct.port;
}
sf::Packet& operator>> (sf::Packet& _packet, BSS::Peer& _struct)
{
	return _packet >> _struct.ip >> _struct.port;
}

BSS::BSS() {
	sf::Socket::Status status = dispacher.listen(5000);

	if (status != sf::Socket::Done) {
		//No se ha podido vincular. 
		std::cout << "No se ha podido vincular el puerto" << std::endl;
	}
	else {
		std::cout << "Vinculado" << std::endl;
	}
	server_running = false;
}

BSS::~BSS()
{
}

void BSS::Run()
{
	while (true)
	{
		if (dispacher.accept(incoming) != sf::Socket::Done) {
			cout << "Error al conectarse" << endl;
		}
		else {
			cout << "Conectado jugador con IP" << incoming.getRemoteAddress() << endl;
			jugadores.push_back({ incoming.getRemoteAddress().toString(), incoming.getRemotePort() });
			cout << "Numero de jugadores conectados" << jugadores.size() << endl;
			sf::Packet packet;
			int size = jugadores.size();
			packet << size;
			cout << "TAMAÑO " << jugadores.size() << endl;
			if (jugadores.size() == 0) {
				cout << "No hay jugadores todavia" << endl;
			}
			else {
				for (size_t i = 0; i < jugadores.size(); i++)
				{
					packet << jugadores[i];
				}
			}
			sf::Socket::Status statusCliente = incoming.send(packet);
			if (statusCliente != sf::Socket::Done) {
				cout << "Fuck" << endl;
			}
			else {
				cout << "PAQUETE enviado." << endl;
			}
			incoming.disconnect();
		}
	}	
}

	
