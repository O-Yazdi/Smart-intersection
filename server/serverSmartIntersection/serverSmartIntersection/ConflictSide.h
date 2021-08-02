#pragma once
#include <vector>
#include "Vehicle.h"

class ConflictSide
{
public:
	ConflictSide(int id) : conflictSideID(id) {}
	void addVehicleToCS(const Vehicle& v)
	{
		VehicleList.emplace_back(v);
	}
	int getCSID() const { return conflictSideID; }

private:
	std::vector<Vehicle> VehicleList;
	int conflictSideID;
};