#include "..\headers\player.h"

Player::Player() : Player(0,0) {}

Player::Player(int x_coord, int y_coord) {
    x = x_coord;
    y = y_coord;
    move = 4;
    token = '@';
}

void Player::updateCoords(int x_coord, int y_coord) {
    if(!canMove(x_coord, y_coord)) {
        return;
    }
    x = x_coord;
    y = y_coord;
}

bool Player::canMove(int x_coord, int y_coord) const {
    int dx = x_coord - x;
    int dy = y_coord - y;

    if((dx*dx + dy* dy) <= move*move) {
        return true;
    }
    return false;
}

char Player::getToken() const {
    return token;
}

int Player::getX() const {
    return x;
}

int Player::getY() const {
    return y;
}