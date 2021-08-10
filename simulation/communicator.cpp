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

void Communicator::vehiclesToBuff(QList<Vehicle *> veh)
{
    int test =0;
    std::vector<int> carsInt;

    qDebug()<<"veh.size()="<<veh.size()<<"\n";

    for(int i=0; i<veh.size(); i++)
    {
        //if(veh[i]->is_in_stop_point())
        {
            qDebug()<<"\ncar number : = "<<i;
            carsInt.push_back(veh[i]->getPriority());
            qDebug()<<"\tPriority = "<<veh[i]->getPriority();

            carsInt.push_back(veh[i]->getCs());
            qDebug()<<"\tCS = "<<veh[i]->getCs();

            carsInt.push_back(veh[i]->getWaitingTime());
            qDebug()<<"\tWaitingTime = "<<veh[i]->getWaitingTime();

            carsInt.push_back(veh[i]->getIsEmergency());
            qDebug()<<"\tIsEmergency = "<<(int)veh[i]->getIsEmergency();

        }
    }

    int numInt;
    unsigned char arrChar[4];

    buff.clear();

    qDebug()<<"carsInt.size()="<<carsInt.size()<<"\n";

    int length = carsInt.size()/4; //coz every cars its 4 int
    char lengthInChar[4];
    memcpy(lengthInChar, &length, 4);
    qDebug()<<"length = "<<length<<"\n";
    for(int i=0; i<4; i++)
    {
        buff.push_back(lengthInChar[i]);
    }

    for(int i=0; i<carsInt.size(); i++)
    {
        numInt = carsInt[i];
        memcpy(arrChar, &numInt, sizeof(int));

        for(int j=0; j<4; j++)
        {
            buff.push_back(arrChar[j]);
        }
    }


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

void Communicator::sendDataToServer() const
{
    //conversion from Buffer* into const char *
    std::string str(buff.begin(), buff.end());
    const char* arr = str.c_str();

    int res = send(_simulationSocket, arr, buff.size(), 0);

    if (res == SOCKET_ERROR)
        throw std::exception();

    //maybe to check if 'send' finish all the bytes
}

void Communicator::getResponseFromServer()
{
    char buffToRec[20];

    recv(_simulationSocket, buffToRec, 20, 0);

    buff.clear();
    for(int i=0; i<20; i++)
    {
        buff.push_back(buffToRec[i]);
    }
}

void Communicator::buffToIntArr(int* arr) const
{
    std::string str(buff.begin(), buff.end());
    const char* arr2 = str.c_str();

    memcpy(arr, arr2, 20);
}


void Communicator::startConnection()
{
    //int carData[4];
    //carData[0] = 3;			//priority
    //carData[1] = 2;			//CS ID
    //carData[2] = 123432;	//time
    //carData[3] = 0;			//is emergency

    //int arrToSend[17];		//1 for the length, and 4 int for each car
    //arrToSend[0] = 4;	//cuz 4 cars

    int dataFromServer[5];

    //for (int i = 1; i < 17; i += 4)
    //{
        //for (int j = 0; j < 4; j++)
        //{
            //arrToSend[i + j] = carData[j];
        //}
    //}
    //for (int i = 0; i < 17; i++)
    //{
    //	std::cout << arrToSend[i] << " ";
    //}
    std::cout << "\n";

    char buffToSend[68];
    char buffToRec[20];

    //memcpy(buffToSend, arrToSend, 68);

    while (true)
    {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(4s);

        //get all cars
        //check the cars who before the line
        //make buff of char to send
        //we need mutexs
        QList<Vehicle *> veh = sim.getVehicle();


        vehiclesToBuff(veh);

        //send(_simulationSocket, buffToSend, 68, 0);  // last parameter: flag. for us will be 0.
        sendDataToServer();

        //recv(_simulationSocket, buffToRec, 20, 0);
        getResponseFromServer();

        buffToIntArr(dataFromServer);

        std::cout << "response from server: " << std::endl;
        /*for (int i = 0; i < 5; i++)
        {
            std::cout << dataFromServer[i] << " ";
        }*/
        std::cout << "\n";

        sim.setCsGreenLight(dataFromServer[0]);

    }
}
