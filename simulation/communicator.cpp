#include "communicator.h"
#include <iostream>
#include <thread>
#include <chrono>

Communicator::Communicator(SimulationScene& sim) : wsaInit(), sim(sim)
{
    _simulationSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (_simulationSocket == INVALID_SOCKET)
        throw std::exception();
}

Communicator::~Communicator()
{
    try
    {
        closesocket(_simulationSocket);
    }
    catch (...) {}
}

void Communicator::connect(std::string serverIP, int port)
{
    struct sockaddr_in sa = { 0 };

    sa.sin_port = htons(port); // port that server will listen to
    sa.sin_family = AF_INET;   // must be AF_INET
    sa.sin_addr.s_addr =  inet_addr(serverIP.c_str());    // the IP of the server

    // notice that we step out to the global namespace
    // for the resolution of the function socket
    int status = ::connect(_simulationSocket, (struct sockaddr*)&sa, sizeof(sa));

    if (status == INVALID_SOCKET)
        throw std::exception();
}

void Communicator::startConnection()
{
    int carData[4];
    carData[0] = 3;			//priority
    carData[1] = 2;			//CS ID
    carData[2] = 123432;	//time
    carData[3] = 0;			//is emergency

    int arrToSend[17];		//1 for the length, and 4 int for each car
    arrToSend[0] = 4;	//cuz 4 cars

    int dataFromServer[5];

    for (int i = 1; i < 17; i += 4)
    {
        for (int j = 0; j < 4; j++)
        {
            arrToSend[i + j] = carData[j];
        }
    }
    //for (int i = 0; i < 17; i++)
    //{
    //	std::cout << arrToSend[i] << " ";
    //}
    std::cout << "\n";

    char buffToSend[68];
    char buffToRec[20];

    memcpy(buffToSend, arrToSend, 68);

    int k = 0;
    while (true)
    {
        std::cout << "Message send to server: " << std::endl;
        //std::cout << buffToSend << std::endl;


        send(_simulationSocket, buffToSend, 68, 0);  // last parameter: flag. for us will be 0.

        recv(_simulationSocket, buffToRec, 20, 0);

        memcpy(dataFromServer, buffToRec, 20);

        std::cout << "response from server: " << std::endl;
        for (int i = 0; i < 5; i++)
        {
            std::cout << dataFromServer[i] << " ";
        }
        std::cout << "\n";

        sim.setCsGreenLight(dataFromServer[0]);

        k++;
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(3s);
    }
}
