#ifndef REGION_H
#define REGION_H

//stl
#include <vector>
#include <string>

// project
#include "main.h"

using namespace std;

class Region
{
public:
	Region();
	Region(const int& pId, const  int& superRegion);
	virtual ~Region();

	void addNeighbor(const int& neighbor);
	void setArmies(const int& nbArmies) { armies = nbArmies; }
	void setOwner(const Player& pOwner){ owner = pOwner; }

	inline int getArmies() const { return armies; }
	inline Player getOwner() const { return owner; }
	inline int getSuperRegion() const { return superRegion; }
	int getNeighbor(const size_t& index) const ;
	int getNbNeighbors() const;

	Player owner;

	std::vector<int> neighbors;
	int id;
	int superRegion;
	int armies;

private:




};

#endif // REGION_H