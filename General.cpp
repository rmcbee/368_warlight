#include "General.h"

using namespace std;
General::General(Bot* x) {
	bot = x;
}

General::~General() {
    
}

Move General::createMove(Region from, Region to, int armies)
{
	Move dothis;
	dothis.from = from;
	dothis.to = to;
	dothis.armies = armies;
}

int General::pickStartingRegions(std::vector<int> pickfrom) {
	//calculate front

	return pickfrom.front();
}
std::vector<Move> General::getAttack() {

	//need to finsih. Does this do the transfers or just call the attack function?
	this->generateAttacks();

	return std::vector<Move>();
}

std::vector<Move> General::getDefense() {

	//need to finsih. Does this do the transfers or just call the deffense function?
	return std::vector<Move>();
}


std::vector<Move> General::getDeployment() {

	//Calculate the deloyment. Again, isn't this done based on Defense, not because of Defense.
	return std::vector<Move>();
}


void General::calculateTurn() {
	//This call attack and deffense and then combines the moves?
}


bool sortByArmiesDifference(Move l, Move r) {
	return (l.armies - l.from.getArmies()) > (r.armies - r.from.getArmies());
}



vector<Move> General::generateAttacks()
{
	int counter = 0;


	vector<Move> moves;

	// Get adjacent regions depending on owner.
	vector<Region> adjacentOpponent = bot->getAdjacentPlayer(ENEMY);

	vector<Region> adjacentNeutral = bot->getAdjacentPlayer(NEUTRAL);




	// Look at potential enemy attacks
	for (Region n : adjacentOpponent) {
		vector<Region> myregions = bot->getNeighbors(ME, n);
		for (Region my : myregions) {


			//Need to push moves this way to prevent seg fault
			moves.push_back(Move());

			//you'll push to moves this way. Every other way seems to seg fault
			moves[counter].to = my;
			moves[counter].from = n;
			moves[counter++].armies = n.getArmies();
		}
	}

	// Look at potential neutral attacks
	for (Region n : adjacentNeutral) {

		vector<Region> myregions = bot->getNeighbors(ME, n);

		for (Region my : myregions) {

			moves.push_back(Move());

			moves[counter].to = my;
			moves[counter].from = n;
			moves[counter++].armies = n.getArmies();
		}
	}

	for(Move r: moves)
	{
		cerr  << "move: " << r.from.id;
		cerr << endl;
	}
	if(!moves.size())
	{
		cerr << "empty" << endl;
	}

	return moves;
	//return moves;
}


