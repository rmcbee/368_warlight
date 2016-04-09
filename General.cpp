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


void General::getDeployment() {

	//Calculate the deloyment. Again, isn't this done based on Defense, not because of Defense.

	int lowestSuper = 100;
	Region regionAddingTo;
	int temp;

	vector<Region> myRegions= bot->getOwnedRegions();

	for(Region r: myRegions)
	{
		temp = bot->regionsLeftInSuper(bot->superRegions[r.superRegion]);

		if(temp < lowestSuper && temp != 0)
		{
			lowestSuper = temp;
			regionAddingTo = r;
		}


	}

	std::cout << bot->botName << " place_armies " << regionAddingTo.id << " " << bot->armiesLeft
	<< std::endl;

	bot->addArmies(regionAddingTo.id, bot->armiesLeft);

	return;
}


void General::calculateTurn() {
	//This call attack and deffense and then combines the moves?
	vector<Move> moves = generateAttacks();


	sendToEngineAttack(moves);
}


bool sortByArmiesDifference(Move l, Move r) {
	return (l.armies - l.from.getArmies()) > (r.armies - r.from.getArmies());
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

		vector<Region> Nb = bot->NbInSuperRegion(n);

		if(Nb.empty())
		{
			Nb = bot->getAllNeighbors(n);

			if(Nb.empty())
			{
				Nb = bot->getNeighbors(ME, n);
			}
		}

		Region attackingRegion;

		for(Region r: Nb)
		{
			attackingRegion = r;

			if(attackingRegion.owner == ENEMY)
				break;
		}


		//Need to push moves this way to prevent seg fault
		moves.push_back(Move());

		//you'll push to moves this way. Every other way seems to seg fault
		moves[counter].to = attackingRegion;
		moves[counter].from = n;
		moves[counter++].armies = n.getArmies() - 1;




	}




	return moves;
	//return moves;
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



