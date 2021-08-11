#pragma once

class Vehicle
{
public:
	Vehicle(int passengersPriorities, int conflictSideID, int waitingTime, int isEmergency);
	bool getIsEmergency() const { return isEmergency; }
	int getCS() const { return conflictSideID; }
	void setFinalPriority(int prio) { this->finalPriority = prio; }
	int getFinalPriority() const { return finalPriority; }
private:
	int priorityFromPassengers;
	int finalPriority; //final priority that consider the waiting time
	int waitingTimeSinceEpoch;
	bool isEmergency;
	int conflictSideID;
};