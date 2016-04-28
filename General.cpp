#include <map>
#include <random>
#include <string>
#include <queue>
#include <stdlib.h>
#include "tools/StringManipulation.h"
#include "General.h"
#include "Region.h"
#include <math.h>


General::General(Bot* x) {
	bot = x;
}

General::~General() {
    
}


int General::pickStartingRegions(std::set<int> pickfrom) {
	std::map<int, double> regions;
	for (int region : pickfrom) {
		double score = 0;

		double centralvalue = 1, supervalue = 0.5;

		// Centrality of region
		// Negative because less central is a bigger number
		score += centralvalue * -bot->centrality(region);

		// Value of region
		score += supervalue * bot->superRegions[bot->regions[region].superRegion].reward;

		regions.insert(std::pair<int, int>(region, score));
	}

	// Get the highest value region
	std::pair<int, double> r(0, 0);
	for (std::pair < int, double > region : regions)
		if (r.second < region.second || r.first == 0) r = region;
	return r.first;
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

	int numAddingTo = 3;
	int weight;

	//vector<pair<Region*, int>> regionAddingTo; //variables for the calculations
	vector<Region*> myRegions= bot->getOwnedRegions(); //get all regions owned by me



	std::vector<std::string> deployMoves; //vetor of moves that will deploy the troops for this round

	Region* rankRegion[100];
	int rankWeight[100] = {};
	int totalweights = 0;

	for(int i = 0; i < numAddingTo; i++)
	{
		rankRegion[i] = NULL;
	}

	//loop through all of my regions and find the region that is in the super region that is easiest to take
	for(Region* r: myRegions) {



		weight = getWeight(r);
		totalweights += weight;


		int lowest = 0;

		for(int i = 1; i < numAddingTo; i++)
		{
			if(rankWeight[i] < rankWeight[lowest])
				lowest = i;
		}

		if(weight > lowest)
		{
			rankRegion[lowest] = r;
			rankWeight[lowest] = weight;
		}
		//if(weight > 0) break;

	}


	int armiesRemaining = bot->armiesLeft;

	for(int i = 0; i < numAddingTo; i++)
	{
		std::stringstream deployMove;
		if(rankRegion[i] == NULL)
			continue;

		int weightArmies = (bot->armiesLeft * rankWeight[i]) / totalweights;

		if(weightArmies == 0)
			continue;

		deployMove << bot->botName << " place_armies " << rankRegion[i]->id << " " << weightArmies;

		bot->addArmies(rankRegion[i]->id, weightArmies);

		deployMoves.push_back(deployMove.str());

		//update the number of armies left in the bot class
		armiesRemaining -= weightArmies;
	}



	if(armiesRemaining)
	{

		int highest = 0;

		for(int i = 1; i < numAddingTo; i++)
		{
			if(rankWeight[i] > rankWeight[highest])
				highest = i;
		}



		std::stringstream deployMove;

		deployMove << bot->botName << " place_armies " << rankRegion[highest]->id << " " << bot->armiesLeft;


		//cerr << "test DATA: " << highest << " " << rankWeight[highest] << endl;

		//update the number of armies left in the bot class
		bot->addArmies(rankRegion[highest]->id, bot->armiesLeft);


		deployMoves.push_back(deployMove.str());
	}

	std::cout << join(deployMoves) << endl;

	return;
}

void General::sortRankedDeployments(vector<pair<Region*, int> >& deployments)
{
	size_t pass, index;
	pair<Region*, int> valpair;

	for(pass = 1; pass < deployments.size(); pass++)
	{
		valpair = deployments[pass];
		index = pass - 1;
		while(index >= 0 && deployments[index].second < valpair.second)
		{
			deployments[index + 1] = deployments [index];
			index--;
		}
		deployments[index + 1] = valpair;
	}
	return;
}

