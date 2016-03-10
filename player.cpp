#include "player.h"

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish 
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;

    // Temporary measure right now for testing
    iterativeDeepening = true;

    // Temporary for MTD(f)
    mtd = false;

    turnCount = 1;
    gameBoard = new Board();
    us = (side == BLACK);
    them = switchSide2(us);
}

/*
 * Destructor for the player.
 */
Player::~Player() {
    delete gameBoard;
}

void Player::updateHeuristics(Board *b, int scoreMap[64]) {
    /* Nikhil, you want to do this one? */
}

long double Player::timeAllocator(bool oppMove, int msLeft) {
    if (msLeft == -1) {
        /* No time limit */
        return TTIME;
    }
    else if (oppMove == false) {
        /* Opponent passed; probably not too many moves we have to think through */
        return min(20000, (msLeft / 4));
    }
    else {
        /* We have 30 planned turns; factor tells us which it is. */
        int turnFactor = (turnCount + 1) / 2;
        double turnMultiplier;
        /* Last turns don't take long at all. */
        if (turnFactor < 25) {
            turnMultiplier = 1;
        }
        else {
            turnMultiplier = pow(0.75, 30 - turnFactor);
        }
        int turnTime = 1000 * (0.6 * turnFactor + 24) * turnMultiplier;
        return min(turnTime, msLeft / 4);
    }
}

/**
 * Calculates score from heuristic table
 */
int Player::scoreFunction(Board *b, int win) {
    int score;
    int whitescore = 0;
    int blackscore = 0;

    /* Both players can only pass => someone won */
    if (win == 1) {
        return simpleScoreFunction(b);
    }

    for (unsigned int i = 0; i < 64; i++) {
        if (b->getPiece(i) == 'b') {
            blackscore += heuristic[i];
        }
        else if (b->getPiece(i) == 'w') {
            whitescore += heuristic[i];
        }
    }
    score = whitescore - blackscore;
    if (isWhite(us)) {
        return score;
    }
    return -1 * score;
}

/**
 * Simple difference of sqaures.
 * Arbitrarily large/small amounts for winning/losing
 */
