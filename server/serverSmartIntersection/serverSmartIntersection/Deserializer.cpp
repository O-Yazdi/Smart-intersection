#include "Deserializer.h"
//#include "json.hpp"

//using json = nlohmann::json;

Vehicle Deserializer::deserializeEachVehicle(unsigned char* buff, int length)
{
    int priority, conflictSideID, time, isEmergency;

    //read the 4 bytes to each value from the buff
    memcpy(&priority, buff, sizeof(int)); 
    memcpy(&conflictSideID, buff + 4, sizeof(int));
    memcpy(&time, buff + 8, sizeof(int));
    memcpy(&isEmergency, buff + 12, sizeof(int));

    if (isEmergency != 0 and isEmergency != 1)
    {
        int test = isEmergency;
    }
    return Vehicle(priority, conflictSideID, time, isEmergency);
}

void Deserializer::deserializeGetVehicles(std::vector<Vehicle> &vecVehicle, Buffer& buff)
{
    if (buff.size() % 16 != 0)
        throw std::exception(__FUNCTION__ " buff.size() % 16 != 0");

    unsigned char* buff2 = new unsigned char[buff.size()];
    for (int i = 0; i < buff.size(); i++)
    {
        buff2[i] = buff[i];
    }

    for (int i = 0; i < buff.size(); i += 16)
    {
        vecVehicle.push_back(deserializeEachVehicle(buff2 + i, 16));
    }

    delete[] buff2;
}