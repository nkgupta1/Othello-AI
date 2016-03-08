#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include "common.h"
#include "board.h"
#include "Node.h"
#include <vector>

#define XY(i, j) (i + 8 * j)
#define N         8
#define TESTDEPTH 2
#define MAXDEPTH  2
#define TIME      960000

using namespace std;

class Player {

public:
    Player(Side side);
    ~Player();

    Move *simpleHeuristic();
    Node recursiveHeuristic(Board * b, int depth, Side side);
    Move *doMove(Move *opponentsMove, int msLeft);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
    Board *gameBoard;
    Side us;
    Side them;
    int heuristic[64] = {  1000, 100, 100, 100, 100, 100, 100, 1000, 
                            100, -40, -10, -10, -10, -10, -40,  100,
                            100, -10,  -1,  -1,  -1,  -1, -10,  100,
                            100, -10,  -1,  -1,  -1,  -1, -10,  100,
                            100, -10,  -1,  -1,  -1,  -1, -10,  100,
                            100, -10,  -1,  -1,  -1,  -1, -10,  100,
                            100, -40, -10, -10, -10, -10, -40,  100,
                           1000, 100, 100, 100, 100, 100, 100, 1000};
};

#endif
