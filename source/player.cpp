#include "..\headers\player.h"
#include <string>
using std::string;

Player::Player() : Player(0,0, "Great Sword", "melee", 4, 50, 3, 5, 2, 1.0, 0) {}

Player::Player(int row_coord, int col_coord, string weapon_n, string weapon_c, 
    int spd, int max_health, int range, int damage, int damage_var, float accuracy, 
    float accuracy_decay) : Character('@', row_coord, col_coord, spd, max_health, 
    range, damage, damage_var, accuracy, accuracy_decay) {
    weapon_name = weapon_n;
    weapon_class = weapon_c;
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
        }
        else if(stat == "attack"){
            atk += amount;
        }
        else{
            return false;
        }
        return true;
    }
    return false;
}

bool Player::consume(string item) {
    //reset consumable buffs
    if(consume_dur > 0) {
        consumeAgain();
    }

    //look through the inventory
    list<Consumable>::iterator inv_cursor = inventory.begin();
    while(inv_cursor->getName() != item && inv_cursor != inventory.end()) {
        inv_cursor++;
    }

    //return false if the item is not found
    if(inv_cursor->getName() != item) {
        return false;
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
    if(consume_dur > 0) {
        consume_dur--;
    } else {
        str -= consume_str;
        move -= consume_move;
        consume_str = 0;
        consume_move = 0;
    }
    return consume_dur;
}

void Player::consumeAgain() {
    str -= consume_str;
    move -= consume_move;
}

string Player::getWeaponName() const{
    return weapon_name;
}

string Player::getWeaponClass() const{
    return weapon_class;
}

string Player::getConsumeEffects() const {
    string effects = std::to_string(consume_dur) + " t";
    if(consume_str != 0) {
        if(consume_str < 0) {
            effects += ", " + std::to_string(consume_str) + " str";
        } else {
            effects += ", +" + std::to_string(consume_str) + " str";
        }
    }
    if(consume_move != 0) {
        if(consume_move < 0) {
            effects += ", " + std::to_string(consume_move) + " move";
        } else {
            effects += ", +" + std::to_string(consume_move) + " move";
        }
    }
    
    return effects;
}