int General::getWeight(Region* place)
{
	int weight = 1;

	//weight based on who is around you

	//if I am surrounded by myself, return 0
	vector<Region*> neighborsNotMe = bot->getOtherNeighbors(place);

	if(neighborsNotMe.empty())
		return weight;

	vector<Region*> neighborsMe = bot->getNeighbors(ME, place);

	//weight based on super region
	SuperRegion super = bot->superRegions[place->superRegion];

	//calculates the rank of this super reigon



	//how valuable is the super region this region is in
	int RegionsLeft = bot->regionsLeftInSuper(super);
	int superRank = super.reward * (super.size() - RegionsLeft) * (super.size() - RegionsLeft);

	//Is this region at risk from being taken?
	/*vector<Region*> enemies = bot->getNeighbors(ENEMY, place);
	vector<Region*> neutral = bot->getNeighbors(NEUTRAL, place);

	double troopDiff = 0;
	int worth = 0;


	//some enemy neihbors
	if(!enemies.empty())
	{
		for(Region* r: enemies)
		{
			//chpeck troop difference
			troopDiff = (place->armies - r->armies) / place->armies;

			//place worth on taking the region
			worth = 1;

			if(place->superRegion == r->superRegion)
			{
				worth += 1;
			}


			double value = troopDiff * worth;

			if(troopDiff >= 1)
			{
				//add weight based on troop diff
				weight += value * 2;
				//continue
			}
			else if(troopDiff >= 0.3)
			{
				//add num troops based on troop diff
				//add troops to ATTACK!!!!!
				weight += value * 3;
				//add in luck factor?
				//continue
			}
			else if(troopDiff >= 0)
			{
				//add no troops
				weight += value;
				//continue;
				//continue
			}

			//defend
			else
			{
				//recalculate worth
				worth = 1;
				//check if this is defending other regions
					//check if the super region is worth keeping


				//recalculate value
				value = troopDiff * worth;

				if(troopDiff >= -0.5)
				{
					//add some troops for defense
					weight += value;
					//add in luck factor??
					//continue
				}
				else
				{
					//check if out current region is needed

					//if needed
					//add troops!!!!!
					weight += value * 2;

					//else
					//bye bye
				}
			}

		}
	}
	//all neutral neibhors
	else
	{
		if(RegionsLeft != 0)
			weight += superRank;
	}*/

	if(RegionsLeft != 0)
		weight += superRank;

	return weight;
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
	vector<Region*> myRegions = bot->getOwnedRegions();




	// Look at potential enemy attacks
	for (Region* n : myRegions) {


		if(n->armies == 1)
			continue;



		vector<Region*> Nb = bot->otherNbInSuperRegion(n);



		if(Nb.empty())
		{
			Nb = bot->getOtherNeighbors(n);

			if(Nb.empty())
			{
				Move transfer = calculateTransfer(n);

				//Need to push moves this way to prevent seg fault
				moves.push_back(Move());

				//you'll push to moves this way. Every other way seems to seg fault
				moves[counter].to = transfer.to;
				moves[counter].from = transfer.from;
				moves[counter++].armies = transfer.armies;

				continue;
			}
		}


		for(Region* r: Nb)
		{
			if(r->owner == ENEMY)
			{
				//you'll push to moves this way. Every other way seems to seg fault
				if(n->armies >= r->armies * 2)
				{

					//Need to push moves this way to prevent seg fault
					moves.push_back(Move());
				
					moves[counter].to = r;
					moves[counter].from = n;
					moves[counter++].armies = r->armies * 2;
					n->setArmies(n->armies - r->armies * 2);
				}	
			}
			else if(r->owner == NEUTRAL)
			{
				//you'll push to moves this way. Every other way seems to seg fault
				if(n->armies >= r->armies * 2)
				{

					//Need to push moves this way to prevent seg fault
					moves.push_back(Move());

					moves[counter].to = r;
					moves[counter].from = n;
					moves[counter++].armies = r->armies * 2;
					n->setArmies(n->armies - r->armies * 2);
				}
			}

		}
		if(n->armies){
			//Need to push moves this way to prevent seg fault
			moves.push_back(Move());

			moves[counter].to = Nb[0];
			moves[counter].from = n;
			moves[counter++].armies = n->armies;
			n->setArmies(0);
		}


	}

	return moves;
}

int General::rateNeed(Region* region){

	int need = 0;

	vector<Region*> Nb = bot->getOtherNeighbors(region);

	SuperRegion super = bot->superRegions[region->superRegion];

	need += Nb.size();
    for(Region* region : Nb){
		need += region->getArmies();
	}
	need += super.reward * 2;

	return need;

}

Region* General::getTransferEndDest(){
    std::vector<Region*> owned = bot->getRegionsOwnedBy(ME);
	//cerr << "size: " << owned.size() << endl;

	Region* destination = NULL;
    for(Region* possEnd : owned){
		//cerr << "id: " << possEnd->id << endl;
		if(destination == NULL) destination = possEnd;
        if(rateNeed(possEnd) > rateNeed(destination)){
            destination = possEnd;
        }
    }
	//cerr << "id: " << destination->id << endl;

    return destination;
}

/** \brief Calculates the next step in the shortest path from start to last.
 *
 * \param start Region* the starting region
 *
 * \param last Region* the target region
 * \return Region* the region adjacent to start to move to
 *
 */     
Region* General::nextStep(Region* start, Region* last){
    int numRegions = bot->regions.size();
    bool* visited = (bool*) calloc(numRegions + 1, 1); //each index refers to a region id
    int numVisited = 0;
    std::queue<Region*> pathQueue;
    pathQueue.push(last);
    while(numVisited < numRegions){
        Region* region = pathQueue.front();
        pathQueue.pop();
        std::vector<int> neighbors = bot->getNeighbors(region->id);
        for(int neighbor : neighbors){
            if(visited[neighbor]) continue;
            if(neighbor == start->id){
                free(visited);
                return region;
            }
            visited[neighbor] = true;
            pathQueue.push(&bot->regions[neighbor]);
            numVisited++;
        }
    }
    free(visited);
    return NULL;
}

Move General::calculateTransfer(Region *place) {



	Move transfer;

	bot->getNeighbors(ME, place);
	vector<Region*> Nb = bot->getNeighbors(ME, place);

	if(!Nb.empty())
	{
		transfer.from = place;

		//transfer.to = Nb[rand() % Nb.size()];

		transfer.to = nextStep(place, getTransferEndDest());

		cerr << "destination " << transfer.to->id << endl << endl;

		transfer.armies = transfer.from->getArmies() - 1;

	}


	/*

	;*/

	return transfer;
}

void General::sendToEngineAttack(vector<Move> attacks)
{
	if(attacks.empty())
	{
		std::cout << "No moves" << endl;
		return;
	}


	std::vector<std::string> attackMoves;
	//std::stringstream attackMove;

	for(Move m: attacks) {
		std::stringstream attackMove;


		attackMove << bot->botName << " attack/transfer " << m.from->id << " " << m.to->id
		<< " " << m.armies;


		attackMoves.push_back(attackMove.str());

	}

	//for(int i = 0; i < attackMoves.size(); i++)
		//std::cout << attackMoves[i] << std::endl;
		std::cout << join(attackMoves) << std::endl;
}



