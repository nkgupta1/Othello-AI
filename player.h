#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include "common.h"
#include "board.h"
using namespace std;

class Player {
private: 
    int heuristic[64] = {
        1000,  100,  100,  100,  100,  100,  100, 1000,
         100,  -40,  -10,  -10,  -10,  -10,  -40,  100,
         100,  -10,    1,    1,    1,    1,  -10,  100,
         100,  -10,    1,    1,    1,    1,  -10,  100,
         100,  -10,    1,    1,    1,    1,  -10,  100,
         100,  -10,    1,    1,    1,    1,  -10,  100,
         100,  -40,  -10,  -10,  -10,  -10,  -40,  100,
        1000,  100,  100,  100,  100,  100,  100, 1000
    };
    Board *b;
    Side mySide;
    Side oppSide;
public:
    Player(Side side);
    ~Player();

    
    Move *doMove(Move *opponentsMove, int msLeft);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
};

#endif
