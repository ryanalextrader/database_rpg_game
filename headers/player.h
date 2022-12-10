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

    //check if the player is able to move to a given location, then moves there
    //returns true if the move was successful (the given point was within the player's move range)
    bool updateCoords(int row_coord, int col_coord);
    //increases a stat (hp, str, move)
    bool levelUp(string stat, int amount);

    //applies the effects of the item at index to the player
    //returns false if index is not in inventory
    bool consume(int index);
    //ticks down consume duration and resets stat changes when it reaches 0
    int consumeTimer();

    //puts the input item into inventory
    //if inventory is full, return false and do not add the item
    bool addItem(Consumable item);

    //n_item: the new item to place in the inventory
    //index: integer in the range [1,9], corresponding to the index in getInventoryList()
    //returns false if index is not in inventory
    bool replaceItem(Consumable n_item, int index);

    //reset consumable effects in preparation of consuming a new item
    void consumeAgain();

    //set weapon traits (atk, atk_var, range, acc, acc_decay) to the new values
    void tradeWeapons(string wep_name, string wep_class, int wep_atk, int wep_atk_range, int wep_atk_var, float wep_acc, float wep_acc_decay);
    
    //increase cur_hp by 2 or to max_hp
    void floorHeal();

    string getWeaponName() const;
    string getWeaponClass() const;
    //returns a vector listing the effects of the current values of consume_str, consume_move, consume_dur
    vector<string> getConsumeEffects() const;
    //returns a string of all item stat blocks in the player's inventory
    string getInventoryList();
    //returns the id of the consumable at the given index in the inventory list
    int getItemId(int index);
    int getInvSize() const;
    //whether or not the player is currently under the effects of a potion
    bool isBuffed() const;

    int setCurHP(int val);
};

#endif