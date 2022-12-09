#ifndef PLAYER_H
#define PLAYER_H

#include ".\character.h"
#include ".\consumable.h"
#include <cmath>
#include <string>
#include <list>
#include <vector>
using std::list;
using std::string;
using std::sqrt;
using std::vector;

const int INVENTORY_CAP = 9;

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
    Player(int row_coord, int col_coord, string weapon_n, string weapon_c, int spd, int max_health, int range, int damage, int damage_var, float accuracy, float accuracy_decay, int strength);

    bool updateCoords(int row_coord, int col_coord);
    bool levelUp(string stat, int amount);

    //false if index is not in inventory
    bool consume(int index);
    int consumeTimer();

    //returns false if inventory is full
    bool addItem(Consumable item);

    //n_item: the new item to place in the inventory
    //index: integer in the range [1,9], corresponding to the index in getInventoryList()
    //returns false if index is not in inventory
    bool replaceItem(Consumable n_item, int index);

    //reset consumable effects in preparation of consuming a new item
    void consumeAgain();

    void tradeWeapons(string wep_name, string wep_class, int wep_atk, int wep_atk_range, int wep_atk_var, float wep_acc, float wep_acc_decay);

    string getWeaponName() const;
    string getWeaponClass() const;
    vector<string> getConsumeEffects() const;
    string getInventoryList();
    int getItemId(int index);
    int getInvSize() const;
    bool isBuffed() const;

    int setCurHP(int val);
};

#endif