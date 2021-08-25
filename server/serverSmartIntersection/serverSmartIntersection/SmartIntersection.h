#pragma once
#include "Intersection.h"

typedef std::vector<unsigned char> Buffer;

class SmartIntersection
{
public:
	SmartIntersection(Intersection& intersect) :intersect(intersect) {};
	void setIntersection(Intersection& intersect) { this->intersect = intersect; }
	int calculatenextCSGreen(int* priCS); //calc the next green and upfdate the arr of priority for each CS
	void setCurrentCSGreen(int csID);
	int getCurrentCSGreen() const { return currentCSGreen; }

	static int calculateFinalPriority(int priorityFromPassengers, int waitingTimeSinceEpoch);

private:
	static int calculatePriorityWaitingTime(int waitingTimeSinceEpoch);
	void resetSumPriorities(int* priCS) const;
	int maxCsPriority(int* priCS) const;
	Intersection intersect;
	int currentCSGreen = 0;
	int iterationCountFromLastChange = 0;			//this hold the count of iteration that pass form the switch CS green
	int const minIteratonForGreen = 2;				//minimum 4 sec (coz every iteration = 2 sec)  before change the traffic light (in the 4th we can switch)
	int const timeForIteration = 2;					//The simulation sends data to the server every 2 seconds
	int const priorityForCurrentGreen = 15;			//the current CS with the green light gets more 15 priorities points
	//static int const MAX_PRIORITY = 50;
	bool const smartIntersection = true;

};