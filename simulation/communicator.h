#ifndef communicator_H
#define communicator_H

#include <exception>
#include <WinSock2.h>
#include <Windows.h>
#include <string>
#include "UI/simulationscene.h"
#include "commonenum.h"
#include "Entities/Vehicle/vehicle.h"

typedef std::vector<unsigned char> Buffer;

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
    Buffer buff;

    void vehiclesToBuff(QList<Vehicle *> veh);
    void sendDataToServer() const;
    void getResponseFromServer();
    void buffToIntArr(int* arr) const;

public:
    Communicator(SimulationScene& sim);
    ~Communicator();
    void connect(std::string serverIP, int port);
    void startConnection();
};


#endif // Communicator_H

