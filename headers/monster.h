#ifndef MONSTER_H
#define MONSTER_H

#include ".\player.h"
#include ".\character.h"
#include <cmath>
#include <string>
using std::string;
using std::sqrt;

class Monster : public Character{
private:
    string name;
    string desc;

    char dead_token;

    int sight; //"sight" range
    bool spotted; //whether or not the monster has seen the player
    bool provoked;
    int behave[3]; //element meanings: 0 == random, 1 == approach player, 2 == flee player
    int b_index; //index meanings: 0 == provoked, 1 == spotted, 2 == default

    int dest[2]; //col, row

    //square of distance between a given point and the monster
    int getDistS(int col_coord, int row_coord) const;
    //distance between two given points - a, b
    int distanceS(int col_a, int row_a, int col_b, int row_b) const; //distance between point a and point b, squared

    //monsters move by taking `move` "steps" in cardinal directions
    //move adjacent to the player
    void adjMove(Player plr, int max_cols, int max_rows);
    //move toward the player
    void chaseMove(Player plr, int max_cols, int max_rows);
    //move away from the player
    void fleeMove(Player plr, int max_cols, int max_rows);
    //move somewhere within move range
    void randMove(Player plr, int max_cols, int max_rows);
public:
    Monster();
    //behave values: 0 == random, 1 == approach, 2 == flee
    Monster(string monst_name, string monst_desc, int col_coord, int row_coord, char symbol, int sight_range, int behaveP, int behaveS, int behaveD, int spd, int max_health, int range, int damage, int damage_var, float accuracy, float accuracy_decay);

    //moves monster to dest
    void updateCoords();
    //whether or not the player is within move range of the monster
    bool canApproach(Player plr) const;
    //whether or not the player is within the sight range of the monster
    bool canSee(Player plr);
    //whether or not the monster has taken damage
    bool isProvoked();

    //checks the behavior flags (spotted, provoked) flags to set b_index
    int checkBehavior();

    //calls the appropriate move script according to the current behavior
    void setDest(Player plr, int max_cols, int max_rows);

    //0 == random, 1 == approach player, 2 == flee player
    int getCurBehavior() const;
    char getToken() const;
    string getName() const;
    string getDesc() const;
};

#endif