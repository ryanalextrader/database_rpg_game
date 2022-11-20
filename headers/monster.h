#ifndef MONSTER_H
#define MONSTER_H

#include ".\player.h"
#include <cmath>
using std::sqrt;

class Monster{
private:
    char token;
    int x;
    int y;
    int move;
    int sight; //"sight" range
    bool provoked;
    int behave[3]; //element meanings: 0 == random, 1 == approach player, 2 == flee player
    int b_index; //index meanings: 0 == provoked, 1 == default, 2 == cannot see player

    int dest[2]; //tile the monster wants to move to

    int getDist(int x_coord, int y_coord) const; //check how far a given point is from the monster

public:
    Monster();
    Monster(int x_coord, int y_coord, char symbol, int behaveP, int behaveD, int behaveB);

    void setCoords(int x_coord, int y_coord); //puts monster at [x_coord, y_coord] !!ONLY CALL AFTER checkOverlap IN MAP!!
    void updateCoords(); //moves monster to dest
    bool canMove(int x_coord, int y_coord) const;
    bool canApproach(Player plr) const;
    bool canSee(Player plr);

    void setDest(Player plr);

    int getX() const;
    int getY() const;
    char getToken() const;
};

#endif