#include "..\headers\player.h"

Player::Player() : Player(0,0) {}

Player::Player(int row_coord, int col_coord) {
    row = row_coord;
    col = col_coord;
    move = 4;
    token = '@';
}

bool Player::updateCoords(int row_coord, int col_coord) {
    if(!canMove(row_coord, col_coord)) {
        return false;
    }
    row = row_coord;
    col = col_coord;
    return true;
}

bool Player::canMove(int row_coord, int col_coord) const {
    int drow = row_coord - row;
    int dcol = col_coord - col;

    if((drow*drow + dcol* dcol) <= move*move) {
        return true;
    }
    return false;
}

char Player::getToken() const {
    return token;
}

int Player::getRow() const {
    return row;
}

int Player::getCol() const {
    return col;
}