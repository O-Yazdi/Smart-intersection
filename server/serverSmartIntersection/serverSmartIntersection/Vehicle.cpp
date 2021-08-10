#include"Vehicle.h"
#include "SmartIntersection.h"
#include <exception>
Vehicle::Vehicle(int passengersPriorities, int conflictSideID, int waitingTimeSinceEpoch, int isEmergency)
{
	if (conflictSideID < 0 or conflictSideID > 3)
		throw std::exception(__FUNCTION__ " conflictSideID must be between 0 to 3");

	if (isEmergency != 0 and isEmergency != 1)
		throw std::exception(__FUNCTION__ " isEmergency must be 0 or 1");

	this->priorityFromPassengers = passengersPriorities;
	this->conflictSideID = conflictSideID;
	this->waitingTimeSinceEpoch = waitingTimeSinceEpoch;
	this->isEmergency = isEmergency;

	this->finalPriority = SmartIntersection::calculateFinalPriority(priorityFromPassengers, waitingTimeSinceEpoch);
}