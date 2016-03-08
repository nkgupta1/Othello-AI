#ifndef __COMMON_H__
#define __COMMON_H__

#define switchSide(s)    (s == BLACK) ? WHITE : BLACK
#define printVectorMoves(v) \
   for (unsigned int i = 0; i < v.size(); i++) { \
       fprintf(stderr, "X: %d Y: %d\n", v[i].getX(), v[i].getY()); \
   }
#define printBoard(b) \
   { \
   fprintf(stderr, "-------------------------\n"); \
   for (int i = 0; i < 64; i++) { \
        fprintf(stderr, "%c ", b->getPiece(i)); \
        if (i % 8 == 7) {fprintf(stderr, "\n");} \
   } \
   fprintf(stderr, "-------------------------\n"); \
   }
#define printMove(m) \
   fprintf(stderr, "X: %d Y: %d\n", m->getX(), m->getY());

enum Side { 
    WHITE, BLACK
};

class Move {
   
public:
    int x, y;
    Move(int x, int y) {
        this->x = x;
        this->y = y;        
    }
    ~Move() {}

    int getX() { return x; }
    int getY() { return y; }

    void setX(int x) { this->x = x; }
    void setY(int y) { this->y = y; }
};

#endif
