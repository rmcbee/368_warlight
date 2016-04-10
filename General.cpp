#include "General.h"
#include <string>
#include "tools/StringManipulation.h"

using namespace std;
General::General(Bot* x) {
	bot = x;
}

General::~General() {
    
}


int General::pickStartingRegions(std::vector<int> pickfrom) {

	//this function still needs implemented, we currently are just given random
	//areas to begin with

	return pickfrom.front();
}

std::vector<Move> General::generateNonAttack() {

	/*this function will take as input a region that has no enemy or neutral
	 * neighbors around it and calculates the best route to send the troops as
	 * to reinforce and/or start attacking
	 * this will bing an optimized path problem aka Dijkstra's algorithm
	 *
	 */

	return std::vector<Move>();
}

void General::getDeployment() {

	//creates a variable to store which super region I have troops in that has the least amount of regions left to take
	int lowestSuper = 100;

	//variables for the calculations
	Region regionAddingTo;
	int temp;

	//get all regions owned by me
	vector<Region> myRegions= bot->getOwnedRegions();

	//loop through all of my regions and find the region that is in the super region that is easiest to take
	for(Region r: myRegions)
	{
		//get the amount of regions left in the super region
		temp = bot->regionsLeftInSuper(bot->superRegions[r.superRegion]);

		//if there are less regions to take in this super region, add make this region the one we add troops to
		if(temp < lowestSuper && temp != 0)
		{
			lowestSuper = temp;
			regionAddingTo = r;
		}


	}

	//add all of the troops to the region that is in the easiest super region to take
	std::cout << bot->botName << " place_armies " << regionAddingTo.id << " " << bot->armiesLeft
	<< std::endl;

	//update the number of armies left in the bot class
	bot->addArmies(regionAddingTo.id, bot->armiesLeft);

	return;
}


void General::calculateTurn() {
	//This call attack and deffense and then combines the moves?
	vector<Move> moves = generateAttacks();

	sendToEngineAttack(moves);
}



vector<Move> General::generateAttacks()
{
	int counter = 0;


	vector<Move> moves;

	// Get adjacent regions depending on owner.
	vector<Region> myRegions = bot->getRegionsOwnedBy(ME);


	// Look at potential enemy attacks
	for (Region n : myRegions) {


		if(n.armies == 1)
			continue;

		vector<Region> Nb = bot->otherNbInSuperRegion(n);

		if(Nb.empty())
		{
			Nb = bot->getOtherNeighbors(n);

			if(Nb.empty())
			{
				//Nb = bot->getNeighbors(ME, n);

				Move transfer = calculateTransfer(n);

				//Need to push moves this way to prevent seg fault
				moves.push_back(Move());

				//you'll push to moves this way. Every other way seems to seg fault
				moves[counter].to = transfer.to;
				moves[counter].from = transfer.from;
				moves[counter++].armies = transfer.armies;

			}
		}

		Region attackingRegion;

		for(Region r: Nb)
		{
			attackingRegion = r;

			if(attackingRegion.owner == ENEMY)
				break;
		}

		if(!Nb.empty())
		{
			//Need to push moves this way to prevent seg fault
			moves.push_back(Move());

			//you'll push to moves this way. Every other way seems to seg fault
			moves[counter].to = attackingRegion;
			moves[counter].from = n;
			moves[counter++].armies = n.getArmies() - 1;
		}


	}


	return moves;
}

Move General::calculateTransfer(Region place)
{
	vector<Region> MyNb = bot->getNeighbors(ME, place);

	Move transfer;

	transfer.from = place;

	transfer.to = MyNb[rand() % MyNb.size()];

	transfer.armies = transfer.from.getArmies() - 1;

	return transfer;
}

void General::sendToEngineAttack(vector<Move> attacks)
{
	if(attacks.empty())
		return;

	std::vector<std::string> attackMoves;
	std::stringstream attackMove;

	for(Move m: attacks)
	{

			attackMove << bot->botName << " attack/transfer " << m.from.id << " " << m.to.id
				<< " " << m.armies << ",";



		attackMoves.push_back(attackMove.str());

	}
	std::cout << string::join(attackMoves) << std::endl;
}



