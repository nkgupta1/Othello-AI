#ifndef __NODE_H__
#define __NODE_H__

#include <vector>
#include "common.h"

using namespace std;

class Node {
public:
    int score;
    Move singlemove;
    vector<Move> moves;
    vector<Node> children;
    Node () {

    }
    Node (int s) {
        score = s;
    }
    Node (int s, Move *m) {
        score = s;
        moves.push_back(*m);
    }

    // Getters
    int getScore() {
        return score;
    }
    vector<Move> getMoves() {
        return moves;
    }
    vector<Node> getChildren() {
        return children;
    }
    Move getSingleMove() {
        return singlemove;
    }


    // Setters
    void setScore(int s) {
        score = s;
    }
    void setMoves(vector<Move> m) {
        moves = m;
    }
    void setChildren(vector<Node> c) {
        children = c;
    }
    void setSingleMove(Move m) {
        singlemove.setX(m.getX());
        singlemove.setY(m.getY());
    }

};

#endif