int Player::simpleScoreFunction(Board *b) {
    int score = (isWhite(us))   ? b->countWhite() - b->countBlack()
                                : b->countBlack() - b->countWhite();

    /**
     * Game is over; let's see who won 
     * '+ score' is so that AI emphasizes
     * overwhelming victories/closer defeats.
     */
    if (turnCount >= 60) {
        if (isWhite(us) && (score > 0)) {
            return 9000 + score;
        }
        else if (isBlack(us) && (score > 0)) {
            return 9000 + score;
        }
        else {
            return -9000 + score;
        }
    }

    if (isWhite(us)) {
        if (b->countWhite() == 0) {
            return -9000 + score;
        }
        else if (b->countBlack() == 0) {
            return 9000 + score;
        }
    }
    else if (isBlack(us)) {
        if (b->countWhite() == 0) {
            return 9000 + score;
        }
        else if (b->countBlack() == 0) {
            return -9000 + score;
        }
    } 
    return score;
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

/**
 * Incorporates the minimax algorithm.
 */
Node Player::minimax(Board *b, int depth, int enddepth, Side2 side) {
    if ((depth == enddepth) || (!b->hasMoves(side))) {
        if (testingMinimax) {
            return Node(simpleScoreFunction(b));
        }
        return Node(scoreFunction(b, 0));
    }

    if (isEqual(side, us)) {
        Node result;
        Move bestMove;
        int best = -10000;
        for (unsigned int i = 0; i < 64; i++) {
            Move *curr = new Move(i % N, i / N);
            Node test;
            if (b->checkMove(curr, side)) {

                vector<Move> testmove;
                testmove = b->doMove(curr, side);
                turnCount += 1;
                test = minimax(b, depth + 1, enddepth, switchSide2(side));
                int testscore = test.getScore();

                if (testscore > best) {
                    best = testscore;
                    bestMove.setX(curr->getX());
                    bestMove.setY(curr->getY());
                }
                b->undoMove(testmove, side);
                turnCount -= 1;
            }
            delete curr;
        }
        result.setSingleMove(bestMove);
        result.setScore(best);
        return result;
    }

    else {
        Node result;
        Move bestMove;
        int best = 10000;
        for (unsigned int i = 0; i < 64; i++) {
            Move *curr = new Move(i % N, i / N);
            Node test;
            if (b->checkMove(curr, side)) {

                vector<Move> testmove;
                testmove = b->doMove(curr, side);
                turnCount += 1;
                test = minimax(b, depth + 1, enddepth, switchSide2(side));
                int testscore = test.getScore();

                if (testscore < best) {
                    best = testscore;
                    bestMove.setX(curr->getX());
                    bestMove.setY(curr->getY());
                }
                b->undoMove(testmove, side);
                turnCount -= 1;
            }
            delete curr;
        }
        result.setSingleMove(bestMove);
        result.setScore(best);
        return result;
    }
}

/**
 * Minimax with alpha-beta pruning. Even better.
 */
Node Player::alphaBeta(Board *b, int depth, int enddepth, int alpha, int beta, Side2 side, int pass) {

    /* At some point, good idea to implement a transposition table: */

    // if (mtd) {
    //     addToTable
    //}

    /* (Before every return statement except the time one) */

    /* TIME CUTOFF - just return an empty node */
    if (TIMEDIFF > 0.9 * remTime) {
        Node result;
        return result;
    }
    /**
     * One side needs to pass; move to next layer down
     * One side passes after another passed; game over, find score.
     * This is done even if means going below depth; wins/losses
     * are important.
     */
    if (!b->hasMoves(side)) {
        if (pass == 0) {
            Node result;
            result = alphaBeta(b, depth + 1, enddepth, alpha, beta, switchSide2(side), 1);
            return result;
        }
        else if (pass == 1) {
            return Node(scoreFunction(b, 1));
        }
    }

    if (depth >= enddepth) {
        if (testingMinimax) {
            return Node(simpleScoreFunction(b));
        }
        return Node(scoreFunction(b, 0));
    }

    if (isEqual(us, side)) {
        Node result;
        Move bestMove;
        int best = -10000;
        for (unsigned int i = 0; i < 64; i++) {
            Move *curr = new Move(i % N, i / N);
            Node test;
            if (b->checkMove(curr, side)) {

                vector<Move> testmove;
                testmove = b->doMove(curr, side);
                turnCount += 1;
                test = alphaBeta(b, depth + 1, enddepth, alpha, beta, switchSide2(side), 0);

                /* TIME CUTOFF - just return an empty node */
                if (TIMEDIFF > 0.9 * remTime) {
                    delete curr;
                    b->undoMove(testmove, side);
                    turnCount -= 1;
                    Node result;
                    return result;
                }

                int testscore = test.getScore();

                if (testscore > best) {
                    best = testscore;
                    bestMove.setX(curr->getX());
                    bestMove.setY(curr->getY());
                }
                b->undoMove(testmove, side);
                turnCount -= 1;

                alpha = max(alpha, best);
            }
            delete curr;
            if (beta <= alpha) { /* Beta cutoff : fail-high */
                break;
            }
        }
        result.setSingleMove(bestMove);
        result.setScore(best);
        return result;
    }

    else {
        Node result;
        Move bestMove;
        int best = 10000;
        for (unsigned int i = 0; i < 64; i++) {
            Move *curr = new Move(i % N, i / N);
            Node test;
            if (b->checkMove(curr, side)) {

                vector<Move> testmove;
                testmove = b->doMove(curr, side);
                turnCount += 1;
                test = alphaBeta(b, depth + 1, enddepth, alpha, beta, switchSide2(side), 0);

                /* TIME CUTOFF - just return an empty node */
                if (TIMEDIFF > 0.9 * remTime) {
                    delete curr;
                    b->undoMove(testmove, side);
                    turnCount -= 1;
                    Node result;
                    return result;
                }

                int testscore = test.getScore();

                if (testscore < best) {
                    best = testscore;
                    bestMove.setX(curr->getX());
                    bestMove.setY(curr->getY());
                }
                b->undoMove(testmove, side);
                turnCount -= 1;

                beta = min(best, best);
            }
            delete curr;
            if (beta <= alpha) { /*Alpha cutoff : fail-low */
                break;
            }
        }
        result.setSingleMove(bestMove);
        result.setScore(best);
        return result;
    }
}

/**
 * Implementation of MTD(F), fastest known minimax algorithm
 * Need to implement a transposition table before it'll really work, though
 */
Node Player::MTDF(Board *b, int enddepth, int bound) {
    Node best;
    int currBound = bound;
    int upperBound = 1000000;
    int lowerBound = -1000000;
    while (1) {
        Node test;
        int beta = (currBound == lowerBound) ? currBound + 1 : currBound;
        test = alphaBeta(b, 0, enddepth, beta - 1, beta, us, 0);
        currBound = test.getScore();
        if (currBound < beta) {
            upperBound = currBound;
        }
        else {
            lowerBound = currBound;
        }

        if (lowerBound >= upperBound) {
            best = test;
            break;
        }

        if (TIMEDIFF > 0.9 * remTime) {
            Node result;
            return result;
        }
    }
    return best;
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
    startTime = 1000 * time(0);

    /* Update board with opponent's move */
    gameBoard->doMove(opponentsMove, them);

    /* Update the turn counter */
    if (opponentsMove != NULL) {
        turnCount += 1;
    }

    /* If move isn't null, then update */
    bool oppMove = true;
    turnCount += 1;
    /* But, if move is null... */
    if (opponentsMove == NULL) {
        /*...Stil at same turn. Have to allocate extra time for unexpected turn. */
        turnCount -= 1;
        oppMove = false;
        /* Except for case where game just started, that's okay */
        if (turnCount == 1) {
            oppMove = true;
        }
    }


    /* For testing purposes, don't want longer than 10 sec */
    remTime = timeAllocator(oppMove, msLeft);

    /* Update heuristics to be more accurate */
    updateHeuristics(gameBoard, heuristic); 

    /* No legal moves to make */
    if (gameBoard->hasMoves(us) == false) {
        return NULL;
    }

    Move *heuristicMove;
    Node best;

    if (testingMinimax) {
        best = minimax(gameBoard, 0, TESTDEPTH, us);
    }
    else if (iterativeDeepening) {
        int depth = STARTDEPTH;
        Node test;
        do {
            fprintf(stderr, "Depth reached: %d\n", depth);
            test = alphaBeta(gameBoard, 0, depth, -100000, 100000, us, 0);
            /* Make sure that the last input gave a valid result */
            if (test.getSingleMove().getX() != -1) {
                best = test;
            }
            depth += 2;
            std::cerr << "Time difference: " << 1000 * time(0) - startTime << std::endl;
        } while ((TIMEDIFF < 0.5 * remTime) && ((turnCount + depth) <= 62)); //Will get a better iterative standard at some point
    }
    else if (mtd) {
        int depth = STARTDEPTH;
        int startValue = 0;
        Node test;
        do { /* Please make sure to set up a transposition table */
            // fprintf(stderr, "Depth reached: %d\n", depth);
            test = MTDF(gameBoard, depth, startValue);
            /* Make sure that the last input gave a valid result */
            if (test.getSingleMove().getX() != -1) {
                best = test;
                startValue = test.getScore();
                // fprintf(stderr, "Current startValue: %d\n", startValue);
            }
            depth += 2;
            // std::cerr << "Time difference: " << 1000 * time(0) - startTime << std::endl;
        } while ((TIMEDIFF < 0.5 * remTime) && ((turnCount + depth) <= 62)); //Will get a better iterative standard at some point

    }
    else {
        best = alphaBeta(gameBoard, 0, MAXDEPTH, -100000, 100000, us, 0);
    }

    heuristicMove = new Move(best.getSingleMove().getX(), best.getSingleMove().getY());

    /* Make sure that move is legal */
    if (!gameBoard->checkMove(heuristicMove, us)) {
        // fprintf(stderr, "Oopsies!\n");
        // std::cerr << "X: " << heuristicMove->getX() << " Y: " << heuristicMove->getY() << std::endl;
        /* If not, default to legal best 1-ply move */
        heuristicMove = simpleHeuristic();
    }

    /* Update board with our move */
    gameBoard->doMove(heuristicMove, us);

    /* We made a valid move, took 1/60 squares */
    if (heuristicMove != NULL) {
        turnCount += 1;
    }

    return heuristicMove;
}
