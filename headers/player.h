#ifndef PLAYER_H
#define PLAYER_H

#include ".\character.h"
#include <cmath>
#include <string>
using std::string;
using std::sqrt;

class Player : public Character{
private:
    string weapon_name;
    string weapon_class; // melee or range
public:
    Player();
    Player(int row_coord, int col_coord, string weapon_n, string weapon_c);

    bool updateCoords(int row_coord, int col_coord);

    string getWeaponName() const;
    string getWeaponClass() const;
};

#endif