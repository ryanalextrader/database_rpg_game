#include "..\headers\player.h"

Player::Player() : Player(0,0) {}

Player::Player(int row_coord, int col_coord) {
    row = row_coord;
    col = col_coord;
    move = 4;
    token = '@';
    atk_range = 3;
    acc = 1;
    acc_rate = .25;
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

    if((drow*drow + dcol*dcol) <= move*move) {
        return true;
    }
    return false;
}

bool Player::canAttack(int target_row, int target_col) const {
    int drow = target_row - row;
    int dcol = target_col - col;

    if((drow*drow + dcol*dcol) <= atk_range*atk_range) {
        return true;
    }
    return false;
}

int Player::rollAttack(int target_row, int target_col) const {
    int drow = target_row - row;
    int dcol = target_col - col;
    
//roll accuracy
    //calculate distance to target
    int dist = sqrt(drow*drow + dcol*dcol);
    //distance * acc_rate = acc_loss
    //full accuracy within 1 tile, dist - 1
    float acc_loss = (dist - 1) * acc_rate;
        
    //1 - (acc - acc_loss) = chance_to_miss
    //num % 100 > chance_to_miss -> hit
    int roll = 1 + rand() % 100;
    if(roll < 100 * (1 - acc + acc_loss)) {
        return -1;
    }
    
//roll damage
    //damage mod: between -atk_var and atk_var
    roll = (rand() % ((2 * atk_var) + 1)) - atk_var;
    
    return atk + roll;
}

int Player::receiveAttack(int dmg) {
    cur_hp = cur_hp - dmg;
    return cur_hp;
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