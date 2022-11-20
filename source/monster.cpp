#include "..\headers\monster.h"

Monster::Monster() : Monster(0,0,'0',2,1,0) {}

Monster::Monster(int col_coord, int row_coord, char symbol, int behaveP, int behaveD, int behaveB) {
    col = col_coord;
    row = row_coord;
    token = symbol;
    //set sight/move
    sight = 5;
    move = 3;
    behave[0] = behaveP;
    behave[1] = behaveD;
    behave[2] = behaveB;
    provoked = false;
    b_index = 2;
}

void Monster::setCoords(int col_coord, int row_coord) {
    col = col_coord;
    row = row_coord;
}

void Monster::updateCoords() {
    col = dest[0];
    row = dest[1];
}

bool Monster::canMove(int col_coord, int row_coord) const {
    if(getDistS(col_coord, row_coord) <= move*move) {
        return true;
    }
    return false;
}

bool Monster::canApproach(Player plr) const {
    if(getDistS(plr.getCol(), plr.getRow()) <= (move+1)*(move+1)) {
        return true;
    }
    return false;
}

bool Monster::canSee(Player plr) {
    if(provoked) {
        b_index = 0;
    }

    if(getDistS(plr.getCol(), plr.getRow()) <= sight*sight) {
        if(!provoked) {
            b_index = 1;
        }
        return true;
    }

    if(!provoked) {
        b_index = 2;
    }

    return false;
}

void Monster::setDest(Player plr, int max_cols, int max_rows) {
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

char Monster::getToken() const {
    return token;
}

int Monster::getCol() const {
    return col;
}

int Monster::getRow() const {
    return row;
}