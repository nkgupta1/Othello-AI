#include "board.h"

#define XY(i, j) (i + 8 * j)

/*
 * Make a standard 8x8 othello board and initialize it to the standard setup.
 */
Board::Board() {
    taken.set(3 + 8 * 3);
    taken.set(3 + 8 * 4);
    taken.set(4 + 8 * 3);
    taken.set(4 + 8 * 4);
    black.set(4 + 8 * 3);
    black.set(3 + 8 * 4);
}

/*
 * Destructor for the board.
 */
Board::~Board() {
}

/*
 * Returns a copy of this board.
 */
Board *Board::copy() {
    Board *newBoard = new Board();
    newBoard->black = black;
    newBoard->taken = taken;
    return newBoard;
}

bool Board::occupied(int x, int y) {
    return taken[x + 8*y];
}

bool Board::get(Side2 side, int x, int y) {
    return occupied(x, y) && isEqual(black[x + 8*y], isBlack(side));
}

void Board::set(Side2 side, int x, int y) {
    taken.set(x + 8*y);
    black.set(x + 8*y, isBlack(side));
}

bool Board::onBoard(int x, int y) {
    return(0 <= x && x < 8 && 0 <= y && y < 8);
}

 
/*
 * Returns true if the game is finished; false otherwise. The game is finished 
 * if neither side has a legal move.
 */
bool Board::isDone() {
    return !(hasMoves(BLACK) || hasMoves(WHITE));
}

/*
 * Returns true if there are legal moves for the given side.
 */
bool Board::hasMoves(Side2 side) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Move move(i, j);
            if (checkMove(&move, side)) return true;
        }
    }
    return false;
}

/*
 * Returns true if a move is legal for the given side; false otherwise.
 */
bool Board::checkMove(Move *m, Side2 side) {
    // Passing is only legal if you have no moves.
    if (m == NULL) return !hasMoves(side);

    int X = m->getX();
    int Y = m->getY();

    // Make sure the square hasn't already been taken.
    if (occupied(X, Y)) return false;

    Side2 other = switchSide2(side);
    int x = X-1;
    int y = Y;
    bool flipped = 0;
    // check left
    while (x > 0 && get(other, x, y)) {
        x--;
        flipped = 1;
    } 
    if (get(side, x, y) && flipped) return true;
    // check right
    x = X+1;
    y = Y;
    flipped = 0;
    while (x < 7 && get(other, x, y)) {
        x++;
        flipped = 1;
    } 
    if (get(side, x, y) && flipped) return true;
    // check up
    x = X;
    y = Y-1;
    flipped = 0;
    while (y > 0 && get(other, x, y)) {
        y--;
        flipped = 1;
    } 
    if (get(side, x, y) && flipped) return true;
    // check down
    x = X;
    y = Y+1;
    flipped = 0;
    while (y < 7 && get(other, x, y)) {
        y++;
        flipped = 1;
    } 
    if (get(side, x, y) && flipped) return true;
    // check diagonal up left
    x = X-1;
    y = Y-1;
    flipped = 0;
    while (x > 0 && y > 0 && get(other, x, y)) {
        x--;
        y--;
        flipped = 1;
    } 
    if (get(side, x, y) && flipped) return true;
    // check diagonal up right
    x = X+1;
    y = Y-1;
    flipped = 0;
    while (x < 7 && y > 0 && get(other, x, y)) {
        x++;
        y--;
        flipped = 1;
    } 
    if (get(side, x, y) && flipped) return true;
    // check diagonal down left
    x = X-1;
    y = Y+1;
    flipped = 0;
    while (x > 0 && y < 7 && get(other, x, y)) {
        x--;
        y++;
        flipped = 1;
    } 
    if (get(side, x, y) && flipped) return true;
    // check diagonal down right
    x = X+1;
    y = Y+1;
    flipped = 0;
    while (x < 7 && y < 7 && get(other, x, y)) {
        x++;
        y++;
        flipped = 1;
    }
    if (get(side, x, y) && flipped) return true;

    return false;
}

/*
 * Returns true if a move is legal for the given side; false otherwise.
 */
