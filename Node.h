#ifndef __NODE_H__
#define __NODE_H__

#include <vector>
#include "common.h"

using namespace std;

class Node {
public:
    int score;
    Move singlemove;
    Node () {

    }
    Node (int s) {
        score = s;
    }

    // Getters
    int getScore() {
        return score;
    }
    Move getSingleMove() {
        return singlemove;
    }


    // Setters
    void setScore(int s) {
        score = s;
    }
    void setSingleMove(Move m) {
        singlemove.setX(m.getX());
        singlemove.setY(m.getY());
    }

};

#endif