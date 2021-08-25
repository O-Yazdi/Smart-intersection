#pragma once
#include "ConflictSide.h"

class Intersection
{
public:
	Intersection(std::vector<ConflictSide> csList) : csList(csList) {} //in the CTOR need to create CS'S
	std::vector<ConflictSide> getCsList() const { return csList; }
private:
	std::vector<ConflictSide> csList;
};
