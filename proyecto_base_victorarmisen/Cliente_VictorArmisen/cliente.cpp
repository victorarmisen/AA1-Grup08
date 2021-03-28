#include <iostream>
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <thread>
#include <mutex>
#include <vector>
#include <time.h>

using std::cout;
using std::endl;

int main()
{
	sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);

	sf::TcpSocket cliente; 

	sf::Socket::Status status = cliente.connect("192.168.1.60", 5000);

	if (status != sf::Socket::Done) {
		cout << "Error al conectarse" << endl;
	}
	std::string message; 
	message = "Binding port of Isaac, Haha";
	sf::Packet packet;
	packet << message;
	sf::Socket::Status status2 = cliente.send(packet);

	//while (window.isOpen())
	//{
	//	sf::Event event;
	//	while (window.pollEvent(event))
	//	{
	//		if (event.type == sf::Event::Closed)
	//			window.close();
	//	}

	//	window.clear();
	//	window.draw(shape);
	//	window.display();
	//}

	/* 
	Torre: 
	Aplicación. HTTP, SSH o protocolo nuestro. 
	Transporte. PORT. Escuchar el puerto de la aplicación. Podemos estar con 50 aplicaciones. 
	Red: Encaminar este paquete hasta la red destino. IP. Red destino. 
 	Enlance: Direcciones MAC. Se encarga de enviar el paquete a un dispositvo concreto. 
	Fisica: Ethernet.

	Processos y tambien existen THREADS. 

	Thread: Evitar que un programa se quede bloqueado. 
	t.join("Function"); esperar hasta que el thread acaba para hacer las siguientes instrucciones. 
	t.detach(); cuando el programa principal acabe, el thread que tenga detach acabara tambien. 
	Classes, Structs.. tambien se puede usar para los threads. 
	Si los threads comparten variables. Recursos compartidos. Hay que protejerlo para que no se modifiquen a la vez. 
	MUTEX. LOCK y UNLOCK. Pregunta si se puede realizar la accion. Haga las acciones y luego hago unlock(). Problema de concurrencia. 
	No habria problemas de intercalacion. 
	usar lockquard cuando acaba el scope, acaba. Te evitas poner unlock que se puede olvidar. 
	Funciones de this_thread::get_id, sleep (dormir un thread durante tanto)
	*/


	return 0;
}

void Pal() {
	
}