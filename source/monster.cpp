#include "..\headers\monster.h"

Monster::Monster() : Monster("MONSTER", "Indescribable creature, indescribably ugly", 
    0,0,'0',6,2,1,0, 3, 10, 1, 3, 1, .75, 0.0) {}

Monster::Monster(string monst_name, string monst_desc, int col_coord, int row_coord, char symbol, int sight_range, int behaveP,
    int behaveS, int behaveD, int spd, int max_health, int range, int damage, int damage_var, 
    float accuracy, float accuracy_decay) : Character(symbol, row_coord, col_coord, spd, max_health, 
    range, damage, damage_var, accuracy, accuracy_decay, 0) {
    sight = sight_range;
    behave[0] = behaveP;
    behave[1] = behaveS;
    behave[2] = behaveD;
    provoked = false;
    spotted = false;
    dead = false;
    b_index = 2;

    name = monst_name;
    desc = monst_desc;
}

void Monster::updateCoords() {
    col = dest[0];
    row = dest[1];
}

bool Monster::canApproach(Player plr) const {
    if(getDistS(plr.getCol(), plr.getRow()) <= (move+1)*(move+1)) { //distance formula
        //move+1: the closest adjacent tile to the player is 1 closer than the player is, so we can move adjacent to any tile that is just outside our range
        return true;
    }
    return false;
}

bool Monster::canSee(Player plr) {
    //if the monster has at any point seen the player, the monster will have spotted the player
    //we don't need to set spotted back to false
    if(getDistS(plr.getCol(), plr.getRow()) <= sight*sight) { //distance formula
        spotted = true;
    }

    return spotted;
}

bool Monster::isProvoked() {
    if(cur_hp < max_hp) {
        provoked = true;
    }

    return provoked;
}

int Monster::checkBehavior() {
    //behavior priority: provoked -> spotted -> default
    if(provoked) {
        b_index = 0;
    } else if(spotted) {
        b_index = 1;
    } else {
        b_index = 2;
    }
    
    return b_index;
}

void Monster::setDest(Player plr, int max_cols, int max_rows) {
//make behavior current with monster state
    //check provoked
    isProvoked();
    //check sight
    canSee(plr);
    //check behavior
    checkBehavior();

    //random movement
    if(behave[b_index] == 0) {
        randMove(plr, max_cols, max_rows);
    }
    //approach the player
    if(behave[b_index] == 1) {
        //can move adjacent to player:
        if(canApproach(plr)) {
            adjMove(plr, max_cols, max_rows);
        }
        //cannot move adjacent to player:
        else {
            chaseMove(plr, max_cols, max_rows);
        }
    }
    //flee from the player
    if(behave[b_index] == 2) {
        fleeMove(plr, max_cols, max_rows);
    }
}

void Monster::adjMove(Player plr, int max_cols, int max_rows) {
    dest[0] = plr.getCol();
    dest[1] = plr.getRow();
    //see which side of the player is closest to the monster, and move there
    if(getDistS(dest[0], dest[1]) > getDistS(plr.getCol() - 1, plr.getRow()) && plr.getCol() - 1 >= 0) {
        dest[0] = plr.getCol() - 1;
        dest[1] = plr.getRow();
    }
    if(getDistS(dest[0], dest[1]) > getDistS(plr.getCol(), plr.getRow() - 1) && plr.getRow() - 1 >= 0) {
        dest[0] = plr.getCol();
        dest[1] = plr.getRow() - 1;
    }
    if(getDistS(dest[0], dest[1]) > getDistS(plr.getCol() + 1, plr.getRow()) && plr.getCol() + 1 < max_cols) {
        dest[0] = plr.getCol() + 1;
        dest[1] = plr.getRow();
    }
    if(getDistS(dest[0], dest[1]) > getDistS(plr.getCol(), plr.getRow() + 1) && plr.getRow() + 1 < max_rows) {
        dest[0] = plr.getCol();
        dest[1] = plr.getRow() + 1;
    }
}

