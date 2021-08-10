#pragma once
#include <WinSock2.h>
#include <Windows.h>
//#include <vector>
#include "SmartIntersection.h"

#define DATA_BYTES_SIZE sizeof(int) //4
#define SIZE_OF_PIECE_DATA 4*sizeof(int) //piece = car

//typedef std::vector<unsigned char> Buffer;

class Communicator
{
private:
	SOCKET server_socket;

	void bindAndListen() const;
	void handleSimulation(SOCKET client_socket);
	void getLength(SOCKET client_socket, int& length) const;
	void getDataFromClient(SOCKET client_socket, Buffer& buff, int length) const;
	void sendResponseToClient(SOCKET client_socket, const Buffer& buff) const;
	Intersection arrangeBufferToIntersection(const std::vector<Vehicle>& vec) const;
	void serializeResponse(Buffer& buff, int cs, int* arrPriCS);
	void sendDeffaulMssg(SOCKET client_socket) const;
	//void sayHello(SOCKET client_socket) const;
	


public:
	Communicator();
	Communicator(const Communicator&) = delete; //prevent coppy object of singleton class
	Communicator& operator=(const Communicator&) = delete; //prevent coppy object of singleton class
	void startHandleRequests();

	~Communicator();
};