#ifndef MONSTER_H
#define MONSTER_H

#include ".\player.h"
#include <cmath>
#include <string>
using std::string;
using std::sqrt;

class Monster{
private:
    string name;
    int max_hp;
    int cur_hp;
    string desc;

    int atk_range;
    int atk;
    int atk_var;

    float acc;
    float acc_rate;

    char token;
    int col;
    int row;
    int move;
    int sight; //"sight" range
    bool provoked;
    int behave[3]; //element meanings: 0 == random, 1 == approach player, 2 == flee player
    int b_index; //index meanings: 0 == provoked, 1 == default, 2 == cannot see player

    int dest[2]; //col, row

    int getDistS(int col_coord, int row_coord) const; //distance from monster, squared
    int distanceS(int col_a, int row_a, int col_b, int row_b) const; //distance between point a and point b
public:
    Monster();
    Monster(int col_coord, int row_coord, char symbol, int behaveP, int behaveD, int behaveB);

    void setCoords(int col_coord, int row_coord); //puts monster at [col_coord, row_coord] !!ONLY CALL AFTER checkOverlap IN MAP!!
    void updateCoords(); //moves monster to dest
    bool canMove(int col_coord, int row_coord) const;
    bool canApproach(Player plr) const;
    bool canSee(Player plr);

    bool canAttack(int target_row, int target_col) const;
    int rollAttack(int target_row, int target_col) const;

    void setDest(Player plr, int max_cols, int max_rows);

    void adjMove(Player plr, int max_cols, int max_rows); //move adjacent to the player
    void chaseMove(Player plr, int max_col, int max_row); //move toward the player
    void fleeMove(Player plr, int max_cols, int max_rows); //move away from the player
    void randMove(Player plr, int max_cols, int max_rows); //move within range, not onto the player

    int getCol() const;
    int getRow() const;
    char getToken() const;
};

#endif