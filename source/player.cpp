#include "..\headers\player.h"
#include <string>
using std::string;

Player::Player() : Player(0,0, "Great Sword", "melee") {}

Player::Player(int row_coord, int col_coord, string weapon_n, string weapon_c) : Character('@', row_coord, col_coord, 4, 50, 3, 5, 2, 1.0, 0) {
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

string Player::getWeaponName() const{
    return weapon_name;
}

string Player::getWeaponClass() const{
    return weapon_class;
}