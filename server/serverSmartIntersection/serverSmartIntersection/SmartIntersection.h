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
	int const minTimeForGreen = 4;					//minimum 4 iteration before change the traffic light (in the 4th we can switch)
	int const timeForIteration = 1;					//every 1 sec = 1 iteration
	int const priorityForCurrentGreen = 15;			
	//static int const MAX_PRIORITY = 50;
};