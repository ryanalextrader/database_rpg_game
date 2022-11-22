#ifndef PLAYER_H
#define PLAYER_H

#include ".\character.h"
#include ".\consumable.h"
#include <cmath>
#include <string>
#include <list>
using std::list;
using std::string;
using std::sqrt;

class Player : public Character{
private:
    string weapon_name;
    string weapon_class; // melee or range

    list<Consumable> inventory;

    int consume_str;
    int consume_move;
    int consume_dur;

public:
    Player();
    Player(int row_coord, int col_coord, string weapon_n, string weapon_c, int spd, int max_health, int range, int damage, int damage_var, float accuracy, float accuracy_decay);

    bool updateCoords(int row_coord, int col_coord);
    bool levelUp(string stat, int amount);

    bool consume(string item);
    int consumeTimer();

    void consumeAgain();

    string getWeaponName() const;
    string getWeaponClass() const;
    string getConsumeEffects() const;
};

#endif