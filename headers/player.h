#ifndef PLAYER_H
#define PLAYER_H

class Player{
private:
    char token;
    int row;
    int col;
    int move;

public:
    Player();
    Player(int row_coord, int col_coord);

    bool updateCoords(int row_coord, int col_coord);
    bool canMove(int row_coord, int col_coord) const;

    int getRow() const;
    int getCol() const;
    char getToken() const;
};

#endif