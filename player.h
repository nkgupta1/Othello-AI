#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include "common.h"
#include "board.h"
#include "Node.h"
#include <vector>
#include <ctime>

#define XY(i, j)   (i + 8 * j)
#define N          8
#define TESTDEPTH  2
#define STARTDEPTH 3
#define MAXDEPTH   7
#define TIME       960000
#define TTIME      1000

using namespace std;

class Player {

public:
    Player(Side side);
    ~Player();

    int turnCount;

    Move *simpleHeuristic();
    Node minimax(Board *b, int depth, int enddepth, Side side);
    Node alphaBeta(Board *b, int depth, int enddepth, int alpha, int beta, Side side, int pass);
    Move *doMove(Move *opponentsMove, int msLeft);
    int scoreFunction(Board *b, int win);
    int simpleScoreFunction(Board *b);
    void updateHeuristics(Board *b, int scoreMap[64]);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
    // Temporary bool to test iterative deepening capabilities
    bool iterativeDeepening;
    Board *gameBoard;
    Side us;
    Side them;
    int heuristic[64] = {  1000,  50, 100, 100, 100, 100,  50, 1000, 
                             50, -40, -10, -10, -10, -10, -40,   50,
                            100, -10,  -1,  -1,  -1,  -1, -10,  100,
                            100, -10,  -1,  -1,  -1,  -1, -10,  100,
                            100, -10,  -1,  -1,  -1,  -1, -10,  100,
                            100, -10,  -1,  -1,  -1,  -1, -10,  100,
                             50, -40, -10, -10, -10, -10, -40,   50,
                           1000,  50, 100, 100, 100, 100,  50, 1000};
};

#endif