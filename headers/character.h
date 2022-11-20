#ifndef CHARACTER_H
#define CHARACTER_H

#include <cmath>
using std::sqrt;

class Character {
protected:
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

    bool dead; //whether or not the character has died

public:
    Character();
    Character(char symbol, int row_coord, int col_coord, int spd, int max_health, int range, int damage, int damage_var, float accuracy, float accuracy_decay);

    void setCoords(int row_coord, int col_coord);
    bool canMove(int row_coord, int col_coord) const;

    bool canAttack(int target_row, int target_col) const;
    int rollAttack(int target_row, int target_col) const; //returns -1 if the attack misses
    int receiveAttack(int dmg); //returns cur_hp *after* taking damage

    int getRow() const;
    int getCol() const;
    virtual char getToken() const;
    bool isDead() const;
};

#endif