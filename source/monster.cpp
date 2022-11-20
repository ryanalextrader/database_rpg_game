#include "..\headers\monster.h"

Monster::Monster() : Monster(0,0,'0',2,1,0) {}

Monster::Monster(int x_coord, int y_coord, char symbol, int behaveP, int behaveD, int behaveB) {
    x = x_coord;
    y = y_coord;
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

void Monster::updateCoords() {
    x = dest[0];
    y = dest[1];
}

bool Monster::canMove(int x_coord, int y_coord) const {
    if(getDist(x_coord, y_coord) <= move*move) {
        return true;
    }
    return false;
}

bool Monster::canApproach(Player plr) const {
    if(getDist(plr.getX(), plr.getY()) <= (move+1)*(move+1)) {
        return true;
    }
    return false;
}

bool Monster::canSee(Player plr) {
    if(getDist(plr.getX(), plr.getY()) <= sight*sight) {
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

void Monster::setDest(Player plr) {
    //approach
    if(behave[b_index] == 1) {
        //can move to player
        if(canApproach(plr)) {
            dest[0] = plr.getX() - 1;
            dest[1] = plr.getY();
            if(getDist(dest[0], dest[1]) > getDist(plr.getX(), plr.getY() - 1)) {
                dest[0] = plr.getX();
                dest[1] = plr.getY() - 1;
            }
            if(getDist(dest[0], dest[1]) > getDist(plr.getX() + 1, plr.getY())) {
                dest[0] = plr.getX() + 1;
                dest[1] = plr.getY();
            }
            if(getDist(dest[0], dest[1]) > getDist(plr.getX(), plr.getY() + 1)) {
                dest[0] = plr.getX();
                dest[1] = plr.getY() + 1;
            }
            return;
        }
        //cannot move directly to player
        //get slope between monster and player
        int rise = plr.getY() - y;
        int run = plr.getX() - x;
        //calculate distance to move in x direction: x^2 + (x * dy / dx)^2 = move^2 -> x^2 * (1 + dy^2 / dx^2) = m^2
        int dx = sqrt(move*move * (1 + (run / rise)));
        dest[0] = x + dx;
        dest[1] = y + dx * (rise / run);
        return;
    }
    
}

int Monster::getDist(int x_coord, int y_coord) const {
    int dx = x_coord - x;
    int dy = y_coord - y;

    return (dx*dx + dy*dy);
}

char Monster::getToken() const {
    return token;
}

int Monster::getX() const {
    return x;
}

int Monster::getY() const {
    return y;
}