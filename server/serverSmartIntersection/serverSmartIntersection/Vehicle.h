#pragma once

class Vehicle
{
public:
	Vehicle(int passengersPriorities, int conflictSideID, int waitingTime, int isEmergency);
	bool getIsEmergency() const { return isEmergency; }
	int getCS() const { return conflictSideID; }
	void setFinalPriority(int prio) { this->finalPriority = prio; }
private:
	int priorityFromPassengers;
	int finalPriority; //priority that takes the waiting time
	int waitingTime;//need to get date and time then to calc the waiting time
	bool isEmergency;
	int conflictSideID;
};