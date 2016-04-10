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

    /** std::vector<Move> generateNonAttack();
    * Calculates the suggested movement of troops that are not going to attack
    */
    std::vector<Move> generateNonAttack();

    /*
     * input: none
     *
     * output: none
     *
     * This function calculates and executes which troops should be deployed for the round. This function is independent of
     * the attack and defense functionality in order to make the overall generals job simpiler. This will be one of the key
     * algorithms we will show off for this project.
     */
    void getDeployment();
        
    /** void calculateTurn(float time);
     *  This function prioritizes different troop placements and
     *  prioritizes different attacks and moves taking into account the
     *  results from getAttack and generateNonAttack.
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
