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

std::vector<TcpSocket*> peers;

int main()
{
	TcpSocket socket;
	std::string portBSS = "";

	Socket::Status status = socket.connect("192.168.1.61", 5000);

	if (status != Socket::Done) {
		cout << "Error connecting to BSS server" << endl;
	}
	else {
		cout << "Connected to the BSS server" << endl;
		portBSS = std::to_string(socket.getLocalPort());
		cout << "The port: " << portBSS << endl;
		//Recivir todos los datos, de los clientes que hay en partida. 
		//Desconectarse del BSS. 
		socket.disconnect();
		TcpSocket* clientNuevo = new TcpSocket();
		Socket::Status status = clientNuevo->connect("192.168.1.61", 5001);
		peers.push_back(clientNuevo);
	}


	return 0;
}
