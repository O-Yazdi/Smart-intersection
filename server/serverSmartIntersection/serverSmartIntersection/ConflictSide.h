#pragma once
#include <vector>
#include "Vehicle.h"

class ConflictSide
{
public:
	ConflictSide(int id) : conflictSideID(id) {}
	void addVehicleToCS(const Vehicle& v)
	{
		vehicleList.emplace_back(v);
	}
	int getCSID() const { return conflictSideID; }
	std::vector<Vehicle> getVehicleList() const { return vehicleList; }

private:
	std::vector<Vehicle> vehicleList;
	int conflictSideID;
};