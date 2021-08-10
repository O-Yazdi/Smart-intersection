#pragma once
#include "Intersection.h"

typedef std::vector<unsigned char> Buffer;

class SmartIntersection
{
public:
	SmartIntersection(Intersection& intersect) :intersect(intersect) {};
	void setIntersection(Intersection& intersect) { this->intersect = intersect; }
	int calculatenextCSGreen(int* priCS) const; //calc the next green and upfdate the arr of priority for each CS
	void setCurrentCSGreen(int csID);
	int getCurrentCSGreen() const { return currentCSGreen; }

	static int calculateFinalPriority(int priorityFromPassengers, int waitingTimeSinceEpoch);

private:
	static int calculatePriorityWaitingTime(int waitingTimeSinceEpoch);

	Intersection intersect;
	int currentCSGreen = 0;
	int const minTimeForGreen = 10;
	int const timeForIteration = 3;
	int const priorityForCurrentGreen = 10;
	//static int const MAX_PRIORITY = 50;
};