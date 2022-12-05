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

    int getDistS(int col_coord, int row_coord) const; //distance from monster, squared
    int distanceS(int col_a, int row_a, int col_b, int row_b) const; //distance between point a and point b, squared

    void adjMove(Player plr, int max_cols, int max_rows); //move adjacent to the player
    void chaseMove(Player plr, int max_col, int max_row); //move toward the player
    void fleeMove(Player plr, int max_cols, int max_rows); //move away from the player
    void randMove(Player plr, int max_cols, int max_rows); //move within range
public:
    Monster();
    //behave values: 0 == random, 1 == approach, 2 == flee
    Monster(string monst_name, string monst_desc, int col_coord, int row_coord, char symbol, int sight_range, int behaveP, int behaveS, int behaveD, int spd, int max_health, int range, int damage, int damage_var, float accuracy, float accuracy_decay);

    void updateCoords(); //moves monster to dest
    bool canApproach(Player plr) const;
    bool canSee(Player plr);
    bool isProvoked();

    int checkBehavior();

    void setDest(Player plr, int max_cols, int max_rows);

    //0 == random, 1 == approach player, 2 == flee player
    int getCurBehavior() const;
    char getToken() const;
    string getName() const;
    string getDesc() const;
};

#endif