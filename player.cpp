#include "player.h"

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish 
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;
    gameBoard = new Board();
    us = side;
    them = (us == WHITE) ? BLACK : WHITE;
}

/*
 * Destructor for the player.
 */
Player::~Player() {
    delete gameBoard;
}

/* Calculates the best available move at 1-ply */
Move *Player::simpleHeuristic() {
    Move *bestmove =  new Move(0, 0);
    Move *m = new Move(0, 0);

    int best = -100; /* Lower value than present in heuristic */
    for (int i = 0; i < 64; i++) {
        m->setX(i % N);
        m->setY(i / N);
        if ((gameBoard->checkMove(m, us) == true)  &&
            (heuristic[i] > best)) {
                best = heuristic[i];
                bestmove->setX(i % N);
                bestmove->setY(i / N);
            }
    }
    delete m;
    return bestmove;
}

/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be NULL.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return NULL.
 */
Move *Player::doMove(Move *opponentsMove, int msLeft) {
    /* START TIME */

    /* Update board with opponent's move */
    gameBoard->doMove(opponentsMove, them);

    /* No legal moves to make */
    if (gameBoard->hasMoves(us) == false) {
        return NULL;
    }

    Move *heuristicMove;
    heuristicMove = simpleHeuristic();

    /* Update board with our move */
    gameBoard->doMove(heuristicMove, us);
    return heuristicMove;
}
