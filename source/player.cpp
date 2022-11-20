#include "..\headers\player.h"

Player::Player() : Player(0,0) {}

Player::Player(int row_coord, int col_coord) : Character('@', row_coord, col_coord, 4, 20, 3, 5, 2, 1.0, 0) {
    // row = row_coord;
    // col = col_coord;
    // move = 4;
    // token = '@';
    // atk_range = 3;
    // acc = 1;
    // acc_rate = .25;
}

bool Player::updateCoords(int row_coord, int col_coord) {
    if(!canMove(row_coord, col_coord)) {
        return false;
    }
    
    setCoords(row_coord, col_coord);
    return true;
}