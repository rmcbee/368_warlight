#ifndef GENERAL_H
#define GENERAL_H

#include <vector>


#include "Move.h"
#include <iostream>
#include <fstream>
#include "MoveGenerator.h"

#include "Bot.h"
#include <set>


using namespace std;

class Attack;

class General {
public:
	// Constructor/Destructor
    General(Bot* x);
    virtual ~General();

	/** int pickStartingRegions(std::vector<int> pickfrom);
	 * @param std::vector<Region&> pickfrom List of possible regions to start with.
	 * Returns the desired starting regions.
	 */
	int pickStartingRegions(std::vector<int> pickfrom);

    /** std::vector<Move> getAttack();
	 * Calculates the suggested attacks for this turn.
	 */
	std::vector<Move> getAttack();

    /** std::vector<Move> getDefense();
    * Calculates the suggested attacks for this turn.
    */
    std::vector<Move> getDefense();
        
    /** std::vector<Move> getDeployment();
	 * Returns the calculated deployment for this turn.
     */  
    void getDeployment();
        
    /** void calculateTurn(float time);
     *  This function prioritizes different troop placements and
     *  prioritizes different attacks and moves taking into account the
     *  results from getAttack and getDefense.
     */
    void calculateTurn();

    Bot* bot;

    vector<Move> generateAttacks();

    void sendToEngineAttack(vector<Move> attacks);

private:
    // stop making class variables for attack and defense
    std::vector<MoveGenerator *> advisors;


};

#endif // !GENERAL_H
