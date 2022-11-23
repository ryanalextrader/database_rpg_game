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

    // int setStr(int value);
    // int setMove(int value);
    // int setMaxHp(int value);

    // //positive number
    // int setAtkRange(int value);
    // //positive number
    // int setAtk(int value);
    // //positive number, less than atk
    // int setAtkVar(int value);
    // //0.5 -> 1.0
    // float setAcc(float value);
    // //0.0 - > 0.25, less than acc
    // float setAccDecay(float value);
public:
    Character();
    Character(char symbol, int row_coord, int col_coord, int spd, int max_health, int range, int damage, int damage_var, float accuracy, float accuracy_decay);

    void setCoords(int row_coord, int col_coord);
    bool canMove(int row_coord, int col_coord) const;

    bool canAttack(int target_row, int target_col) const;
    int rollAttack(int target_row, int target_col) const; //returns -1 if the attack misses, NEED TO ACCOUNT FOR STRENGTH!!!
    int receiveAttack(int dmg); //returns cur_hp *after* taking damage

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