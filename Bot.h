#ifndef BOT_H
#define BOT_H

// stl
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <sstream>

// project
#include "main.h"
#include "Parser.h"
#include "Region.h"
#include "SuperRegion.h"
#include "General.h"


//forward declaration so bot recognizes general
class General;


using namespace std;

class Bot: boost::noncopyable
{

public:
	enum Phase
	{
		NONE, PICK_STARTING_REGION, PLACE_ARMIES, ATTACK_TRANSFER
	};

	Bot();
	virtual ~Bot();

	void playGame();    ///< plays a single game of Warlight

	void pickStartingRegion();
	void placeArmies();
	void makeMoves();   ///< makes moves for a single turn

	void addRegion(const unsigned& noRegion, const unsigned& noSuperRegion);
	void addSuperRegion(const unsigned& noSuperRegion, const int& reward);
	void addNeighbors(const unsigned& noRegion, const unsigned& Neighbors);
	void addWasteland(const unsigned& noRegion);

	/// Setters for settings
	void setBotName(const std::string& name);
	void setOpponentBotName(const std::string& name);
	void setArmiesLeft(const int& nbArmies);
	void setTimebank(const int& newTimebank);
	void setTimePerMove(const int& newTimePerMove);
	void setMaxRounds(const int& newMaxRounds);

	/**
	 * Adds armies to a region
	 * @param noRegion region to add to
	 * @param nbArmies number of Armies
	 */
	void addArmies(const unsigned& noRegion, const int& nbArmies);
	/**
	 * Moves armies on the map
	 * @param noRegion starting region
	 * @param toRegion target region
	 * @param nbArmies number of Armies
	 */
	void moveArmies(const unsigned& noRegion, const unsigned& toRegion, const int& nbArmies);

	void clearStartingRegions();
	void addStartingRegion(const unsigned& noRegion);

	void addOpponentStartingRegion(const unsigned& noRegion);

	void opponentPlacement(const unsigned& noRegion, const int& nbArmies);
	void opponentMovement(const unsigned& noRegion, const unsigned& toRegion, const int& nbArmies);

	void startDelay(const int& delay);

	void setPhase(const Phase phase);

	/**
	 * Evaluates the current phase and in turn figures out the proper action for the current move
	 * Hook in your army placements and attack moves here
	 */
	void executeAction();
	/**
	 * Updates the regions from the game engine
	 * @param noRegion region identifier
	 * @param playerName player who owns it
	 * @param nbArmies number of armies he gets
	 */
	void updateRegion(const unsigned& noRegion, const std::string& playerName, const int& nbArmies);

	void resetRegionsOwned();




	/*
	 * input:
	 *       Player player: player of the regions we are looking for
	 * output:
	 * 		vector<Region>: vector of all seen regions of that player
	 *
	 * Does the same thing that get adjacent player does I beleive, but it is called by some functions so I am keeping this function.
	 */
	vector<Region*> getRegionsOwnedBy(Player player);

	/*
	 * input:
	 * 		Player player: player you want to find the regions of
	 *
	 * output:
	 * 		vector<Region>: returns all regions on the known map owned by the certain player
	 *
	 * same as output
	 */
	vector<Region> getAdjacentPlayer(Player player);

	/*
	 * input:
	 * 		Player player: the player that you want to find if you are neighbors with
	 * 		Region place: the region which you are examining the neighbors of
	 *
	 *output:
	 * 		Vector<Region>: Returns all neighbors around a certain Region that are owned by a particular owner
	 *
	 * Same as output.
	 */
	vector<Region*> getNeighbors(Player player, Region* place);

	/*
	 * input: none
	 *
	 * output: vecotr<Regions>: returns all regions owned by us
	 *
	 * This function will return all regions owned by us as a vecotr of Regions
	 */
	vector<Region*> getOwnedRegions();

	/*
	 * input: none
	 *
	 * output: Vector<Region>: returns a vector of all of the wastelands on the map
	 *
	 * Returns a vector of all regions that are wastelands
	 */
	vector<Region*> getwastelands();

	/*
	 * input: none
	 *
	 * output: vector<Region>: returns a vector of Regions of all of the starting reginos
	 *
	 * It is literally what it says. It returns the starting regions we were given
	 */
	vector<Region*> getstartingRegionsreceived();

	/* input:
	 * 		Region Place: Region instance you are wanting to find the neighbors of.
	 * outpout:
	 * 		vector<Region>: Returns a vector of all neighbors that are not me
	 *
	 * This function takes in any region and will return a vector of all regions
	 * not us that are it's neighbors
	 */
	vector<Region*> getOtherNeighbors(Region* place);

	/*
	 * input: SuperRegion place: SuperRegion you are examining
	 *
	 * output: int: returns the amount of not us teritories left in the super region
	 *
	 * This function counts the number of regions that are not us that are in a particular superRegion
	 */
	int regionsLeftInSuper(SuperRegion place);

	/*
	 * input: Region location: the region you want to find to find the neibhors that are in the same super region as it
	 *
	 * output: vector<Region>: returns a Region vecotr of all neighbors in the same super region
	 *
	 * This function checks for all other neighbors in a super region that are not you and then returns them as a vecotr of regions
	 *
	 */
	vector<Region*> otherNbInSuperRegion(Region* location);


	General* general;  //the general that will be running the games
	std::string botName;
	std::vector<Region> regions;
	std::vector<SuperRegion> superRegions;
	int armiesLeft;


private:
	std::ifstream in;

	//std::string botName;
	std::string opponentBotName;
	std::vector<int> startingRegionsreceived;
	std::vector<unsigned> opponentStartingRegions;
	std::vector<int> ownedRegions;
	std::vector<int> wastelands;

	int timebank;
	int timePerMove;
	int maxRounds;
	Parser parser;
	Phase phase;
};

#endif // BOT_H
