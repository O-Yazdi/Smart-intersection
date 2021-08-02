#include "SmartIntersection.h"

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
	return 0;
}

int SmartIntersection::calculatePriorityWaitingTime(int waitingTime)
{
	//to do...
	return 0;
}