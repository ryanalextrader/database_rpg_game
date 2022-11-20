#include <iostream>
#include "..\headers\map.h"
#include "..\headers\consInteraction.h"

void Map::changeColor(int row_coord, int col_coord) const{
    if(grid[row_coord][col_coord] == plr.getToken()){
        setTextColor(9);
    }
    else if (grid[row_coord][col_coord] != bkgrnd){
        setTextColor(4);
    }
    else if(plr.canMove(row_coord, col_coord)){
        setTextColor(10);
    }
    else{
        setTextColor(15);
    }
}

bool Map::checkOverlap(int i) {
    if(mnstr[i].getCol() == plr.getCol() && mnstr[i].getRow() == plr.getRow()) {
        return true;
    }
    for(int j = 0; j < mnstr.size(); j++) {
        if((mnstr[i].getCol() == mnstr[j].getCol() && mnstr[i].getRow() == mnstr[j].getRow()) && (j != i)) {
            return true;
        }
    }
    return false;
}

void Map::handleOverlap(Monster & mnstr){
    int dir = rand() % 4;
    bool flag = true;
    while(flag) { //this loop will run AT MOST twice. If we are against 1 col wall, we are not against the other, and same with row walls
        flag = false;
        if(dir == 0 && mnstr.getCol() - 1 >= 0){
            mnstr.setCoords(mnstr.getCol() - 1, mnstr.getRow());
            return;
        } else if(dir == 0) {
            dir++;
        }
        if(dir == 1 && mnstr.getCol() + 1 < grid[0].size()){
            mnstr.setCoords(mnstr.getCol() + 1, mnstr.getRow());
            return;
        } else if(dir == 1){
            dir--; //go back through the loop to hit the prior statement
            flag = true;
        }
        if(dir == 2 && mnstr.getRow() - 1 >= 0){
            mnstr.setCoords(mnstr.getCol(), mnstr.getRow() - 1);
            return;
        } else if(dir == 2){
            dir++;
        }
        if(dir == 3 && mnstr.getRow() + 1 < grid.size()){
            mnstr.setCoords(mnstr.getCol(), mnstr.getRow() + 1);
            return;
        } else if(dir == 3){
            dir--;
            flag = true;
        }
    }
}

Map::Map() : Map(12, 12, '+', 7) {}

Map::Map(int rows, int cols, char back, int num_monst){
    plr = Player(rand() % rows, rand() % cols);

    for(int i = 0; i < num_monst; i++) {
        mnstr.push_back(Monster(rand() % rows, rand() % cols, '0', 1, 1, 1));
    }
    
    bkgrnd = back;
    grid.assign(rows, vector<char>());
    for(int i = 0; i < rows; i++){
        grid[i].assign(cols, back);
    }
    grid[plr.getRow()][plr.getCol()] = plr.getToken();

    //make sure no monsters overlap with any other monsters or the player
    bool overlap = true;
    while(overlap) {
        overlap = false;
        for(int i = 0; i < mnstr.size(); i++) {
            if(checkOverlap(i)) {
                handleOverlap(mnstr[i]);
                overlap = true;
            }
        }
    }

    //add monsters to grid
    for(int i = 0; i < mnstr.size(); i++) {
        grid[mnstr[i].getRow()][mnstr[i].getCol()] = mnstr[i].getToken();
    }
}

void Map::setNumRowsCols(int row, int col){
    grid.resize(row);
    for(int i = 0; i < row; i++){
        grid[i].resize(col);
    }
    crsr.moveCursor(0,0,1,1);
}

void Map::moveCursor(char dir){
    if(dir == 'W'){
        crsr.moveCursor(crsr.getRow() - 1, crsr.getCol(), grid.size(), grid[0].size());
    } else if(dir == 'A') {
        crsr.moveCursor(crsr.getRow(), crsr.getCol() - 1, grid.size(), grid[0].size());
    } else if(dir == 'S') {
        crsr.moveCursor(crsr.getRow() + 1, crsr.getCol(), grid.size(), grid[0].size());
    } else if(dir == 'D') {
        crsr.moveCursor(crsr.getRow(), crsr.getCol() + 1, grid.size(), grid[0].size());
    } else {
        return;
    }
}

bool Map::movePlayer() {
    bool hasMoved = false;
    for(int i = 0; i < mnstr.size(); i++) {
        //check if we are moving on top of a monster
        if(crsr.getCol() == mnstr[i].getCol() && crsr.getRow() == mnstr[i].getRow()) {
            return false;
        }
    }

    grid[plr.getRow()][plr.getCol()] = bkgrnd;
    if(plr.updateCoords(crsr.getRow(), crsr.getCol())) {
        hasMoved = true;
    }
    grid[plr.getRow()][plr.getCol()] = plr.getToken();

    return hasMoved;
}

void Map::moveMonsters() {
    for(int i = 0; i < mnstr.size(); i++) {
        mnstr[i].setDest(plr, grid[i].size(), grid.size());

        grid[mnstr[i].getRow()][mnstr[i].getCol()] = bkgrnd;
        mnstr[i].updateCoords();
    }
    bool overlap = true;
    while(overlap) {
        overlap = false;
        for(int i = 0; i < mnstr.size(); i++) {
            if(checkOverlap(i)) {
                overlap = true;
                handleOverlap(mnstr[i]);
            }
        }
    }
    for(int i = 0; i < mnstr.size(); i++) {
        grid[mnstr[i].getRow()][mnstr[i].getCol()] = mnstr[i].getToken();
    }
}

int Map::getNumRows() const {
    return grid.size();
}

int Map::getNumCols() const {
    return grid[0].size();
}

void Map::printGrid() {

    setCursorPos(0, 0);

    for(int i = 0; i < 2 * (grid[0].size() + 2) - 1; i++) {
        cout << "#";
    }
    
    cout << endl;

    bool cursor_edge = false;
    for(int i = 0; i < grid.size(); i++){
        cout << "#";
        for(int j = 0; j < grid[0].size(); j++){
            changeColor(i, j);
            if(i == crsr.getRow() && j == crsr.getCol()) {
                if(grid[i][j] != plr.getToken() && grid[i][j] != bkgrnd && plr.canAttack(i, j)){
                    setTextColor(14);
                    cout << "[";
                    changeColor(i,j);
                    cout << grid[i][j];
                    setTextColor(14);
                    cout << "]";
                }
                else{
                    cout << "[" << grid[i][j] << "]"; 
                }
                cursor_edge = true;
            }
            else{
                if(!cursor_edge && !(plr.canAttack(i, j) && grid[i][j] != plr.getToken() && grid[i][j] != bkgrnd)){
                    cout << " ";
                }
                else if(!cursor_edge && plr.canAttack(i, j) && grid[i][j] != plr.getToken() && grid[i][j] != bkgrnd){
                    cout << "!";
                }
                else{
                    cursor_edge = false;
                }
                cout << grid[i][j];
            }
            //if(cursor_edge && monster_loc)
            //move i/o cursor before grid[i][j]
            //print yellow cursor
            //move after
        }
        if(!cursor_edge){
            cout << " ";
        }
        setTextColor(15);
        cout << "#\n";
        cursor_edge = false;
    }
    
    for(int i = 0; i < 2 * (grid[0].size() + 2) - 1; i++) {
        cout << "#";
    }
}