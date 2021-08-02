#pragma once
#include "Intersection.h"

typedef std::vector<unsigned char> Buffer;

class SmartIntersection
{
	//need to delete all the car in the junction
public:
	SmartIntersection(Intersection& intersect) :intersect(intersect) {};
	void setIntersection(Intersection& intersect) { this->intersect = intersect; }
	int calculatenextCSGreen(int* priCS) const; //clac the next green and upfdate the arr of priority for each CS
	void setCurrentCSGreen(int csID);
	int getCurrentCSGreen() const { return currentCSGreen; }

	static int calculatePriorityWaitingTime(int waitingTime); //each one or all together in cs

private:
	Intersection intersect;
	int currentCSGreen = 0;
	int const minTimeForGreen = 10;
	int const timeForIteration = 3;
	int const priorityForCurrentGreen = 10;
};