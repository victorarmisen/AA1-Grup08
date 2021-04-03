#include "clientP2P.h"









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
		
		socket->disconnect();
		cout << "Escucho por el puerto: " << (peers[(int)peers.size() - 1].port).toString() << endl;
	}
	for (size_t i = 0; i < peers.size() - 1; i++)
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
	//peers[(int)peers.size() - 1].port
	if (listener.listen(socket->getLocalPort()) != Socket::Done) {
		cout << "Puerto no disponible" << endl;
	}
	else {
		cout << "Puerto abierto" << endl;
	}
	while (true) {
		TcpSocket* socket = new TcpSocket();
		if (listener.accept(*socket) != Socket::Done) {
			cout << "Error al conectarse" << endl;
			break;
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
