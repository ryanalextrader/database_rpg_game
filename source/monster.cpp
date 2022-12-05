#include "..\headers\monster.h"

Monster::Monster() : Monster("MONSTER", "Indescribable creature, indescribably ugly", 
    0,0,'0',6,2,1,0, 3, 10, 1, 3, 1, .75, 0.0) {}

Monster::Monster(string monst_name, string monst_desc, int col_coord, int row_coord, char symbol, int sight_range, int behaveP,
    int behaveS, int behaveD, int spd, int max_health, int range, int damage, int damage_var, 
    float accuracy, float accuracy_decay) : Character(symbol, row_coord, col_coord, spd, max_health, 
    range, damage, damage_var, accuracy, accuracy_decay, 0) {
    // col = col_coord;
    // row = row_coord;
    // token = symbol;
    // //set sight/move
    // move = 3;
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
    if(getDistS(plr.getCol(), plr.getRow()) <= (move+1)*(move+1)) {
        return true;
    }
    return false;
}

bool Monster::canSee(Player plr) {
    if(getDistS(plr.getCol(), plr.getRow()) <= sight*sight) {
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

    //random
    if(behave[b_index] == 0) {
        randMove(plr, max_cols, max_rows);
    }
    //approach
    if(behave[b_index] == 1) {
        //can move to player:
        if(canApproach(plr)) {
            adjMove(plr, max_cols, max_rows);
        }
        //cannot move directly to player:
        else {
            chaseMove(plr, max_cols, max_rows);
        }
    }
    //flee
    if(behave[b_index] == 2) {
        fleeMove(plr, max_cols, max_rows);
    }
}

void Monster::adjMove(Player plr, int max_cols, int max_rows) {
    dest[0] = plr.getCol();
    dest[1] = plr.getRow();
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
    int min_dist_s;
    int new_dist_s;
    int cur_col;
    int cur_row;
    dest[0] = col;
    dest[1] = row;
    for(int i = 0; i < move; i++) {
        cur_col = dest[0];
        cur_row = dest[1];
        min_dist_s = distanceS(cur_col, cur_row, plr.getCol(), plr.getRow());
        if(cur_col - 1 >= 0) {
            new_dist_s = distanceS(cur_col - 1, cur_row, plr.getCol(), plr.getRow());
            if(min_dist_s > new_dist_s) {
                min_dist_s = new_dist_s;
                dest[0] = cur_col - 1;
                dest[1] = cur_row;
            }
        }
        if(cur_row - 1 >= 0) {
            new_dist_s = distanceS(cur_col, cur_row - 1, plr.getCol(), plr.getRow());
            if(min_dist_s > new_dist_s){
                min_dist_s = new_dist_s;
                dest[0] = cur_col;
                dest[1] = cur_row - 1;
            }
        }
        if(cur_col + 1 < max_cols) {
            new_dist_s = distanceS(cur_col + 1, cur_row, plr.getCol(), plr.getRow());
            if(min_dist_s > new_dist_s) {
                min_dist_s = new_dist_s;
                dest[0] = cur_col + 1;
                dest[1] = cur_row;
            }
        }
        if(cur_row + 1 < max_rows) {
            new_dist_s = distanceS(cur_col, cur_row + 1, plr.getCol(), plr.getRow());
            if(min_dist_s > new_dist_s) {
                dest[0] = cur_col;
                dest[1] = cur_row + 1;
            }
        }
    }
}

void Monster::fleeMove(Player plr, int max_cols, int max_rows) {
    int max_dist_s;
    int new_dist_s;
    int cur_col;
    int cur_row;
    dest[0] = col;
    dest[1] = row;
    for(int i = 0; i < move; i++) {
        cur_col = dest[0];
        cur_row = dest[1];
        max_dist_s = distanceS(cur_col, cur_row, plr.getCol(), plr.getRow());
        if(cur_col - 1 >= 0) {
            new_dist_s = distanceS(cur_col - 1, cur_row, plr.getCol(), plr.getRow());
            if(max_dist_s < new_dist_s) {
                max_dist_s = new_dist_s;
                dest[0] = cur_col - 1;
                dest[1] = cur_row;
            }
        }
        if(cur_row - 1 >= 0) {
            new_dist_s = distanceS(cur_col, cur_row - 1, plr.getCol(), plr.getRow());
            if(max_dist_s < new_dist_s){
                max_dist_s = new_dist_s;
                dest[0] = cur_col;
                dest[1] = cur_row - 1;
            }
        }
        if(cur_col + 1 < max_cols) {
            new_dist_s = distanceS(cur_col + 1, cur_row, plr.getCol(), plr.getRow());
            if(max_dist_s < new_dist_s) {
                max_dist_s = new_dist_s;
                dest[0] = cur_col + 1;
                dest[1] = cur_row;
            }
        }
        if(cur_row + 1 < max_rows) {
            new_dist_s = distanceS(cur_col, cur_row + 1, plr.getCol(), plr.getRow());
            if(max_dist_s < new_dist_s) {
                dest[0] = cur_col;
                dest[1] = cur_row + 1;
            }
        }
    }
}

void Monster::randMove(Player plr, int max_cols, int max_rows) {
    int dir;
    int cur_col;
    int cur_row;
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
    return ((col_a - col_b) * (col_a - col_b) + (row_a - row_b) * (row_a - row_b));
}

int Monster::getDistS(int col_coord, int row_coord) const {
    int dcol = col_coord - col;
    int drow = row_coord - row;

    return (dcol*dcol + drow*drow);
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