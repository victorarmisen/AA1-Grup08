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
#define MAX_PLAYERS 2

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

void Chat() {

	sf::Vector2i screenDimensions(800, 600);
	sf::RenderWindow window;
	window.create(sf::VideoMode(screenDimensions.x, screenDimensions.y), "Chat");

	sf::String mensaje = " >";

	sf::Font font;
	if (!font.loadFromFile("calibri.ttf"))
	{
		std::cout << "Can't load the font file" << std::endl;
	}

	sf::Text chattingText(mensaje, font, 14);
	chattingText.setFillColor(sf::Color(0, 160, 0));
	chattingText.setStyle(sf::Text::Bold);

	sf::Text text(mensaje, font, 14);
	text.setFillColor(sf::Color(0, 160, 0));
	text.setStyle(sf::Text::Bold);
	text.setPosition(0, 560);

	sf::RectangleShape separator(sf::Vector2f(800, 5));
	separator.setFillColor(sf::Color(200, 200, 200, 255));
	separator.setPosition(0, 550);


	//char buffer[100];
	//size_t bytesReceived;

	Packet msgPacket;

	while (window.isOpen())
	{
		sf::Event evento;
		while (window.pollEvent(evento))
		{
			switch (evento.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				if (evento.key.code == sf::Keyboard::Escape)
					window.close();
				else if (evento.key.code == sf::Keyboard::Return)
				{
					std::string messageToSend = mensaje;

					//------------------ Send ------------------
					msgPacket << messageToSend;

					for (int i = 0; i < peerSockets.size(); i++)
					{				
						peerSockets[i]->send(msgPacket);
					}
					mensaje = ">";
				}
				break;
			case sf::Event::TextEntered:
				if (evento.text.unicode >= 32 && evento.text.unicode <= 126)
					mensaje += (char)evento.text.unicode;
				else if (evento.text.unicode == 8 && mensaje.getSize() > 0)
					mensaje.erase(mensaje.getSize() - 1, mensaje.getSize());
				break;
			}
		}
		window.draw(separator);

		//------------------ Receive------------------

		Packet arrivePacket;

		for (int i = 0; i < peerSockets.size(); i++)
		{
			Socket::Status status = peerSockets[i]->receive(msgPacket);

			if (status != Socket::Status::Done) {
				
			}
			else {
				std::string chatting;
				msgPacket >> chatting;
				cout << "El contenido del paquete es: " << chatting << endl;
				chattingText.setPosition(sf::Vector2f(0, 20 * i));
				chattingText.setString(chatting);
			}
		}
		window.draw(text);
		window.display();
		window.clear();
	}

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

	for (int i = 0; i < peerSockets.size(); i++)
	{
		peerSockets[i]->setBlocking(false);
	}

	Chat();

	return 0;
}

