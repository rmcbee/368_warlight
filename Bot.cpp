
// stl
#include <iostream>
#include <string>

//project
#include "Bot.h"
#include "General.h"
#include <set>

//tools
#include "tools/StringManipulation.h"

using namespace std;

Bot::Bot() :
		armiesLeft(0), timebank(0), timePerMove(0), maxRounds(0), parser(this), phase(NONE)
{
	 general = new General(this);

}

Bot::~Bot()
{

}

void Bot::playGame()
{
	parser.parseInput();
}

void Bot::pickStartingRegion(int time, std::set<int> regions)
{
	int region = general->pickStartingRegions(regions);

	// START HERE!
	std::cout << region << std::endl;
}

void Bot::placeArmies()
{
	general->getDeployment();
}

void Bot::makeMoves()
{
	general->calculateTurn();

}

void Bot::addRegion(const unsigned& noRegion, const unsigned& noSuperRegion)
{
	while (regions.size() <= noRegion)
	{
		regions.push_back(Region());
	}
	regions[noRegion] = Region(noRegion, noSuperRegion);
	superRegions[noSuperRegion].addRegion(noRegion);
}
void Bot::addNeighbors(const unsigned& noRegion, const unsigned& neighbors)
{
	regions[noRegion].addNeighbor(neighbors);
	regions[neighbors].addNeighbor(noRegion);
}
void Bot::addWasteland(const unsigned &noRegion)
{
	wastelands.push_back(noRegion);
}
void Bot::addSuperRegion(const unsigned& noSuperRegion, const int&reward)
{
	while (superRegions.size() <= noSuperRegion)
	{
		superRegions.push_back(SuperRegion());
	}
	superRegions[noSuperRegion] = SuperRegion(reward);
}

void Bot::setBotName(const std::string& name)
{
	botName = name;
}
void Bot::setOpponentBotName(const std::string& name)
{
	opponentBotName = name;
}
void Bot::setArmiesLeft(const int& nbArmies)
{
	armiesLeft = nbArmies;
}
void Bot::setTimebank(const int &newTimebank)
{
	timebank = newTimebank;
}
void Bot::setTimePerMove(const int &newTimePerMove)
{
	timePerMove = newTimePerMove;
}
void Bot::setMaxRounds(const int &newMaxRounds)
{
	maxRounds = newMaxRounds;
}

void Bot::clearStartingRegions()
{
	startingRegionsreceived.clear();
}

void Bot::addStartingRegion(const unsigned& noRegion)
{
	startingRegionsreceived.push_back(noRegion);
}

void Bot::addOpponentStartingRegion(const unsigned& noRegion)
{
	opponentStartingRegions.push_back(noRegion);
}
void Bot::opponentPlacement(const unsigned & noRegion, const int & nbArmies)
{
	// suppress unused variable warnings
	(void) noRegion;
	(void) nbArmies;

	// TODO: STUB
}
void Bot::opponentMovement(const unsigned &noRegion, const unsigned &toRegion, const int &nbArmies)
{
	// suppress unused variable warnings
	(void) noRegion;
	(void) toRegion;
	(void) nbArmies;

	// TODO: STUB
}

void Bot::startDelay(const int& delay)
{
	// suppress unused variable warnings
	(void) delay;
	// TODO: STUB
}
void Bot::setPhase(const Bot::Phase pPhase)
{
	phase = pPhase;
}
void Bot::executeAction()
{
	if (phase == NONE)
		return;
	if (phase == Bot::PICK_STARTING_REGION)
	{
		//pickStartingRegion();
	}
	else if (phase == Bot::PLACE_ARMIES)
	{
		placeArmies();
	}
	else if (phase == Bot::ATTACK_TRANSFER)
	{
		makeMoves();
	}
	phase = NONE;
}

void Bot::updateRegion(const unsigned& noRegion, const  std::string& playerName, const int& nbArmies)
{
	Player owner;
	if (playerName == botName)
		owner = ME;
	else if (playerName == opponentBotName)
		owner = ENEMY;
	else
		owner = NEUTRAL;
	regions[noRegion].setArmies(nbArmies);
	regions[noRegion].setOwner(owner);
	if (owner == ME)
		ownedRegions.push_back(noRegion);
}
void Bot::addArmies(const unsigned& noRegion, const int& nbArmies)
{
	regions[noRegion].setArmies(regions[noRegion].getArmies() + nbArmies);
}
void Bot::moveArmies(const unsigned& noRegion, const unsigned& toRegion, const int& nbArmies)
{
	if (regions[noRegion].getOwner() == regions[toRegion].getOwner() && regions[noRegion].getArmies() > nbArmies)
	{
		regions[noRegion].setArmies(regions[noRegion].getArmies() - nbArmies);
		regions[toRegion].setArmies(regions[toRegion].getArmies() + nbArmies);
	}
	else if (regions[noRegion].getArmies() > nbArmies)
	{
		regions[noRegion].setArmies(regions[noRegion].getArmies() - nbArmies);
		if (regions[toRegion].getArmies() - std::round(nbArmies * 0.6) <= 0)
		{
			regions[toRegion].setArmies(nbArmies - std::round(regions[toRegion].getArmies() * 0.7));
			regions[toRegion].setOwner(regions[noRegion].getOwner());
		}
		else
		{
			regions[noRegion].setArmies(
					regions[noRegion].getArmies() + nbArmies - std::round(regions[toRegion].getArmies() * 0.7));
			regions[toRegion].setArmies(regions[toRegion].getArmies() - std::round(nbArmies * 0.6));
		}
	}
}

