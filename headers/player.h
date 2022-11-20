#ifndef PLAYER_H
#define PLAYER_H

#include <cmath>
using std::sqrt;

class Player{
private:
    char token;
    int row;
    int col;
    int move;

    int max_hp;
    int cur_hp;
    int atk_range;
    int atk; //average damage value
    int atk_var; //variance in attack damage: damage range == [atk - atk_var, atk+atk_var]
    float acc; //1.0 -> always hits, 0.9 -> 90% chance to hit...
    float acc_rate; //how much the accuracy decreases every unit of distance

public:
    Player();
    Player(int row_coord, int col_coord);

    bool updateCoords(int row_coord, int col_coord);
    bool canMove(int row_coord, int col_coord) const;

    bool canAttack(int target_row, int target_col) const;
    int rollAttack(int target_row, int target_col) const; //returns -1 if the attack misses
    int receiveAttack(int dmg); //returns cur_hp *after* taking damage

    int getRow() const;
    int getCol() const;
    char getToken() const;
};

#endif