#include "..\headers\character.h"

Character::Character() : Character('x', 0, 0, 3, 10, 1, 5, 2, 1.0, 0.0, 0) {}

Character::Character(char symbol, int row_coord, int col_coord, int speed, int max_health, int range, int damage, int damage_var, float accuracy, float accuracy_decay, int strength) {
    token = symbol;
    row = row_coord;
    col = col_coord;
    move = speed;
    max_hp = max_health;
    cur_hp = max_hp;
    atk_range = range;
    atk = damage;
    atk_var = damage_var;
    acc = accuracy;
    acc_rate = accuracy_decay;
    str = strength;
}

void Character::setCoords(int row_coord, int col_coord) {
    row = row_coord;
    col = col_coord;
}
bool Character::canMove(int row_coord, int col_coord) const {
    int drow = row_coord - row;
    int dcol = col_coord - col;

    if((drow*drow + dcol*dcol) <= move*move) { //distance formula
        return true;
    }
    return false;
}

bool Character::canAttack(int target_row, int target_col) const {
    int drow = target_row - row;
    int dcol = target_col - col;

    if((drow*drow + dcol*dcol) <= atk_range*atk_range) { //distance formula
        return true;
    }
    return false;
}

int Character::rollAttack(int target_row, int target_col) const {
    int drow = target_row - row;
    int dcol = target_col - col;
    
    //roll accuracy
        //calculate distance to target
    int dist = sqrt(drow*drow + dcol*dcol);
        //distance * acc_rate = acc_loss
        //full accuracy within 1 tile -> dist - 1
    float acc_loss = (dist - 1) * acc_rate;
        
        //1 - (acc - acc_loss) = chance_to_miss
        //num % 100 > chance_to_miss -> hit
        //acc, acc_loss given as percent values, mult by 100
    int roll = 1 + rand() % 100;
    if(roll < 100 * (1 - acc + acc_loss)) {
        return -1; //-1 means miss
    }
    
    //roll damage
    //damage mod: between -atk_var and atk_var
    roll = (rand() % ((2 * atk_var) + 1)) - atk_var;
    
    return str + atk + roll;
}

int Character::receiveAttack(int dmg) {
    if(dmg < 0) { //negative damage denotes a miss, not a heal
        return -1 * cur_hp; //flag miss, but still return cur_hp
    }
    cur_hp = cur_hp - dmg;
    if(cur_hp <= 0) { //we died
        cur_hp = 0; //we don't want to accidentally flag a miss in the case the character dies
        dead = true;
    }
    return cur_hp; //return cur_hp
}

int Character::getRow() const{
    return row;
}    

int Character::getCol() const{
    return col;
}

char Character::getToken() const {
    return token;
}

bool Character::isDead() const {
    return dead;
}

int Character::getMaxHp() const {
    return max_hp;
}

int Character::getCurHp() const {
    return cur_hp;
}

int Character::getAtk() const {
    return atk;
}

int Character::getAtkRange() const {
    return atk_range;
}

int Character::getAtkVar() const{
    return atk_range;
}

float Character::getAcc() const{
    return acc;
}

float Character::getAccRate() const{
    return acc_rate;
}

int Character::getMove() const {
    return move;
}

int Character::getStr() const {
    return str;
}