bool Board::checkMoveOrg(Move *m, Side side) {
    // Passing is only legal if you have no moves.
    if (m == NULL) return !hasMoves(side);

    int X = m->getX();
    int Y = m->getY();

    // Make sure the square hasn't already been taken.
    if (occupied(X, Y)) return false;

    Side other = switchSide(side);
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dy == 0 && dx == 0) continue;

            // Is there a capture in that direction?
            int x = X + dx;
            int y = Y + dy;
            if (onBoard(x, y) && get(other, x, y)) {
                do {
                    x += dx;
                    y += dy;
                } while (onBoard(x, y) && get(other, x, y));

                if (onBoard(x, y) && get(side, x, y)) return true;
            }
        }
    }
    return false;
}

/*
 * Modifies the board to reflect the specified move.
 */
vector<Move> Board::doMove(Move *m, Side2 side) {
    vector<Move> res;    
    // A NULL move means pass.
    if (m == NULL) return res;

    // Ignore if move is invalid.
    if (!checkMove(m, side)) return res;

    /* Actual move gets deleted for undo; remainder get flipped */
    Move curr(m->getX(), m->getY());
    res.push_back(curr);


    int X = m->getX();
    int Y = m->getY();
    Side2 other = switchSide2(side);
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dy == 0 && dx == 0) continue;

            int x = X;
            int y = Y;
            do {
                x += dx;
                y += dy;
            } while (onBoard(x, y) && get(other, x, y));

            if (onBoard(x, y) && get(side, x, y)) {
                x = X;
                y = Y;
                x += dx;
                y += dy;
                while (onBoard(x, y) && get(other, x, y)) {
                    set(side, x, y);
                    /* All changes get flipped in undo */
                    int x2 = x;
                    int y2 = y;
                    Move curr2(x2, y2);
                    res.push_back(curr2);
                    x += dx;
                    y += dy;
                }
            }
        }
    }
    set(side, X, Y);
    return res;
}

void Board::undoMove(vector<Move> moves, Side2 side) {
    if (moves.size() == 0)
    {
        return;
    }
    
    taken[XY(moves[0].getX(), moves[0].getY())] = 0;
    black[XY(moves[0].getX(), moves[0].getY())] = 0;

    if (moves.size() == 1)
    {
        return;
    }

    Side2 flipside = switchSide2(side);
    for (unsigned int i = 1; i < moves.size(); i++)
    {
        set(flipside, moves[i].getX(), moves[i].getY());
    }
}

/*
 * Current count of given side's stones.
 */
int Board::count(Side2 side) {
    return (isBlack(side))? countBlack() : countWhite();
}

/*
 * Current count of black stones.
 */
int Board::countBlack() {
    return black.count();
}

/*
 * Current count of white stones.
 */
int Board::countWhite() {
    return taken.count() - black.count();
}

/*
 * Sets the board state given an 8x8 char array where 'w' indicates a white
 * piece and 'b' indicates a black piece. Mainly for testing purposes.
 */
void Board::setBoard(char data[]) {
    taken.reset();
    black.reset();
    for (int i = 0; i < 64; i++) {
        if (data[i] == 'b') {
            taken.set(i);
            black.set(i);
        } if (data[i] == 'w') {
            taken.set(i);
        }
    }
}

char Board::getPiece(int i) {
    if (!taken[i] & 1) return ' ';
    if (black[i] & 1) return 'b';
    else return 'w';
}

bitset<128> Board::naiveHash() {
    bitset<128> toReturn;
    for (int i = 0; i < 64; i++) {
        toReturn.set(2*i  , taken[i]);
        toReturn.set(2*i+1, black[i]);
    }
    return toReturn;
}

string Board::toHashString() {
    bitset<128> toString;
    for (int i = 0; i < 64; i++) {
        toString.set((2*i)%128  , taken[i]);
        toString.set((2*i+1)%128, black[i]);
    }
    string s;
    char c = 0;
    for (int i = 0; i < 128; i++) {
        c += (int)(toString[i]);
        c = c << 1;
        if (i % 8 == 7) {
            s += c;
            c = 0;
        }
    }

    return s;
}