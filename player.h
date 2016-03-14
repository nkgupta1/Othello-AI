#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include "common.h"
#include "board.h"
#include "Node.h"
#include <vector>
#include <ctime>
#include <cmath>
#include <stdio.h>

#define XY(i, j)   (i + 8 * j)
#define TIMEDIFF   (1000 * time(0) - startTime)
#define N          8
#define TESTDEPTH  2
#define STARTDEPTH 3
#define MAXDEPTH   5
#define TIME       960000
#define TTIME      3000

using namespace std;

class Player {

public:
    Player(Side side);
    ~Player();

    int turnCount;
    long double startTime = time(0);
    long double remTime = time(0);

    void updateHeuristicsTL      (Board *b, int scoreMap[64], bool ourUpdate);
    void updateHeuristicsTR      (Board *b, int scoreMap[64], bool ourUpdate);
    void updateHeuristicsBL      (Board *b, int scoreMap[64], bool ourUpdate);
    void updateHeuristicsBR      (Board *b, int scoreMap[64], bool ourUpdate);
    void updateHeuristicsMiddleRC(Board *b, int scoreMap[64], bool ourUpdate);

    void appendToFile(FILE* f, bitset<128> toAppend);

    long double timeAllocator(bool oppMove, int msLeft);

    int scoreFunction(Board *b, int win);
    int simpleScoreFunction(Board *b);

    Move *simpleHeuristic();   
    Node minimax(Board *b, int depth, int enddepth, Side2 side);
    Node alphaBeta(Board *b, int depth, int enddepth, int alpha, int beta, Side2 side, int pass);
    Node MTDF(Board *b, int enddepth, int bound);

    Move *doMove(Move *opponentsMove, int msLeft);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
    // Temporary bool to test iterative deepening capabilities
    bool iterativeDeepening;
    // Temporary bool for MTD(f) algorithm
    bool mtd;

    Board *gameBoard;
    Side2 us;
    Side2 them;
    int heuristic[64] = {  1000,  50, 100, 100, 100, 100,  50, 1000, 
                             50, -40, -10, -10, -10, -10, -40,   50,
                            100, -10,  -1,  -1,  -1,  -1, -10,  100,
                            100, -10,  -1,  -1,  -1,  -1, -10,  100,
                            100, -10,  -1,  -1,  -1,  -1, -10,  100,
                            100, -10,  -1,  -1,  -1,  -1, -10,  100,
                             50, -40, -10, -10, -10, -10, -40,   50,
                           1000,  50, 100, 100, 100, 100,  50, 1000};
    FILE *f;
};

#endif