#pragma once
#include "Communicator.h"
#include <iostream>
#include "Deserializer.h"

static const unsigned short PORT = 8826;
static const unsigned int IFACE = 0;

Communicator::Communicator()
{
	server_socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (server_socket == INVALID_SOCKET)
		throw std::exception(__FUNCTION__ " - socket");
}


Communicator::~Communicator()
{
	try
	{
		// the only use of the destructor should be for freeing 
		// resources that was allocated in the constructor
		::closesocket(server_socket);
	}
	catch (...) {}
}
//int cs to CS next green
void Communicator::serializeResponse(Buffer& buff, int cs, int* arrPriCS)
{
	buff.clear();

	unsigned char arr[4];

	memcpy(arr, &cs, sizeof(int));

	for (int i = 0; i < 4; i++)
	{
		buff.push_back(arr[i]);
	}

	for (int i = 0; i < 4; i++)
	{
		memcpy(arr, &arrPriCS[i], sizeof(int));
		for (int j = 0; j < 4; j++)
		{
			buff.push_back(arr[j]);
		}
	}
}
Intersection Communicator::arrangeBufferToIntersection(const std::vector<Vehicle>& vec) const
{
	std::vector<ConflictSide> csList;
	for (int i = 0; i < 4; i++)
	{
		csList.push_back(ConflictSide(i));
	}

	for (int i = 0; i < vec.size(); i++)
	{
		int CS = vec[i].getCS();
		switch(CS)
		{
		case 0:
			csList[0].addVehicleToCS(vec[i]);
			break;

		case 1:
			csList[1].addVehicleToCS(vec[i]);
			break;

		case 2:
			csList[2].addVehicleToCS(vec[i]);
			break;

		case 3:
			csList[3].addVehicleToCS(vec[i]);
			break;

		default:
			throw std::exception(__FUNCTION__ " CS NOT VALID!");
		}
	}

	return Intersection(csList);
}


//void Communicator::sayHello(SOCKET client_socket) const
//{
//	char welcomeMsg[6] = "Hello";
//	send(client_socket, welcomeMsg, PREFIX * sizeof(char), 0);
//}

//read the code and length from client_socket into this args
void Communicator::getLength(SOCKET client_socket,int& length) const
{
	char bufferPre[DATA_BYTES_SIZE]; //we read 4 bytes =  int, length of the data

	int res = recv(client_socket, bufferPre, DATA_BYTES_SIZE, MSG_WAITALL);
	std::cout << "res=" << res << std::endl;
	if (res < 0)
		throw std::exception(__FUNCTION__ " recv failed");
	if (res == 0)
		throw std::exception(__FUNCTION__ " connection closed");

	
	memcpy(&length, bufferPre, DATA_BYTES_SIZE); //read the 4 bytes of size from the buffer into length
}

//read and put the data from client_socket into the buff
void Communicator::getDataFromClient(SOCKET client_socket, Buffer& buff, int length) const
{
	buff.reserve(length * SIZE_OF_PIECE_DATA); //allocate space for the buffer in size of length.

	char c = 0;
	for (int i = 0; i < length * SIZE_OF_PIECE_DATA; i++)
	{
		recv(client_socket, &c, sizeof(char), 0);
		buff.push_back(c);
	}
}

//send the buffer data to the socket
void Communicator::sendResponseToClient(SOCKET client_socket, const Buffer& buff) const
{
	//conversion from Buffer* into const char *
	std::string str(buff.begin(), buff.end());
	const char* arr = str.c_str();

	int res = send(client_socket, arr, buff.size(), 0);

	if (res == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " send is failed");
}


void Communicator::bindAndListen() const
{
	struct sockaddr_in sa = { 0 };
	sa.sin_port = htons(PORT);
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = IFACE;

	if (::bind(server_socket, (struct sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - bind");

	if (::listen(server_socket, SOMAXCONN) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - listen");

	std::cout << "server::server is listening..." << std::endl;
}

//The server will listen to clients and handle new requests.
void Communicator::startHandleRequests()
{
	this->bindAndListen();
	SOCKET client_socket = accept(server_socket, NULL, NULL);
	try
	{
		if (client_socket == INVALID_SOCKET)
			throw std::exception("INVALID_SOCKET");

		std::cout << "server::clinet is connected.\n" << std::endl;

		//create new thread for each client
		handleSimulation(client_socket);
	}
	catch (std::exception& ex)
	{
		std::cerr << ex.what() << std::endl;
	}
}

void Communicator::sendDeffaulMssg(SOCKET client_socket) const
{
	int arrInt[5] = { 0,0,0,0,0 };
	char arrChar[20];
	memcpy(arrChar, arrInt, 20);
	int res = send(client_socket, arrChar, 20, 0);

	if (res == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " send is failed");
}
void Communicator::handleSimulation(SOCKET client_socket)
{
	int length;
	int nextCS;
	int priCS[4];
	//init the intersection with default values
	std::vector<ConflictSide> deffCsList;
	Intersection inter(deffCsList);

	//init the smartIntersection with the default intersection
	//after that in each iteration of the loop, we reset the intersection with the new data
	SmartIntersection smartInter(inter);

	while (true)
	{
		try
		{
			//we put in this args the code and lenght from the socket
			getLength(client_socket, length);

			Buffer buff;
			std::vector<Vehicle> vecVehicle;

			getDataFromClient(client_socket, buff, length);

			Deserializer::deserializeGetVehicles(vecVehicle, buff);//turn the all bytes to Vehicles in vecVehicle
			
			//need to create intersection with the new data and set it to the smartIntersection
			inter = arrangeBufferToIntersection(vecVehicle);
			smartInter.setIntersection(inter);

			//algorithm...
			nextCS = smartInter.calculatenextCSGreen(priCS);

			serializeResponse(buff, nextCS, priCS);

			sendResponseToClient(client_socket, buff);
		}
		catch (std::exception& er)
		{
			std::string str = er.what();
			std::cerr << er.what();
			sendDeffaulMssg(client_socket);
			//return;
		}
		catch (...)
		{
			std::cerr << "some error occurred" << std::endl;
			return;
		}
	}
}