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
#define BSS_PORT 5001
#define MAX_PLAYERS 2

std::vector<TcpSocket*> peerSockets;
unsigned short ownPort;

struct Data {
	sf::String ip;
	Uint16 port;
};

enum Familia {Arabe, Bantu, China, Esquimal, India, Mexicana, Tirolesa};
enum Tipo {Abuelo, Abuela, Padre, Madre, Hijo, Hija};

struct Carta {
	Familia familia;
	Tipo tipo;
};

std::vector<Carta> cartas;

std::vector<Data> peers;

sf::Packet& operator <<(sf::Packet& _packet, const Data& _struct) {
	return _packet << _struct.ip << _struct.port;
}
sf::Packet& operator>> (sf::Packet& _packet, Data& _struct) {
	return _packet >> _struct.ip >> _struct.port;
}

std::string DecirFamilia(Familia familia) {
	std::string nombre = "";
	switch (familia) {
	case 0: nombre = "Arabe";
		break;
	case 1: nombre = "Bantu";
		break;
	case 2: nombre = "China";
		break;
	case 3: nombre = "Esquimal";
		break;
	case 4: nombre = "India";
		break;
	case 5: nombre = "Mexicana";
		break;
	case 6: nombre = "Tirolesa";
		break;
	}
	return nombre;
}

std::string DecirTipo(Tipo familia) {
	std::string nombre = "";
	switch (familia) {
		case 0: nombre = "Abuelo";
			break;
		case 1: nombre = "Abuela";
			break;
		case 2: nombre = "Padre";
			break;
		case 3: nombre = "Madre";
			break;
		case 4: nombre = "Hijo";
			break;
		case 5: nombre = "Hija";
			break;
	}
	return nombre;
}

//std::vector<std::string> Split(std::string msg) {
//	int init = 0;
//	int end = 0;
//	std::vector<std::string> palabras;
//	while (end = msg.find(" ", init), end >= 0)
//	{
//		std::string s = msg.substr(init, end - init);
//		palabras.push_back(s);
//		init = end + 1;
//	}
//	return palabras;
//}

void Chat() {

	sf::Vector2i screenDimensions(800, 600);
	sf::RenderWindow window;
	window.create(sf::VideoMode(screenDimensions.x, screenDimensions.y), "Chat");

	sf::String mensaje = "";

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

	//for (int i = 0; i < 7; i++)
	//{
	//	Carta carta;
	//	carta.familia = static_cast<Familia>(rand() % Tirolesa);
	//	carta.tipo = static_cast<Tipo>(rand() % Hija);
	//	cartas.push_back(carta);
	//}

	//cout << "Mis cartas son: " << endl;
	//for (int i = 0; i < cartas.size(); i++)
	//{
	//	cout << "Carta " << i << " " << DecirFamilia(cartas[i].familia) << " del tipo: "
	//		<< DecirTipo(cartas[i].tipo) << endl;
	//}

	Packet msgPacket;

	std::vector<std::string> comandosReceive;

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
					std::string messageToSend = mensaje.toAnsiString();
					std::string commando = "";
					std::string familia = "";
					std::string tipo = "";


					std::istringstream iss(messageToSend);
					std::vector<std::string> results((std::istream_iterator<std::string>(iss)),
						std::istream_iterator<std::string>());

					int size = 0;
					size = results.size();
					msgPacket << size;

					cout << "Palabras a decir" << endl;
					for (int i = 0; i < size; i++)
					{
						cout << results[i] << endl;
						msgPacket << results[i];
					}
					//------------------ Send ------------------

					for (int i = 0; i < peerSockets.size(); i++)
					{				
						peerSockets[i]->send(msgPacket);
					}
					//mensaje = ">";
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
				//cout << "Error reciviendo el paquete" << endl;
			}
			else {
				std::string chatting;
				int sizeOfMsg = 0;
				msgPacket >> sizeOfMsg;
				for (int i = 0; i < sizeOfMsg; i++)
				{
					std::string word;
					msgPacket >> word;
					comandosReceive.push_back(word);
				}
				/*msgPacket >> chatting;*/
				cout << "El contenido del paquete es: " << comandosReceive[0] << endl;
				std::string comando = comandosReceive[0];
				const char* cmd = comando.c_str();

				int checkCmd = 0;

				if (comandosReceive[0] == "Pedir")
					checkCmd = 1;
				if (comandosReceive[0] == "Msg")
					checkCmd = 2;
				if (comandosReceive[0] == "Exit")
					checkCmd = 3;

				std::string fam;
				std::string type;

				switch (checkCmd) {
					case 1: 
						//Pedir carta a jugador especifico. 
						//Esperar recepci�n de si la tiene o no. 
						//Si es que si, recevirla y a�adirla a nuestro mazo. 
						//Si es que no, se cambia el turno. Lo cambiamos aqui?

						fam = comandosReceive[1];
						type = comandosReceive[2];

						int famObjective = 0;
						int tipoObjective = 0;

						if (fam == "Arabe")
							famObjective = 1;
						if (fam == "Abuelo")
							tipoObjective = 1;

						//Comprobar si esta dentro de tu mazo. 
						bool peticion = false;
						Carta enviar;
						for (int i = 0; i < cartas.size(); i++)
						{
							if (cartas[i].familia == famObjective &&
								cartas[i].tipo == tipoObjective) {
								//Carta encontrada en mi mazo. 
								enviar = cartas[i];
								cartas.erase(cartas.begin() + i);
								peticion = true;
								break;
							}
						}

						//Enviar de vuelta que si o no al jugador de la petici�n. 
						if (peticion) {
							//Enviamos al socket de vuelta, que si la tenemos y se la enviamos. 
							Packet packet;
							std::string msg = "Si tengo la carta, te la doy: ";
							packet << msg;
							/*peerSockets.send(packet);*/ //Crear mapa para indetificarlo m�s facil. 
						}
						break;
					case 2:
						//Enviar un msg a cualquier jugador especifico. 
						//O enviar un mensaje global simplemente? 
						break;
					case 3:
						//Desconnectarnos de la partida. 
						break;
				}

				chattingText.setPosition(sf::Vector2f(0, 20 * i));
				chattingText.setString(comandosReceive[0]);
			}
		}
		std::string mensaje_ = mensaje + "_";
		text.setString(mensaje_);
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
		cout << "Error en la recepci�n del paquete " << endl;
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

