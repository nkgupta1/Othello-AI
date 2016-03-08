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
    Move *bestMove =  new Move(0, 0);
    Move *m = new Move(0, 0);

    int best = -100; /* Lower value than present in heuristic */
    for (int i = 0; i < 64; i++) {
        m->setX(i % N);
        m->setY(i / N);
        if ((gameBoard->checkMove(m, us) == true)  &&
            (heuristic[i] > best)) {
                best = heuristic[i];
                bestMove->setX(i % N);
                bestMove->setY(i / N);
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
    // 1-ply
    // heuristicMove = simpleHeuristic();

    // Recursive Heuristic
    if (testingMinimax) {
        heuristicMove = recursiveHeuristic(gameBoard->copy(), 0, us);
    }



    /* Update board with our move */
    test = gameBoard->doMove(heuristicMove, us);

    return heuristicMove;
}

Node recursiveHeuristic(Board *b, int depth, Side side) {
    if (depth == MAXDEPTH || !b->hasMove(side)) {
        // Change scoring function to more advanced
        int score = (side == WHITE) ? b->countWhite() - b->countBlack()
                                    : b->countBlack() - b->countWhite();
        return Node(score);
    }

    // want to maximize
    if (side == us) {
        int best = -100;
        // temp move
        Move *m = new Move(0,0);
        Move *bestMove = new Move(0, 0);
        for (int i = 0; i < 64, i++) {
            m->setX(i % N);
            m->setY(i / N);
            if (gameBoard->checkMove(m, side) == true) {
                vector<Move> moves = b->doMove(m, side);
                Node n = recursiveHeuristic()
            }
        }
    }
    // want to minimize
    else {

    }
}