void Bot::resetRegionsOwned()
{
	ownedRegions.clear();
}

/* tested */
vector<Region*> Bot::getNeighbors(Player player, Region* place)
{
	vector<Region*> ret;

	for (int i = 0; i < place->getNbNeighbors(); i++) {

		if(regions[place->neighbors[i]].owner == player)
			ret.push_back(&regions[place->neighbors[i]]);

	}


	return ret;
}

vector<Region*> Bot::getOtherNeighbors(Region* place)
{
	vector<Region*> ret;

	for (int i = 0; i < place->getNbNeighbors(); i++) {

		if(regions[place->neighbors[i]].owner != ME)
			ret.push_back(&regions[place->neighbors[i]]);

	}


	return ret;
}

vector<Region*> Bot::getOwnedRegions()
{
	vector<Region*> ret;


	for(size_t i = 0; i < ownedRegions.size(); i++)
	{
		ret.push_back(&regions[ownedRegions[i]]);
	}

	return ret;

}

vector<Region*> Bot::getwastelands()
{
	vector<Region*> ret;

	for(size_t i = 0; i < wastelands.size(); i++)
	{
		ret.push_back(&regions[wastelands[i]]);
	}

	return ret;

}

vector<Region*> Bot::getstartingRegionsreceived()
{
	vector<Region*> ret;

	for(size_t i = 0; i < startingRegionsreceived.size(); i++)
	{
		ret.push_back(&regions[startingRegionsreceived[i]]);
	}

	return ret;

}

vector<Region*> Bot::getRegionsOwnedBy(Player player) {
	std::vector<Region*> ret;
	for (int i = 0; i <  regions.size(); i++) {
		if (regions[i].getOwner() == player) {
			ret.push_back(&regions[i]);
		}
	}


	return ret;
}

vector<Region> Bot::getAdjacentPlayer(Player player) {
	vector<Region> ret;
	set<int> regionSet;

vector<Region*> owned = getOwnedRegions();

	for (Region* r: owned) {
		vector<Region*> nb = getNeighbors(player, r);
		for (Region* adj: nb) {

			if (adj->getOwner() == player) {
				regionSet.insert(adj->id);
			}
			else
				break;
		}
	}

	for(std::set<int>::iterator it=regionSet.begin(); it!=regionSet.end(); ++it)
	{
		ret.push_back(regions[*it]);
	}



	return ret;
}

int Bot::regionsLeftInSuper(SuperRegion place)
{

	int counter = 0;

	for(int r: place.regions) {
		if(regions[r].owner != ME)
			counter++;
	}

	return counter;

}

vector<Region*> Bot::otherNbInSuperRegion(Region* location)
{
	vector<Region*> ret;

	for (int i = 0; i < location->getNbNeighbors(); i++) {

		if(regions[location->neighbors[i]].superRegion == location->superRegion && regions[location->neighbors[i]].owner != ME)
			ret.push_back(&regions[location->neighbors[i]]);

	}

	return ret;
}

bool setContainsAll(std::set<int> super, std::set<int> visited) {
	std::set<int> difference;

	for (int element : super) {
		if (visited.count(element) == 0) {
			difference.insert(element);
		}
	}

	/*std::cerr << "Difference: " << difference.size() << std::endl;
	std::cerr << "First: ";
	for (auto r : super) {
		std::cerr << r << " ";
	}
	std::cerr << std::endl << "Second: ";
	for (auto r : visited) {
		std::cerr << r << " ";
	}
	std::cerr << std::endl << "Diff:";
	for (auto r : difference) {
		std::cerr << r << " ";
	}
	std::cerr << std::endl;*/
	return 0 == difference.size();
}

int Bot::centrality(int region)
{
	std::set<int> visited, super;
	int cent = 0;

	std::vector<int> s = superRegions[regions[region].getSuperRegion()].regions;
	for (int r : s) {
		super.insert(r);
	}

	visited.insert(region);

	while (!setContainsAll(super, visited)) {
		cent++;
		for (int r : visited) {
			auto neighbors = getNeighbors(r);
			visited.insert(neighbors.begin(), neighbors.end());
		}
	}
	return cent;
}

std::vector<int> Bot::getNeighbors(int place)
{
	return regions[place].neighbors;
}


