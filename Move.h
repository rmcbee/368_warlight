#ifndef MOVE_H
#define MOVE_H

#include "Region.h"

struct Move {

    Region* from;
    Region* to;
    int armies;
};



#endif // !MOVE_H