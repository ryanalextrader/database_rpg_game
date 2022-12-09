#include "..\headers\player.h"
#include <string>
using std::string;

Player::Player() : Player(0, 0, "Great Sword", "melee", 4, 50, 3, 5, 2, 1.0, 0, 3) {}

Player::Player(int row_coord, int col_coord, string weapon_n, string weapon_c, 
    int spd, int max_health, int range, int damage, int damage_var, float accuracy, 
    float accuracy_decay, int strength) : Character('@', row_coord, col_coord, spd, max_health, 
    range, damage, damage_var, accuracy, accuracy_decay, strength) {
    weapon_name = weapon_n;
    weapon_class = weapon_c;

    consume_dur = 0;
    consume_move = 0;
    consume_str = 0;
}

bool Player::updateCoords(int row_coord, int col_coord) {
    if(!canMove(row_coord, col_coord)) {
        return false;
    }
    
    setCoords(row_coord, col_coord);
    return true;
}

bool Player::levelUp(string stat, int amount){
    if(amount > 0){
        if(stat == "speed"){
            move += amount;
        }
        else if(stat == "max_hp"){
            max_hp += amount;
            cur_hp += amount;
        }
        else if(stat == "strength"){
            str += amount;
        }
        else{
            return false;
        }
        return true;
    }
    return false;
}

bool Player::consume(int index) {
    if(index > inventory.size()) {
        return false;
    }
    

    //look through the inventory
    list<Consumable>::iterator inv_cursor = inventory.begin();
    for(int i = 1; i < index; i++) {
        inv_cursor++;
    }

    //reset consumable buffs iff you have a buff and the potion you are consuming also has a buff
    if(consume_dur > 0 && inv_cursor->getDur() > 0) {
        consumeAgain();
    }
    //consume the item
    cur_hp += inv_cursor->getHeal();
    if(cur_hp > max_hp) {
        cur_hp = max_hp;
    }
    consume_str = inv_cursor->getStrB();
    consume_move = inv_cursor->getMoveB();
    consume_dur = inv_cursor->getDur();

    inventory.erase(inv_cursor);

    str += consume_str;
    move += consume_move;
    return true;
}

int Player::consumeTimer() {
    if(consume_dur == 0) {
        str -= consume_str;
        move -= consume_move;
        consume_str = 0;
        consume_move = 0;
    } else if(consume_dur == 1) {
        consume_dur--;
        str -= consume_str;
        move -= consume_move;
        consume_str = 0;
        consume_move = 0;
    } else {
        consume_dur--;
    }
    return consume_dur;
}

bool Player::addItem(Consumable item) {
    if(inventory.size() < INVENTORY_CAP) {
        inventory.push_back(item);
        return true;
    }
    return false;
}

bool Player::replaceItem(Consumable n_item, int index) {
    if(index > inventory.size()) {
        return false;
    }

    list<Consumable>::iterator inv_crsr = inventory.begin();
    
    //iterate to the item to be replaced
    for(int i = 1; i < index; i++) {
        inv_crsr++;
    }

    inv_crsr = inventory.insert(inv_crsr, n_item);
    inv_crsr++;
    inventory.erase(inv_crsr);
    
    return true;
}

void Player::consumeAgain() {
    str -= consume_str;
    move -= consume_move;
}

void Player::tradeWeapons(string wep_name, string wep_class, int wep_atk, int wep_atk_range, int wep_atk_var, float wep_acc, float wep_acc_decay){
    weapon_name = wep_name;
    weapon_class = wep_class;
    atk = wep_atk;
    atk_range = wep_atk_range;
    atk_var = wep_atk_var;
    acc = wep_acc;
    acc_rate = wep_acc_decay;
}

string Player::getWeaponName() const{
    return weapon_name;
}

string Player::getWeaponClass() const{
    return weapon_class;
}

vector<string> Player::getConsumeEffects() const {
    vector<string> effects;
    if(consume_dur > 0) {
        if(consume_str != 0) {
            if(consume_str < 0) {
                effects.push_back(std::to_string(consume_str) + " str");
            } else {
                effects.push_back("+" + std::to_string(consume_str) + " str");
            }
        } else {
            effects.push_back(" ");
        }

        if(consume_move != 0) {
            if(consume_move < 0) {
                effects.push_back(std::to_string(consume_move) + " move");
            } else {
                effects.push_back("+" + std::to_string(consume_move) + " move");
            }
        } else {
            effects.push_back(" ");
        }
        
        effects.push_back(std::to_string(consume_dur) + " turns left");
    } else {
        effects.push_back("NO BUFFS");
    }
    
    return effects;
}

string Player::getInventoryList() {
    if(inventory.empty()) {
        return "No items in inventory!";
    }

    string inv_list;
    list<Consumable>::iterator inv_crsr = inventory.begin();
    int num_items = 1;

    while(inv_crsr != inventory.end()) {
        inv_list += "[" + std::to_string(num_items) + "]" + inv_crsr->getName() + ": " + inv_crsr->getDesc() + "\n    ";
        if(inv_crsr->getHeal() != 0)
            inv_list += std::to_string(inv_crsr->getHeal()) + " hp";
        if(inv_crsr->getDur() != 0) {
            if(inv_crsr->getHeal() != 0)
                inv_list += ", ";
            if(inv_crsr->getStrB() != 0)
                inv_list += std::to_string(inv_crsr->getStrB()) + " str, ";
            if(inv_crsr->getMoveB() != 0)
                inv_list += std::to_string(inv_crsr->getMoveB()) + " move, ";

            inv_list += std::to_string(inv_crsr->getDur()) + " dur";
        }
        inv_list += "\n";
        inv_crsr++;
        num_items++;
    }

    // inv_list += "[" + std::to_string(num_items) + "]" + inv_crsr->getName() + ": " + inv_crsr->getDesc() + "\n    ";
    // if(inv_crsr->getHeal() != 0)
    //     inv_list += "heal: " + inv_crsr->getHeal();
    // if(inv_crsr->getDur() != 0) {
    //     if(inv_crsr->getHeal() != 0)
    //         inv_list += ", ";
    //     if(inv_crsr->getStrB() != 0)
    //         inv_list += "str: " + std::to_string(inv_crsr->getStrB()) + ", ";
    //     if(inv_crsr->getMoveB() != 0)
    //         inv_list += "move: " + std::to_string(inv_crsr->getMoveB()) + ", ";

    //     inv_list += "dur: " + inv_crsr->getDur();
    // }

//inv_list appearance: (these are examples, and not indicative of real items in the game)
/*
[1] steak: a juicy, magical slab of meat
    heal: 5, str: 5, move: 1, dur: 5
[2] health potion: a simple healing tincture
    heal: 10
[3] slippery goop: I don't know how this works...
    move: 2, dur: 4
[4] performance enhancing drugs: unimaginable power at the price of your hair
    str: 6, move: 2, dur 7
*/

    return inv_list;
}

int Player::getItemId(int index) {
    if(index > inventory.size() || index < 1) {
        return -1;
    }

    list<Consumable>::iterator inv_crsr = inventory.begin();
    for(int i = 1; i < index; i++) {
        inv_crsr++;
    }
    return inv_crsr->getId();
}

int Player::getInvSize() const {
    return inventory.size();
}

bool Player::isBuffed() const {
    if(consume_dur == 0) {
        return false;
    }
    return true;
}

void Player::floorHeal() {
    cur_hp += 2;
    if(cur_hp > max_hp) {
        cur_hp = max_hp;
    }
}

int Player::setCurHP(int val) {
    cur_hp = val;
    return cur_hp;
}