void Monster::chaseMove(Player plr, int max_cols, int max_rows) {
    //takes `move` steps, with each step getting closer to the player
    int min_dist_s; //the distance to the tile adjacent to the monster that is closest to the player
    int new_dist_s; //checks distance to adjacent tiles to find the least value

    //the destination of the current 'step'
    int cur_col;
    int cur_row;

    //the final location of the current 'step'
    dest[0] = col;
    dest[1] = row;

    for(int i = 0; i < move; i++) {
        cur_col = dest[0];
        cur_row = dest[1];
        //systematically check: no step, leftward step, upward step, rightward step, downward step
        
        //no step
        min_dist_s = distanceS(cur_col, cur_row, plr.getCol(), plr.getRow());
        
        if(cur_col - 1 >= 0) { //left
            new_dist_s = distanceS(cur_col - 1, cur_row, plr.getCol(), plr.getRow());
            if(min_dist_s > new_dist_s) {
                min_dist_s = new_dist_s;
                dest[0] = cur_col - 1;
                dest[1] = cur_row;
            }
        }
        if(cur_row - 1 >= 0) { //up
            new_dist_s = distanceS(cur_col, cur_row - 1, plr.getCol(), plr.getRow());
            if(min_dist_s > new_dist_s){
                min_dist_s = new_dist_s;
                dest[0] = cur_col;
                dest[1] = cur_row - 1;
            }
        }
        if(cur_col + 1 < max_cols) { //right
            new_dist_s = distanceS(cur_col + 1, cur_row, plr.getCol(), plr.getRow());
            if(min_dist_s > new_dist_s) {
                min_dist_s = new_dist_s;
                dest[0] = cur_col + 1;
                dest[1] = cur_row;
            }
        }
        if(cur_row + 1 < max_rows) { //down
            new_dist_s = distanceS(cur_col, cur_row + 1, plr.getCol(), plr.getRow());
            if(min_dist_s > new_dist_s) {
                dest[0] = cur_col;
                dest[1] = cur_row + 1;
            }
        }
    }
}

void Monster::fleeMove(Player plr, int max_cols, int max_rows) {
    //take `move` steps, with each step getting further from the player
    int max_dist_s; //furthest adjacent tile from player
    int new_dist_s; //checks every adjacent tile to find the most distance

    //the location of monster after each step
    int cur_col;
    int cur_row;
    
    //the destination of the next step
    dest[0] = col;
    dest[1] = row;

    for(int i = 0; i < move; i++) {
        //assume no move
        cur_col = dest[0];
        cur_row = dest[1];
        max_dist_s = distanceS(cur_col, cur_row, plr.getCol(), plr.getRow());

        if(cur_col - 1 >= 0) { //left
            new_dist_s = distanceS(cur_col - 1, cur_row, plr.getCol(), plr.getRow());
            if(max_dist_s < new_dist_s) {
                max_dist_s = new_dist_s;
                dest[0] = cur_col - 1;
                dest[1] = cur_row;
            }
        }
        if(cur_row - 1 >= 0) { //up
            new_dist_s = distanceS(cur_col, cur_row - 1, plr.getCol(), plr.getRow());
            if(max_dist_s < new_dist_s){
                max_dist_s = new_dist_s;
                dest[0] = cur_col;
                dest[1] = cur_row - 1;
            }
        }
        if(cur_col + 1 < max_cols) { //right
            new_dist_s = distanceS(cur_col + 1, cur_row, plr.getCol(), plr.getRow());
            if(max_dist_s < new_dist_s) {
                max_dist_s = new_dist_s;
                dest[0] = cur_col + 1;
                dest[1] = cur_row;
            }
        }
        if(cur_row + 1 < max_rows) { //down
            new_dist_s = distanceS(cur_col, cur_row + 1, plr.getCol(), plr.getRow());
            if(max_dist_s < new_dist_s) {
                dest[0] = cur_col;
                dest[1] = cur_row + 1;
            }
        }
    }
}

void Monster::randMove(Player plr, int max_cols, int max_rows) {
    int dir; //[0,3], for left, up, right, down
    //checks if the space in the random direction is valid [0,max_coord)
    int cur_col;
    int cur_row;
    //the destination of each step
    dest[0] = col;
    dest[1] = row;

    for(int i = 0; i < move; i++) {
        dir = rand() % 4;
        cur_col = dest[0];
        cur_row = dest[1];
        if(cur_col - 1 >= 0 && dir == 0) {
            dest[0] = cur_col - 1;
            dest[1] = cur_row;
        }
        if(cur_row - 1 >= 0 && dir == 1) {
            dest[0] = cur_col;
            dest[1] = cur_row - 1;
        }
        if(cur_col + 1 < max_cols && dir == 2) {
            dest[0] = cur_col + 1;
            dest[1] = cur_row;
        }
        if(cur_row + 1 < max_rows && dir == 3) {
            dest[0] = cur_col;
            dest[1] = cur_row + 1;
        }
    }
}

int Monster::distanceS(int col_a, int row_a, int col_b, int row_b) const {
    return ((col_a - col_b) * (col_a - col_b) + (row_a - row_b) * (row_a - row_b)); //distance formula
}

int Monster::getDistS(int col_coord, int row_coord) const {
    int dcol = col_coord - col; //run
    int drow = row_coord - row; //rise

    return (dcol*dcol + drow*drow); //distance formula
}

int Monster::getCurBehavior() const {
    return behave[b_index];
}

char Monster::getToken() const {
    return token;
}

string Monster::getName() const {
    return name;
}

string Monster::getDesc() const {
    return desc;
}