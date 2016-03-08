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
    return bestMove;
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
/*    if (testingMinimax) {*/
        Node best = recursiveHeuristic(gameBoard->copy(), 0, us);

        // //fprintf(stderr, "Start\n");
        // for (unsigned int i = 0; i < best.getMoves().size(); i++)
        // {
        // 	std::cerr << "X: " << best.getMoves()[i].getX() << " Y: " << best.getMoves()[i].getY() << std::endl;
        // }
        // //fprintf(stderr, "Finish\n");
        
        // heuristicMove = &best.getMoves()[0];
/*    }*/

    heuristicMove = new Move(best.getMoves()[0].getX(), best.getMoves()[0].getY());

    // std::cerr << "TESTX: " << heuristicMove->getX() << " Y: " << heuristicMove->getY() << std::endl;

    /* Update board with our move */
    gameBoard->doMove(heuristicMove, us);

    return heuristicMove;
}

/**
 * Calculates score from heuristic table
 */
int Player::scoreFunction(Board *b) {
    int score;
    int whitescore = 0;
    int blackscore = 0;
    for (unsigned int i = 0; i < 64; i++) {
        if (b->getPiece(i) == 'b') {
            blackscore += heuristic[i];
        }
        else if (b->getPiece(i) == 'w') {
            whitescore += heuristic[i];
        }
    }
    score = whitescore - blackscore;
    if (us == WHITE) {
        return score;
    }
    return -1 * score;
}

Node Player::recursiveHeuristic(Board *b, int depth, Side side) {
    //fprintf(stderr, "START Depth %d\n", depth);
    // printBoard(b);
    int maxD;
    // //fprintf(stderr, "Depth %d\n\n", depth);
    if (testingMinimax) {
        maxD = TESTDEPTH;
    }
    else {
        maxD = MAXDEPTH; 
    }

    if (depth == maxD || !b->hasMoves(side)) {
        //fprintf(stderr, "END Depth %d\n", depth);
        // printBoard(b);
        int score;
        // if (testingMinimax) {
            // Change scoring function to more advanced
            score = (us == WHITE)   ? b->countWhite() - b->countBlack()
                                    : b->countBlack() - b->countWhite();
            // //fprintf(stderr, "Score %d\n", score);
        // }
        if (!testingMinimax) {
            //Modified simple heuristic
            score *= 5;
            score += scoreFunction(b);
        }
        return Node(score);
    }


    // want to maximize
    if (side == us) {
        Node best(-100);
        // temp move
        Move *m = new Move(0,0);
        for (int i = 0; i < 64; i++) {
            m->setX(i % N);
            m->setY(i / N);
            if (gameBoard->checkMove(m, side) == true) {
            	// //fprintf(stderr, "At depth %d\n", depth);
            	// //fprintf(stderr, "hi\n");
                vector<Move> moves = b->doMove(m, side);

                //fprintf(stderr, "US Depth %d\n", depth);
                // printBoard(b);

                // //fprintf(stderr, "X: %d Y: %d\n", m->getX(), m->getY());


                // //fprintf(stderr, "Start\n");
                // for (unsigned int i = 0; i < moves.size(); i++)
                // {
                //  std::cerr << "X: " << moves[i].getX() << " Y: " << moves[i].getY() << std::endl;
                // }
                // //fprintf(stderr, "Finish\n");

                Node n = recursiveHeuristic(b, depth + 1, switchSide(side));
                n.setMoves(moves);
                best = (best.getScore() > n.getScore())? best : n;
                b->undoMove(n.getMoves(), side);
            }
        }
        // std::cerr << "X: " << best.getMoves()[0].getX() << " Y: " << best.getMoves()[0].getY() << std::endl;
        return best;
    }

    // want to minimize
    else {
        // std::cerr << "WRONG!\n" << std::endl;
        Node best(2000);
        // temp move
        Move *m = new Move(0,0);
        for (int i = 0; i < 64; i++) {
            m->setX(i % N);
            m->setY(i / N);
            if (gameBoard->checkMove(m, side) == true) {
                // printMove(m);
                //fprintf(stderr, "%d\n", gameBoard->checkMove(m, side));
                //fprintf(stderr, "%d\n", side == BLACK);
                vector<Move> moves = b->doMove(m, side);

                //fprintf(stderr, "THEM Depth %d\n", depth);
                // printBoard(b);

                // //fprintf(stderr, "OPP X: %d Y: %d\n", m->getX(), m->getY());
                Node n = recursiveHeuristic(b, depth + 1, switchSide(side));
                n.setMoves(moves);
                best = (best.getScore() < n.getScore())? best : n;
                b->undoMove(n.getMoves(), side);
            }
        }
        return best;
    }
}
