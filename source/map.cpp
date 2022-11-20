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

Map::Map() : Map(10, 10, '+') {}

Map::Map(int rows, int cols, char back){
    plr = Player(rand() % rows, rand() % cols);

    mnstr.push_back(Monster(rand() % rows, rand() % cols, '0', 1, 1, 1));
    
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
                mnstr[i].setCoords(rand() % rows, rand() % cols);
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
        grid[mnstr[i].getRow()][mnstr[i].getCol()] = mnstr[i].getToken();
    }
    
    // bool overlap = true;
    // while(overlap) {
    //     overlap = false;
    //     for(int i = 0; i < mnstr.size(); i++) {
    //         if(checkOverlap(i)) {
    //             overlap = true;
                
    //         }
    //     }
    // }
}

int Map::getNumRows() const {
    return grid.size();
}

int Map::getNumCols() const {
    return grid[0].size();
}

void Map::printGrid() const {

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
                cout << "[" << grid[i][j] << "]"; 
                cursor_edge = true;
            }
            else{
                if(!cursor_edge){
                    cout << " ";
                }
                else{
                    cursor_edge = false;
                }
                cout << grid[i][j];
            }
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