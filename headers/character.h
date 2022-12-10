#ifndef CHARACTER_H
#define CHARACTER_H

#include <cmath>
using std::sqrt;

class Character {
protected:
    //map data
    char token;
    int row;
    int col;

    //character stats
    int move;
    int max_hp;
    int cur_hp;
    int str;
    //try and find a 4th player stat?
    bool dead; //whether or not the character has died

    //weapon stats (for player)
    int atk_range;
    int atk; //average damage value
    int atk_var; //variance in attack damage: damage range == [atk - atk_var, atk+atk_var]
    float acc; //1.0 -> always hits, 0.9 -> 90% chance to hit...
    float acc_rate; //how much the accuracy decreases every unit of distance

public:
    Character();
    Character(char symbol, int row_coord, int col_coord, int spd, int max_health, int range, int damage, int damage_var, float accuracy, float accuracy_decay, int strength);

    //change the coordinates to the given point
    void setCoords(int row_coord, int col_coord);
    //return true if the given point is less distance from the character than move
    bool canMove(int row_coord, int col_coord) const;

    //return true if the given point is less distance from the character than atk_range
    bool canAttack(int target_row, int target_col) const;
    
    //roll an attack targeted at whatever is at a given point
    //generate a number for damage using strength, atk, and atk_var
    //checks additionally if the attack would hit, and returns -1 on a miss
    int rollAttack(int target_row, int target_col) const;
    //subtracts the input damage from cur_hp and returns the new value of cur_hp
    //if dmg is negative, does not change cur_hp
    int receiveAttack(int dmg);

    int getRow() const;
    int getCol() const;
    virtual char getToken() const;
    bool isDead() const;

    int getMaxHp() const;
    int getCurHp() const;
    int getAtk() const;
    int getAtkRange() const;
    int getAtkVar() const;
    float getAcc() const;
    float getAccRate() const;
    int getMove() const;
    int getStr() const;
};

#endif