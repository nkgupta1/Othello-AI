#ifndef __NODE_H__
#define __NODE_H__

#include <vector>
#include "common.h"

using namespace std;

class Node {
public:
    int score;
    vector<Move> moves;
    vector<Node> children;
    Node (int s, Move *m) {
        score = s;
        lastMove = m;
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

};

#endif
