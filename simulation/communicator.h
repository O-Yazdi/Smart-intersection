#ifndef communicator_H
#define communicator_H

#include <exception>
#include <WinSock2.h>
#include <Windows.h>
#include <string>
#include "UI/simulationscene.h"

class SimulationScene;

class Communicator
{
private:
    class WSAInitializer
    {
    public:
        WSAInitializer()
        {
            WSADATA wsa_data = { };
            if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
                throw std::exception();
        }
        ~WSAInitializer()
        {
            try
            {
                WSACleanup();
            }
            catch (...) {}
        }

    };
    SOCKET _simulationSocket;
    WSAInitializer wsaInit;
    SimulationScene &sim;

public:
    Communicator(SimulationScene& sim);
    ~Communicator();
    void connect(std::string serverIP, int port);
    void startConnection();
};


#endif // Communicator_H

