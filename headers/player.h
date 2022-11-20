#ifndef PLAYER_H
#define PLAYER_H

#include ".\character.h"
#include <cmath>
using std::sqrt;

class Player : public Character{
private:
//wooooo
public:
    Player();
    Player(int row_coord, int col_coord);

    bool updateCoords(int row_coord, int col_coord);
};

#endif