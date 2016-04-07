#ifndef DEFENSE_H
#define DEFENSE_H

#include "MoveGenerator.h"

using namespace std;

class Defense: public MoveGenerator {
public:
	Defense() {};
	virtual std::vector<Move> generateMoves();
private:

};

#endif // !DEFENSE_H