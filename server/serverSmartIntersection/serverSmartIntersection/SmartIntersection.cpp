#include "SmartIntersection.h"
#include<chrono>
int ind = 0;

void SmartIntersection::setCurrentCSGreen(int csID)
{
	if (csID < 0 or csID > 3)
		throw std::exception(__FUNCTION__ " CS NOT VALID!");

	currentCSGreen = csID;
}

int SmartIntersection::calculatenextCSGreen(int* priCS) const
{
	//need to calc the sum priority for each CS
	for (int i = 0; i < 4; i++)
	{
		priCS[i] = i;
	}
	//to do...
	ind = 0;//(ind + 1) % 4;
	return ind;
}

//to prevent starvtion for vehicles with low priority we use aging. linear function: every second = 1 priority
int SmartIntersection::calculatePriorityWaitingTime(int waitingTimeSinceEpoch)
{
	using namespace std::chrono;
	seconds endTime = duration_cast<seconds>(
		system_clock::now().time_since_epoch());

	int end = endTime.count();

	int waitingTimeInRed = waitingTimeSinceEpoch - end;
	return waitingTimeInRed * 1;
}

//return the final priority consider the waitingTime.
int SmartIntersection::calculateFinalPriority(int priorityFromPassengers, int waitingTimeSinceEpoch)
{
	int finalPriority = priorityFromPassengers + calculatePriorityWaitingTime(waitingTimeSinceEpoch);
	
	//if (finalPriority <= MAX_PRIORITY)
	return finalPriority;

	//return MAX_PRIORITY;
}
