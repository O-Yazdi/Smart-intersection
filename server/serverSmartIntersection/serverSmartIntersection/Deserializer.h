#pragma once
#include"Vehicle.h"
#include <vector>
#include<string>

std::vector<unsigned char> typedef Buffer;


class Deserializer
{
private:
	static Vehicle deserializeEachVehicle(unsigned char* buff, int length);

public:
	static void deserializeGetVehicles(std::vector<Vehicle>& vecVehicle, Buffer&);
};