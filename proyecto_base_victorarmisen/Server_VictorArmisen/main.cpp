#include <iostream>
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <thread>
#include <mutex>
#include <vector>
#include <time.h>

using std::cout;
using std::endl;

//void TratarTodosLosClientes(std::vector<std::string>* jugadores) {
//	while (true) {
//		for (size_t i = 0; i < jugadores->size(); i++)
//		{
//			cout << "Jugador " << " " << i << ": " << jugadores->at(i) << endl;
//		}
//		std::this_thread::sleep_for(std::chrono::seconds(5));
//	}
//}
//void TratarCliente(std::string nick, std::vector<std::string>* jugadores) {
//	for (size_t i = 0; i < 10; i++)
//	{
//		cout << "Jugador con el nombre: " << nick << endl;
//		std::this_thread::sleep_for(std::chrono::seconds(1));
//	}
//	mtxJugadores.lock();
//	for (size_t i = 0; i < jugadores->size(); i++)
//	{
//		if (jugadores->at(i) == nick) {
//			jugadores->erase(jugadores->begin() + i);
//			break;
//		}
//	}
//	mtxJugadores.unlock();
//}

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

int main() 
{
	//std::string nick;
	//std::vector<std::string> jugadores;
	//std::thread tAll(TratarTodosLosClientes, &jugadores);

	sf::TcpListener dispacher; 
	sf::TcpSocket incoming;

	sf::Socket::Status status = dispacher.listen(5000);

	if (status != sf::Socket::Done) {
		//No se ha podido vincular. 
		cout << "No se ha podido vincular el puerto" << endl;
	}
	else {
		cout << "Vinculado" << endl;
	}

	while (true) 
	{
		if (dispacher.accept(incoming) != sf::Socket::Done) {
			cout << "Error al conectarse" << endl;
		}
		else {
			cout << "Conectado jugador con IP" << incoming.getRemoteAddress() << endl;
			sf::Packet packet;
			packet << jugadores.size();
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
			jugadores.push_back({ incoming.getRemoteAddress().toString(), incoming.getRemotePort() });
			cout << "Numero de jugadores conectados" << jugadores.size() << endl;
		}
	}

	//sf::Packet packet;

	//sf::Socket::Status status2 = incoming.receive(packet);
	//
	//if (status != sf::Socket::Done) {
	//	cout << "El paquete no se ha recibido" << endl;
	//}
	//else {
	//	std::string data; 
	//	packet >> data;
	//	cout << "Info received from the player: " << data << endl;
	//}

	//do {
	//	//std::cin >> nick;
	//	//if (nick == "exit") break;
	//	//jugadores.push_back(nick);
	//	//std::thread t(TratarCliente, nick, &jugadores);
	//	//t.detach();
	//} while (true);
	/*
	Thread pal: While(true) añadimos un jugador cuando se conecte. 
	Funcion Tratar cliente(nick)-> Imprime 10 veces el nombre del jugador. Y despues hace erase. 
	Thread -> Jugadores conectados ahora mismo. Print todos los jugadores. 
	*/
	return 0;
}