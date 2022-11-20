#ifndef PLAYER_H
#define PLAYER_H

class Player{
private:
    char token;
    int x;
    int y;
    int move;

public:
    Player();
    Player(int x_coord, int y_coord);

    void updateCoords(int x_coord, int y_coord);
    bool canMove(int x_coord, int y_coord) const;

    int getX() const;
    int getY() const;
    char getToken() const;
};

#endif