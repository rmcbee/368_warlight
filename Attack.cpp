#include <algorithm>
#include <map>
#include <set>

#include "Attack.h"

using namespace std;

Move Attack::move(Region from, Region to, int armies)
{

}

vector<Move> Attack::generateMoves()
{



	std::vector<Move> moves;

	// Get adjacent regions depending on owner.
	//vector<Region> adjacentOpponent = bot->getAdjacentPlayer(ENEMY);

	/*vector<Region> adjacentNeutral = bot->getAdjacentPlayer(NEUTRAL);

	// Look at potential enemy attacks
	for (Region n : adjacentOpponent) {
		vector<Region> myregions = bot->getNeighbors(ME, n);
		for (Region my : myregions) {


			moves.push_back(move(my, n, n.getArmies()));
		}
	}


	// Look at potential neutral attacks
	for (Region n : adjacentNeutral) {

		vector<Region> myregions = bot->getNeighbors(ME, n);

		for (Region my : myregions) {


			moves.push_back(move(my, n, n.getArmies()));
		}
	}*/
	cerr << endl;
	cerr << endl;
	cerr << endl;
	cerr << "hell yeah!";

	return moves;
	//return moves;
}
void Attack::doNothing(void)
{

}


