#ifndef ATTACK_H
#define ATTACK_H

//#include "MoveGenerator.h"

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
#include "Bot.h"


#include "Move.h"

using namespace std;


class Attack {
public:
	Attack(Bot* x) {bot = x;};
	vector<Move> generateMoves();
	Move move(Region from, Region to, int armies);
	Bot* bot;
	void doNothing(void);
private:


};

#endif // !ATTACK_H
