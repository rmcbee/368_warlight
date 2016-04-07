#pragma once

#include <vector>

#include "Move.h"

class MoveGenerator {
public:
	virtual std::vector<Move> generateMoves() = 0;
};