#pragma once
#include "ConflictSide.h"

class Intersection
{
public:
	Intersection(std::vector<ConflictSide> csList) : csList(csList) {} //in the CTOR need to create CS'S

private:
	std::vector<ConflictSide> csList;
};
