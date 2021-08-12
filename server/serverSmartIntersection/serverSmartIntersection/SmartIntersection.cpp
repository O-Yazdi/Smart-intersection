#include "SmartIntersection.h"
#include<chrono>
#include <iostream>
using namespace std;

void SmartIntersection::setCurrentCSGreen(int csID)
{
	if (csID < 0 or csID > 3)
		throw std::exception(__FUNCTION__ " CS NOT VALID!");

	currentCSGreen = csID;
}

int SmartIntersection::calculatenextCSGreen(int* priCS)
{
	iterationCountFromLastChange++;
	resetSumPriorities(priCS);

	int csGreenLightRes = 0;
	//sum the final priority for each CS and check if there emergency vehicle
	for (const auto& cs : intersect.getCsList())
	{
		for (const auto& veh : cs.getVehicleList())
		{
			if (veh.getIsEmergency())
			{
				cout << "IsEmergency in :" << veh.getCS() << "\n";
				resetSumPriorities(priCS);
				return veh.getCS();
			}
			priCS[veh.getCS()] += veh.getFinalPriority();
		}
	}

	//we want to add priority to the CS that have the green light to let the traffic flow
	if (priCS[currentCSGreen] != 0) //mean that this CS with the current green there vehicles
	{
		priCS[currentCSGreen] += priorityForCurrentGreen;
	}

	if (iterationCountFromLastChange < minIteratonForGreen and priCS[currentCSGreen] != 0)
	{
		csGreenLightRes = currentCSGreen; //we dont switch
		cout << "NEXT_CS_GREEN: " << currentCSGreen << "\nDidn't switch to give more time, and there cars in this CS\n\n";
	}
	else
	{
		csGreenLightRes = maxCsPriority(priCS);
		if (csGreenLightRes != currentCSGreen)
			iterationCountFromLastChange = 0;
		cout << "NEXT_CS_GREEN: " << csGreenLightRes << "\n\n";
	}

	//csGreenLightRes = (csGreenLightRes + 1) % 4;
	currentCSGreen = csGreenLightRes;
	return csGreenLightRes;
}

//to prevent starvtion for vehicles with low priority we use aging. linear function: every second = 1 priority
int SmartIntersection::calculatePriorityWaitingTime(int waitingTimeSinceEpoch)
{
	using namespace std::chrono;
	seconds endTime = duration_cast<seconds>(
		system_clock::now().time_since_epoch());

	int end = endTime.count();

	int waitingTimeInRed = end - waitingTimeSinceEpoch;
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

void SmartIntersection::resetSumPriorities(int* priCS) const
{
	for (int i = 0; i < 4; i++)
	{
		priCS[i] = 0;
	}
}

int SmartIntersection::maxCsPriority(int* priCS) const
{
	int maxCsPirority = priCS[0], indMax = 0;

	for (int i = 0; i < 4; i++)
	{
		cout << "\tpriCS[" << i << "] : " << priCS[i];
		if (priCS[i] > maxCsPirority)
			indMax = i;
	}
	cout << "\n";

	return indMax;
}