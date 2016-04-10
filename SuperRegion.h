#ifndef SUPERREGION_H_INCLUDED
#define SUPERREGION_H_INCLUDED

// stl
#include <vector>

// project
#include "main.h"
#include "Region.h"

class SuperRegion
{
public:
	SuperRegion();
	SuperRegion(const int& pReward);
	virtual ~SuperRegion();

	size_t size() { return regions.size(); }
	void addRegion(const int& region);

	std::vector<int> regions;
private:

	int reward;
};

#endif // SUPERREGION_H_INCLUDED
